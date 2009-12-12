#include "stagemodel.h"

Play::~Play()
{
    foreach(Act *act, acts) {
        act->play = 0;
        delete act;
    }
    foreach(Character *character, characters) {
        character->play = 0;
        delete character;
    }
    foreach(Role *role, roles) {
        role->character = 0;
        delete role;
    }
}

static QString randomWord()
{
    QString t;
    const int size = (rand() % 8) + 3;
    t.reserve(size);
    for (int i=0; i<size; ++i) {
        t += QChar(((int)rand() % 26) + 'a');
    }
    return t;
}


Play *Play::createRandomPlay()
{
    QStringList names = (QStringList() << "Demarco" << "Dion" << "Chikae" << "Beyonce"
                         << "Deion" << "Jayla" << "Deon" << "Demond" << "Ayana" << "Elon"
                         << "Kimani" << "Aisha" << "Tupac" << "Kayla" << "Shaniqua" << "Imani"
                         << "Cleavon" << "Jazara" << "Kya" << "Malik" << "Zari" << "Elroi"
                         << "Laqueta" << "Jelani" << "Darius" << "Amir" << "Jahari" );
    Play *play = new Play;
    play->name = randomWord();
    for (int i=0; i<8; ++i) {
        Character *character = new Character(play);
        character->name = names.takeAt(rand() % names.size());
        play->characters.append(character);
    }

    for (int i=0; i<8; ++i) {
        Actor *actor = new Actor;
        actor->name = names.takeAt(rand() % names.size());
        play->actors.append(actor);
    }

    for (int i=0; i<8; ++i) {
        Role *role = new Role(play->characters.at(i), play->actors.at(i));
        play->roles.append(role);
    }

    return play;
}


enum { Format = 1, Magic = 0xabbaf00b };
void Play::save(QIODevice *device) const
{
    Q_ASSERT(device);
    Q_ASSERT(device->isWritable());
    QDataStream ds(device);
    ds << quint32(Magic) << quint32(Format);
    ds << name;
    ds << notes;
    ds << qint32(characters.size());
    for (int i=0; i<characters.size(); ++i) {
        const Character *character = characters.at(i);
        ds << character->name << character->notes;
    }

    ds << qint32(actors.size());
    for (int i=0; i<actors.size(); ++i) {
        const Actor *actor = actors.at(i);
        ds << actor->name << actor->notes;
    }

    ds << qint32(roles.size());
    for (int i=0; i<roles.size(); ++i) {
        const Role *role = roles.at(i);
        ds << role->character->name << role->actor->name << role->notes;
    }

    ds << qint32(acts.size());
    for (int i=0; i<acts.size(); ++i) {
        const Act *a = acts.at(i);
        ds << a->name << a->notes << qint32(a->frames.size());
        for (int j=0; j<a->frames.size(); ++j) {
            const Frame *frame = a->frames.at(j);
            ds << frame->seconds << frame->startTime << frame->notes << qint32(frame->events.size());
            for (int k=0; k<frame->events.size(); ++k) {
                const Event *event = frame->events.at(k);
                ds << event->role->character->name
                   << event->role->actor->name << event->position
                   << event->angle << event->line << event->notes;
            }
        }
    }
}

static inline uint qHash(const QStringList &list)
{
    return qHash(list.join("|"));
}

Play *Play::load(QIODevice *device)
{
    Q_ASSERT(device);
    Q_ASSERT(device->isReadable());
    QDataStream ds(device);
    quint32 ui32;
    qint32 i32;
    ds >> ui32;
    if (ui32 != Magic)
        return 0;
    ds >> ui32;
    if (ui32 != Format)
        return 0;

    Play *play = new Play;
    ds >> play->name >> play->notes;
    ds >> i32;
    QHash<QString, Character*> characters;
    QHash<QString, Actor*> actors;
    QHash<QStringList, Role*> roles;
    for (int i=0; i<i32; ++i) {
        Character *character = new Character(play);
        ds >> character->name >> character->notes;
        characters[character->name] = character;
        play->characters.append(character);
    }
    ds >> i32;
    for (int i=0; i<i32; ++i) {
        Actor *actor = new Actor;
        ds >> actor->name >> actor->notes;
        actors[actor->name] = actor;
        play->actors.append(actor);
    }
    ds >> i32;
    for (int i=0; i<i32; ++i) {
        QString characterName, actorName;
        ds >> characterName >> actorName;
        Character *character = characters.value(characterName);
        Actor *actor = actors.value(actorName);
        Role *role = new Role(character, actor);
        roles[(QStringList() << characterName << actorName)] = role;

        if (!role->character || !role->actor)
            goto error;
    }
    ds >> i32;
    for (int i=0; i<i32; ++i) {
        Act *act = new Act(play);
        ds >> act->name >> act->notes;
        play->acts.append(act);

        qint32 frameCount;
        ds >> frameCount;
        for (int j=0; j<frameCount; ++j) {
            Frame *frame = new Frame(act);
            ds >> frame->seconds >> frame->startTime >> frame->notes;
            act->frames.append(frame);

            qint32 eventCount;
            ds >> eventCount;
            for (int k=0; k<eventCount; ++k) {
                QString characterName, actorName;
                ds >> characterName >> actorName;
                Role *role = roles.value((QStringList() << characterName << actorName));
                Event *event;
                if (!role)
                    goto error;
                event = new Event(frame, role);
                frame->events.append(event);
            }
        }
    }

    return play;
error:
    delete play;
    return 0;
}

Act::~Act()
{
    if (play)
        play->acts.removeOne(this);
    foreach(Frame *frame, frames) {
        frame->act = 0;
        delete frame;
    }
}

Frame::~Frame()
{
    if (act)
        act->frames.removeOne(this);
    foreach(Event *event, events) {
        event->frame = 0;
        delete event;
    }
}

PlayModel::PlayModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

void PlayModel::setPlay(Play *play)
{

}

QModelIndex PlayModel::index(int row, int column, const QModelIndex &parent) const
{

}

QModelIndex PlayModel::parent(const QModelIndex &child) const
{

}

int PlayModel::rowCount(const QModelIndex &parent) const
{

}

int PlayModel::columnCount(const QModelIndex &parent) const
{

}

QVariant PlayModel::data(const QModelIndex &index, int role ) const
{

}

bool PlayModel::setData(const QModelIndex &index, const QVariant &value, int role )
{

}