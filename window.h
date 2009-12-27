#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>

class StageView;
struct Play;
class StageModel;
class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window(QWidget *parent = 0);
    void closeEvent(QCloseEvent *e);
public slots:
    void onCurrentChanged(const QModelIndex &idx);
private:
    struct Data {
        StageView *stageView;
        QSplitter *splitter;
        QSplitter *verticalSplitter;
        QTableView *objectTable, *sceneTable;
        QTreeView *treeView;
        Play *play;
        StageModel *model;
    } d;
};


#endif
