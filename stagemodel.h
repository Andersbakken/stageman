#ifndef MODEL_H
#define MODEL_H

struct BaseNode
{
    virtual ~BaseNode() {}
    QString notes;
};


// do I need a time class?

struct Act;
struct Character;
struct Frame;
struct Event;
struct Play : public BaseNode
{
    ~Play()
    {
        foreach(Act *act, acts) {
            act->play = 0;
            delete act;
        }
        foreach(Character *character, characters) {
            character->play = 0;
            delete character;
        }
    }

    QString name;
    QList<Act*> acts;
    QList<Character*> characters;

    void save(QIODevice *device) const;
    static Play *load(QIODevice *device);
};

struct Act : public BaseNode
{
    Act(Play *p)
        : play(p)
    {}
    ~Act()
    {
        if (play)
            play->acts.removeOne(this);
        foreach(Frame *frame, frames) {
            frame->act = 0;
            delete frame;
        }
    }
    Play *play;
    QString name;
    QList<Frame*> frames;
};

struct Frame : public BaseNode
{
    Frame(Act *a)
        : act(a), seconds(-1), startTime(-1)
    {}
    ~Frame()
    {
        if (act)
            act->acts.removeOne(this);
        foreach(Event *event, events) {
            event->act = 0;
            delete event;
        }
    }
    Act *act;
    int seconds, startTime; // ### should I store start-time or calculate?
    QList<Event*> events;
};

// ### bad name?
struct Event : public BaseNode
{
    Event(Type t, Frame *f, Role *r)
        : type(t), frame(f), role(r), angle(0)
    {}
    ~Event()
    {
        if (frame)
            frame->events.removeOne(this);
    }
    enum Type {
        Movement,
        Line
    } type;
    Frame *frame;
    Role *Role;
    QPointF position;
    QString line;
    qreal angle;

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
    QString name;
};

struct Role : public BaseNode
{
    Role(Character *c, Actor *a)
        : character(c), actor(a)
    {}
    Character *character;
    Actor *actor;
};

struct Actor
{
    QString name, address;
    QPixmap picture;
};


#endif
