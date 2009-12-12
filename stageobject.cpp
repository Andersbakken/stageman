#include "stageobject.h"

void StageObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (!d.pixmap.isNull())
        painter->drawPixmap(option->rect, d.pixmap);
    if (!d.text.isEmpty()) {
        painter->setPen(d.pen);
        painter->drawText(option->rect, Qt::AlignCenter, d.text); // scale to fit?
    }
}

