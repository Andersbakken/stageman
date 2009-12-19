#ifndef MODEL_H
#define MODEL_H

#include <QtGui>

struct StageNode
{
    enum NodeType {
        PlayType,
        ActType,
        FrameType,
        EventType,
        RoleType,
        CharacterType,
        ActorType,
        ObjectType
    };
    virtual ~StageNode() {}
    virtual NodeType type() const = 0;
    QHash<QString, QVariant> notes;
    QRectF rect; // ### ugly
};


// do I need a time class?

struct Act;
struct Character;
struct Frame;
struct Event;
struct Role;
struct Actor;
struct Object;
struct Play : public StageNode
{
    ~Play();
    enum { Type = PlayType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    QString name; // author, venue, stage etc?
    QList<Character*> characters;
    QList<Role*> roles; // ### is this right?
    QList<Object*> objects; // ### is this right?
    QList<Act*> acts;
    QList<Actor*> actors;

    void save(QIODevice *device) const;
    static Play *load(QIODevice *device);
    static Play *createRandomPlay();
};

struct Act : public StageNode
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

struct Frame : public StageNode
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
struct Event : public StageNode
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

class Object : public StageNode
{
public:
//    Object(const QPainterPath &p)
    Object(const QRectF &r)
    {
        rect = r;
    }
    enum { Type = ObjectType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    QString name;
//    QPainterPath path;
//    QRectF rect;
};

struct Character : public StageNode
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

struct Role : public StageNode
{
    Role(Character *c, Actor *a)
        : character(c), actor(a)
    {
        rect = QRectF(0, 0, 100, 100);
    }
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

struct Actor : public StageNode
{
    enum { Type = ActorType };
    virtual NodeType type() const { return static_cast<NodeType>(Type); }

    QString name;
    QPixmap picture;

    // name is a unique id
};

template <typename T> inline T stageModelNodeCast(StageNode *node)
{
    if (node && node->type() == static_cast<StageNode::NodeType>(static_cast<T>(0)->Type))
        return static_cast<T>(node);
    return 0;
}

class ModelItem : public QStandardItem
{
public:
    ModelItem(StageNode *node)
    {
        d.node = node;
    }

    QVariant data(int role = Qt::UserRole + 1) const;
    StageNode *node() const {
        return d.node;
    }
private:
    struct Data {
        StageNode *node;
    } d;
};

class StageModel : public QStandardItemModel
{
    Q_OBJECT
public:
    StageModel(QObject *parent);
    void setPlay(Play *play);
    Play *play() const;
    void refresh();
private:
    struct Data {
        Play *play;
    } d;
};

#endif
