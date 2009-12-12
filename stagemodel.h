#ifndef MODEL_H
#define MODEL_H

#include <QtGui>

struct BaseNode
{
    enum NodeType {
        PlayType,
        ActType,
        FrameType,
        EventType,
        RoleType,
        CharacterType,
        ActorType
    };
    virtual ~BaseNode() {}
    virtual NodeType type() const = 0;
    QHash<QString, QVariant> notes;
};


// do I need a time class?

struct Act;
struct Character;
struct Frame;
struct Event;
struct Role;
struct Actor;
struct Play : public BaseNode
{
    ~Play();
    enum { Type = PlayType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    QString name; // author, venue, stage etc?
    QList<Character*> characters;
    QList<Role*> roles; // ### is this right?
    QList<Act*> acts;
    QList<Actor*> actors;

    void save(QIODevice *device) const;
    static Play *load(QIODevice *device);
    static Play *createRandomPlay();
};

struct Act : public BaseNode
{
    Act(Play *p)
        : play(p)
    {}
    enum { Type = ActType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    ~Act();
    Play *play;
    QString name;
    QList<Frame*> frames;
};

struct Frame : public BaseNode
{
    Frame(Act *a)
        : act(a), seconds(-1), startTime(-1)
    {}
    enum { Type = FrameType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    ~Frame();
    Act *act;
    int seconds, startTime; // ### should I store start-time or calculate?
    QList<Event*> events;
};

// ### bad name?
struct Event : public BaseNode
{
    Event(Frame *f, Role *r)
        : frame(f), role(r), angle(0)
    {}
    enum { Type = EventType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    ~Event()
    {
        if (frame)
            frame->events.removeOne(this);
    }
    Frame *frame;
    Role *role;
    QPointF position;
    qreal angle;
    QString line;
};

struct Character : public BaseNode
{
    Character(Play *p)
        : play(p)
    {
    }
    enum { Type = CharacterType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    ~Character()
    {
        if (play)
            play->characters.removeOne(this); // ### this might have repercussions for things
    }

    Play *play;
    QString name; // this is a unique id
};

struct Role : public BaseNode
{
    Role(Character *c, Actor *a)
        : character(c), actor(a)
    {}
    enum { Type = RoleType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    ~Role()
    {
        if (character && character->play) {
            character->play->roles.removeOne(this);
        }
    }
    Character *character;
    Actor *actor;
};

struct Actor : public BaseNode
{
    enum { Type = ActorType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    QString name, address;
    QPixmap picture;

    // name is a unique id
};

class PlayModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    PlayModel(QObject *parent = 0);
    void setPlay(Play *play);
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};



#endif
