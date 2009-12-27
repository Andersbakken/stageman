#ifndef STAGESCENE_H
#define STAGESCENE_H

#include <QtGui>
struct Act;
struct StageNode;
struct Frame;

class LineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    LineItem()
        : QGraphicsLineItem()
    {
        setPen(QPen(Qt::blue, 5));
    }
};
class StageScene;
class StageGraphicsItem : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle)
public:
    StageGraphicsItem(StageNode *node);
    QAbstractAnimation *animation() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    StageScene *stageScene() const;
    qreal angle() const;
    void setAngle(qreal angle);
    void setAngleItem(const QLineF &line);
signals:
    void statusChanged(const QString &string);
private:
    struct Data {
        qreal angle;
        StageNode *node;
        QAbstractAnimation *animation;
        enum State { Normal, Moving, Rotating } state;
        QPointF grab;
    } d;
};

struct Role;
class StageScene : public QGraphicsScene
{
    Q_OBJECT
public:
    StageScene(QObject *parent = 0);
    void setAct(Act *act);
    void animate();
    void timerEvent(QTimerEvent *e);
    void keyPressEvent(QKeyEvent *event);
//    void setCurrentFrame(Frame *frame);
public slots:
    void onSceneRectChanged(const QRectF &r);
signals:
    void statusChanged(const QString &);
    void nextFrame();
    void previousFrame();
private:
    struct Data {
        Act *act;
        QState *root;
        QBasicTimer nextFrameTimer;
        QList<QState*> frameStates;
        QHash<Role*, StageGraphicsItem*> roles;
        QStateMachine *stateMachine;
        QGraphicsSimpleTextItem *textItem;
    } d;
};


#endif
