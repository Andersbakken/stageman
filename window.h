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
    void onComboBoxCurrentChanged(int current);
private:
    enum { Scenes, Acts, Roles };
    struct Data {
        QComboBox *cmbView;
        StageView *stageView;
        QSplitter *splitter;
        QTreeView *treeView;
        Play *play;
        StageModel *model;
        QStackedWidget *widgetStack;
    } d;
};


#endif
