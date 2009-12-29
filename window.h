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
    void onTreeViewCurrentChanged(const QModelIndex &idx);
private:
    struct Data {
        StageView *stageView;
        QSplitter *splitter;
        QTreeView *treeView;
        Play *play;
        StageModel *model;
    } d;
};


#endif
