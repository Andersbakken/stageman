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

static inline QPointF bound(const QRectF &rect, const QPointF &pos)
{
    return QPointF(qBound(rect.left(), pos.x(), rect.right()),
                   qBound(rect.top(), pos.y(), rect.bottom()));
}

static const char *quotes[] = {
    "A wretched soul, bruised with adversity, We bid be quiet when we hear it cry; But were we burdened with like weight of pain, As much or more we should ourselves complain.",
    "Action is eloquence.",
    "And since you know you cannot see yourself, so well as by reflection, I, your glass, will modestly discover to yourself, that of yourself which you yet know not of.",
    "And thus I clothe my naked villainy With old odd ends, stol'n forth of holy writ; And seem a saint, when most I play the devil. [info][add][mail]",
    "Assume a virtue, if you have it not.",
    "Be great in act, as you have been in thought.",
    "Blow, blow, thou winter wind",
    "Thou art not so unkind, As man's ingratitude.",
    "Conversation should be pleasant without scurrility, witty without affectation, free without indecency, learned without conceitedness, novel without falsehood.",
    "For they are yet ear-kissing arguments.",
    "Free from gross passion or of mirth or anger constant in spirit, not swerving with the blood, garnish'd and deck'd in modest compliment, not working with the eye without the ear, and but in purged judgement trusting neither? Such and so finely bolted didst thou seem.",
    "Glory is like a circle in the water, Which never ceaseth to enlarge itself, Till by broad spreading it disperses to naught.",
    "God bless thee; and put meekness in thy mind, love, charity, obedience, and true duty!",
    "He who has injured thee was either stronger or weaker than thee. If weaker, spare him; if stronger, spare thyself.",
    "His life was gentle; and the elements So mixed in him, that Nature might stand up, And say to all the world, THIS WAS A MAN!",
    "How poor are they who have not patience! What wound did ever heal but by degrees.",
    "How use doth breed a habit in a man.",
    "I am not bound to please thee with my answers.",
    "I did never know so full a voice issue from so empty a heart: but the saying is true 'The empty vessel makes the greatest sound'.",
    "I dote on his very absence.",
    "I feel within me a peace above all earthly dignities, a still and quiet conscience.",
    "I hate ingratitude more in a man than lying, vainness, babbling, drunkenness, or any taint of vice whose strong corruption inhabits our frail blood.",
    "I must be cruel only to be kind; Thus bad begins, and worse remains behind.",
    "I pray thee cease thy counsel, Which falls into mine ears as profitless as water in a sieve.",
    "I pray you bear me henceforth from the noise and rumour of the field, where I may think the remnant of my thoughts in peace, and part of this body and my soul with contemplation and devout desires.",
    "I wasted time, and now doth time waste me.",
    "I wish you well and so I take my leave,",
    "I Pray you know me when we meet again.",
    "Ill deeds are doubled with an evil word.",
    "In a false quarrel there is no true valour.",
    "In peace there's nothing so becomes a man as modest stillness and humility.",
    "In time we hate that which we often fear.",
    0
};

