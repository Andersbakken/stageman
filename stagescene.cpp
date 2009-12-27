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
                    connect(item, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
                    item->resize(100, 100);
                    addItem(item);
                    d.roles[event->role] = item;
                }
//                frameState->assignProperty(item, "opacity",  // need a way to hide all items initially in each frame

                frameState->assignProperty(item, "pos", event->position);
                frameState->assignProperty(item, "angle", event->angle);
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
    d.state = Data::Normal;
    d.node = node;
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(new QPropertyAnimation(this, "pos"));
    group->addAnimation(new QPropertyAnimation(this, "angle"));
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
        painter->setPen(Qt::black);
        painter->drawText(option->rect, Qt::AlignCenter, static_cast<Role*>(d.node)->character->name);
#if 0
        QString text;
        switch (d.state) {
        case Data::Rotating: text = QString("%1%2").arg(rotation()).arg(QChar(0x00B0)); break;
        case Data::Moving: text = QString("%1 %2").arg(x()).arg(y()); break;
        case Data::Normal:
            break;
        }
        if (!text.isEmpty()) {
            const QRect r = painter->fontMetrics().boundingRect(text);
            painter->fillRect(r, Qt::black);
            painter->setPen(Qt::white);
            painter->drawText(option->rect, Qt::AlignLeft|Qt::AlignTop, text);
        }
#endif
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

#if 0
void StageScene::setCurrentFrame(Frame *frame)
{
    setAct(0);
    clear();
    foreach(Event *event, frame->events) {
        StageGraphicsItem *item = new StageGraphicsItem(event->role);
        item->setAngle(event->angle);
        item->setPos(event->position);
        addItem(item);
    }
}
#endif
void StageScene::onSceneRectChanged(const QRectF &r)
{
    if (d.textItem)
        d.textItem->setPos(r.topLeft());
}

StageScene * StageGraphicsItem::stageScene() const
{
    return qobject_cast<StageScene*>(scene());
}

void StageGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    switch (event->button()) {
    case Qt::MidButton:
    case Qt::LeftButton:
        d.grab = event->scenePos() - pos();
        d.state = (event->button() == Qt::LeftButton ? Data::Moving : Data::Rotating);
        break;
    default:
        event->ignore();
        break;
    }
}

void StageGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch (d.state) {
    case Data::Moving:
        setPos(event->scenePos() - d.grab);
        emit statusChanged(QString("%0 - %1 %2").arg(d.node->text()).arg(x()).arg(y()));
        break;
    case Data::Rotating:
        setAngle(QLineF(d.grab + pos(), event->scenePos()).angle());
        setAngleItem(QLineF(d.grab, event->scenePos()));
        emit statusChanged(QString("%0 - %1 %2").arg(d.node->text()).arg(rotation()).arg(QChar(0x00B0))); break; // don't need to use arg for degrees sign really
        break;
    case Data::Normal:
        break;
    }
}

void StageGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    d.state = Data::Normal;
    setAngleItem(QLineF());
}

qreal StageGraphicsItem::angle() const
{
    return d.angle;
}

void StageGraphicsItem::setAngle(qreal angle)
{
    d.angle = angle;
    QTransform transform;
    const QPointF center = rect().center();
    transform.translate(center.x(), center.y());
    transform.rotate(angle);
    transform.translate(-center.x(), -center.y());
    setTransform(transform);
}

void StageGraphicsItem::setAngleItem(const QLineF &line)
{
    static QWeakPointer<LineItem> item;
    if (line.isNull()) {
        delete item.data();
        return;
    }
    if (!item) {
        item = new LineItem;
        item.data()->setZValue(10);
        scene()->addItem(item.data());
    }
    item.data()->setLine(line);
}
