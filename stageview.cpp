#include "stageview.h"
#include "stagescene.h"

StageView::StageView(QWidget *parent)
    : QGraphicsView(parent)
{
    setOptimizationFlags(DontSavePainterState);
    setScene(d.stageScene = new StageScene(this));
}

void StageView::resizeEvent(QResizeEvent *e)
{
    fitInView(rect(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(e);
}
