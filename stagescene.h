#ifndef STAGESCENE_H
#define STAGESCENE_H

#include <QtGui>
struct Act;
struct StageNode;
struct Frame;
class StageGraphicsItem : public QGraphicsWidget
{
    Q_OBJECT
public:
    StageGraphicsItem(StageNode *node);
    QAbstractAnimation *animation() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
    struct Data {
        StageNode *node;
        QAbstractAnimation *animation;
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
    void setCurrentFrame(Frame *frame);
public slots:
    void onSceneRectChanged(const QRectF &r);
signals:
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