static inline QString randomLine()
{
    int hops = rand() % 160;
    int idx = 0;
    for (int i=0; i<hops; ++i) {
        if (!quotes[++idx]) {
            idx = 0;
        }
    }
    return quotes[idx];
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

    static const char *actNames[] = { "First Act", "Second Act", "Last Act", 0 };
    for (int i=0; actNames[i]; ++i) {
        Act *act = new Act(play);
        act->name = actNames[i];
        int time = 0;
        QList<Role*> rolesInvolvedInAct = play->roles;
        for (int i=0; i<rolesInvolvedInAct.size() / 3; ++i) {
            rolesInvolvedInAct.removeAt(rand() % rolesInvolvedInAct.size());
        }

        QHash<Role*, QPointF> lastPos;

        do {
            Frame *frame = new Frame(act);
            frame->startTime = time;
            frame->seconds = (rand() % 120) + 10;
            act->frames.append(frame);
            for (int i=0; i<rolesInvolvedInAct.size(); ++i) {
                Role *role = rolesInvolvedInAct.at(i);
                Event *event = new Event(frame, role);
                QPointF pos;
                static const QRect sceneRect(0, 0, 1000, 1000);
                if (lastPos.contains(role)) {
                    QLineF line(lastPos.value(role), QPointF(1, 1));
                    line.setAngle(rand() % 360);
                    line.setLength(200);
                    pos = bound(sceneRect, line.p2());
                } else {
                    pos = QPointF(rand() % sceneRect.width(), rand() % sceneRect.height()) + sceneRect.topLeft();
                }

                event->position = pos;
                qDebug() << pos;
                event->angle = rand() % 360;
                event->line = randomLine();
                frame->events.append(event);
                lastPos[role] = pos;
            }
        } while (rand() % 20 != 0);

        play->acts.append(act);
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

QVariant ModelItem::data(int role) const
{
    if (d.node && role == Qt::DisplayRole) {
        switch (d.node->type()) {
        case StageNode::ObjectType:
            return static_cast<Object*>(d.node)->name;
        case StageNode::PlayType:
            return static_cast<Play*>(d.node)->name;
        case StageNode::ActType:
            return static_cast<Act*>(d.node)->name;
        case StageNode::FrameType:
            switch (column()) {
            case 0: return static_cast<const Frame*>(d.node)->seconds;
            case 1: return static_cast<const Frame*>(d.node)->startTime;
            }
            break;
        case StageNode::EventType:
            switch (column()) {
            case 0: return static_cast<const Event*>(d.node)->role->character->name;
            case 1: return static_cast<const Event*>(d.node)->role->actor->name;
            case 2: return QString("%1 %2").
                    arg(static_cast<const Event*>(d.node)->position.x()).
                    arg(static_cast<const Event*>(d.node)->position.y());
            case 3: return QString("%1%2").arg(static_cast<const Event*>(d.node)->angle).
                    arg(QChar(0x00B0));
            }
            break;
        case StageNode::RoleType:
            switch (column()) {
            case 0: return static_cast<const Role*>(d.node)->character->name;
            case 1: return static_cast<const Role*>(d.node)->actor->name;
            }
            break;
        case StageNode::CharacterType:
            return static_cast<const Character*>(d.node)->name;
        case StageNode::ActorType:
            return static_cast<const Actor*>(d.node)->name;
        }
    }
    return QStandardItem::data(role);
}

StageModel::StageModel(QObject *parent)
    : QStandardItemModel(parent)
{
    setColumnCount(4);
    d.play = 0;
}

void StageModel::setPlay(Play *play)
{
    d.play = play;
    refresh();
}

Play * StageModel::play() const
{
    return d.play;
}

void StageModel::refresh()
{
    invisibleRootItem()->removeRows(0, invisibleRootItem()->rowCount());
    if (d.play) {
        ModelItem *root = new ModelItem(d.play);
        setItem(0, 0, root);
        QStandardItem *characters = new QStandardItem("Characters");
        root->setChild(0, 0, characters);
        for (int i=0; i<d.play->characters.size(); ++i) {
            characters->setChild(i, 0, new ModelItem(d.play->characters.at(i)));
        }
        QStandardItem *actors = new QStandardItem("Actors");
        root->setChild(1, 0, actors);
        for (int i=0; i<d.play->actors.size(); ++i) {
            actors->setChild(i, 0, new ModelItem(d.play->actors.at(i)));
        }
        QStandardItem *roles = new QStandardItem("Roles");
        root->setChild(2, 0, roles);
        for (int i=0; i<d.play->roles.size(); ++i) {
            for (int j=0; j<4; ++j) {
                roles->setChild(i, j, new ModelItem(d.play->roles.at(i)));
            }
        }

        QStandardItem *acts = new QStandardItem("Acts");
        root->setChild(3, 0, acts);
        for (int i=0; i<d.play->acts.size(); ++i) {
            Act *a = d.play->acts.at(i);
            ModelItem *act = new ModelItem(a);
            acts->setChild(i, 0, act);
            for (int j=0; j<a->frames.size(); ++j) {
                Frame *f = a->frames.at(j);
                ModelItem *frame;
                for (int jj=1; jj>=0; --jj) { // children must be child of item in column 0
                    frame = new ModelItem(f);
                    act->setChild(j, jj, frame);
                }

                for (int k=0; k<f->events.size(); ++k) {
                    Event *e = f->events.at(k);
                    for (int kk=0; kk<4; ++kk) {
                        ModelItem *event = new ModelItem(e);
                        frame->setChild(k, kk, event);
                    }
                }
            }
        }
    }
}

QSet<Role*> Act::rolesInAct() const
{
    QSet<Role*> ret;
    foreach(Frame *frame, frames) {
        foreach(Event *event, frame->events) {
            ret.insert(event->role);
        }
    }
    return ret;
}

QSet<Role*> Act::rolesNotInAct() const
{
    QSet<Role*> ret;
    if (play) {
        ret = play->roles.toSet();
        foreach(Frame *frame, frames) {
            foreach(Event *event, frame->events) {
                ret.remove(event->role);
            }
        }
    }
    return ret;
}

void StageModel::setCurrentItem(QStandardItem *standardItem)
{
    // change columns based on what is current
}
