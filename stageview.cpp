#include "stageview.h"
#include "stagescene.h"

StageView::StageView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAlignment(Qt::AlignLeft|Qt::AlignTop);
    setOptimizationFlags(DontSavePainterState);
    setRenderHints(QPainter::Antialiasing);
    setScene(d.stageScene = new StageScene(this));
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void StageView::resizeEvent(QResizeEvent *e)
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(e);
}
