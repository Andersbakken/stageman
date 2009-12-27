#include "stagescene.h"
#include "stagemodel.h"

StageScene::StageScene(QObject *parent)
    : QGraphicsScene(parent)
{
    connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(onSceneRectChanged(QRectF)));
    d.root = 0;
    d.stateMachine = 0;
    d.act = 0;
    d.textItem = 0;
}

void StageScene::setAct(Act *act)
{
    clear();
    qDeleteAll(d.frameStates);
    d.frameStates.clear();
    delete d.root;
    d.root = 0;
    d.roles.clear();
    d.act = act;
    delete d.stateMachine;
    d.stateMachine = 0;
    d.textItem = 0;
    if (act) {
        d.textItem = addSimpleText(act->name);
        d.textItem->setPos(0, 0);
        int counter = 0;
        d.root = new QState;
        QState *previousState = 0;
        foreach(Frame *frame, act->frames) {
            QState *frameState = new QState(d.root);
            frameState->setObjectName(QString::number(counter++));
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
                    item->resize(100, 100);
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
        d.stateMachine = new QStateMachine(this);
        d.stateMachine->addState(d.root);
        d.stateMachine->setInitialState(d.root);
        d.stateMachine->start();
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
    case Qt::Key_Space:
        if (event->key() == Qt::Key_Left || event->modifiers() == Qt::ShiftModifier) {
            emit previousFrame();
            break;
        }
        // fall through
    case Qt::Key_Right:
        emit nextFrame();
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

void StageGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    switch (d.node->type()) {
    case StageNode::RoleType: {
        static QPixmap pixmap;
        if (pixmap.isNull()) {
            pixmap.load("smileyface.jpg");
            pixmap.setMask(pixmap.createHeuristicMask());
        }
        painter->drawPixmap(option->rect, pixmap);
        painter->drawText(option->rect, Qt::AlignCenter, static_cast<Role*>(d.node)->character->name);
        break;
    }
    case StageNode::ObjectType:
        painter->fillRect(option->rect, Qt::red);
        break;
    default:
        break;
    }
}


QAbstractAnimation *StageGraphicsItem::animation() const
{
    return d.animation;
}

void StageScene::setCurrentFrame(Frame *frame)
{
    setAct(0);
    clear();
    foreach(Event *event, frame->events) {
        StageGraphicsItem *item = new StageGraphicsItem(event->role);
        item->setRotation(event->angle);
        item->setPos(event->position);
        addItem(item);
    }
}
void StageScene::onSceneRectChanged(const QRectF &r)
{
    if (d.textItem)
        d.textItem->setPos(r.topLeft());
}
