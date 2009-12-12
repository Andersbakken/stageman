#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>

class StageView;
class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window(QWidget *parent = 0);
    void closeEvent(QCloseEvent *e);
private:
    struct Data {
        StageView *stageView;
        QSplitter *splitter;
        QSplitter *verticalSplitter;
        QTableView *objectTable, *sceneTable;
    } d;
};


#endif
