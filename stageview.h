#ifndef STAGEVIEW_H
#define STAGEVIEW_H

#include <QtGui>
class StageScene;
class StageView : public QGraphicsView
{
    Q_OBJECT
public:
    StageView(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *e);
    StageScene *stageScene() const { return d.stageScene; }
private:
    struct Data {
        StageScene *stageScene;
    } d;
};


#endif
