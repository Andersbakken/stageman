#ifndef MODEL_H
#define MODEL_H

#include <QtGui>

struct BaseNode
{
    virtual ~BaseNode() {}
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

    QString name; // author, venue, stage etc?
    QList<Character*> characters;
    QList<Role*> roles; // ### is this right?
    QList<Act*> acts;
    QList<Actor*> actors;

    void save(QIODevice *device) const;
    static Play *load(QIODevice *device);
};

struct Act : public BaseNode
{
    Act(Play *p)
        : play(p)
    {}
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
    ~Frame();
    Act *act;
    int seconds, startTime; // ### should I store start-time or calculate?
    QList<Event*> events;
};

// ### bad name?
struct Event : public BaseNode
{
    enum Type {
        Movement,
        Line
    };

    Event(Type t, Frame *f, Role *r)
        : type(t), frame(f), role(r), angle(0)
    {}
    ~Event()
    {
        if (frame)
            frame->events.removeOne(this);
    }
    Type type;
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
    QString name, address;
    QPixmap picture;

    // name is a unique id
};


#endif
