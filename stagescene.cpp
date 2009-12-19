#include "stagescene.h"
#include "stagemodel.h"

StageScene::StageScene(QObject *parent)
    : QGraphicsScene(parent)
{
    d.act = 0;
}

void StageScene::setAct(Act *act)
{
    qDeleteAll(d.frameStates);
    d.frameStates.clear();
    delete d.root;
    d.root = 0;
    qDeleteAll(d.roles);
    d.roles.clear();
    d.act = act;
    if (act) {
        d.root = new QState;
        QState *previousState = 0;
        foreach(Frame *frame, act->frames) {
            QState *frameState = new QState(d.root);
            d.frameStates.append(frameState);
            QAbstractTransition *transitionNext = 0, *transitionPrevious = 0;
            if (previousState) {
                transitionNext = previousState->addTransition(this, SIGNAL(nextFrame()), frameState);
                transitionPrevious = frameState->addTransition(this, SIGNAL(previousFrame()), previousState);
            } else {
                d.root->setInitialState(frameState);
            }

            foreach(Event *event, frame->events) {
                StageGraphicsItem *item = d.roles.value(event->role);
                if (!item) {
                    item = new StageGraphicsItem(event->role);
                    addItem(item);
                    d.roles[event->role] = item;
                }
//                frameState->assignProperty(item, "opacity",  // need a way to hide all items initially in each frame

                frameState->assignProperty(item, "pos", event->position);
                frameState->assignProperty(item, "rotation", event->angle);
                Q_ASSERT(!transitionNext == !transitionPrevious);
                if (transitionNext) {
                    transitionNext->addAnimation(item->animation());
                    transitionPrevious->addAnimation(item->animation());
                }
            }
            previousState = frameState;
        }
        d.stateMachine.addState(d.root);
        d.stateMachine.setInitialState(d.root);
        d.stateMachine.start();
    }
}

void StageScene::animate()
{

}

void StageScene::timerEvent(QTimerEvent *e)
{

}

void StageScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        emit nextFrame();
        break;
    case Qt::Key_Right:
        emit previousFrame();
        break;
    default:
        QGraphicsScene::keyPressEvent(event);
        return;
    }
    event->accept();
}


StageGraphicsItem::StageGraphicsItem(StageNode *node)
{
    d.node = node;
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(new QPropertyAnimation(this, "pos"));
    group->addAnimation(new QPropertyAnimation(this, "rotation"));
    d.animation = group;
}

QAbstractAnimation *StageGraphicsItem::animation() const
{
    return d.animation;
}
