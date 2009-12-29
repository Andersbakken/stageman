#include "window.h"
#include "stageview.h"
#include "stagemodel.h"
#include "stagescene.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *w = new QWidget(this);
    QVBoxLayout *l = new QVBoxLayout(w);
    l->addWidget(d.cmbView = new QComboBox, 0, Qt::AlignLeft);
    l->addWidget(d.splitter = new QSplitter(Qt::Horizontal));
    d.splitter->addWidget(d.treeView = new QTreeView);
    d.splitter->addWidget(d.widgetStack = new QStackedWidget);
    d.widgetStack->insertWidget(Scenes, d.stageView = new StageView);
    setCentralWidget(w);
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    d.splitter->restoreState(settings.value("splitter").toByteArray());
    d.play = Play::createRandomPlay();
    d.model = new StageModel(this);
    d.model->setPlay(d.play);
    d.treeView->setModel(d.model);
    connect(d.treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTreeViewCurrentChanged(QModelIndex)));
    connect(d.stageView->stageScene(), SIGNAL(statusChanged(QString)),
            statusBar(), SLOT(showMessage(QString)));

}

void Window::closeEvent(QCloseEvent *e)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", d.splitter->saveState());
    QMainWindow::closeEvent(e);
}

void Window::onTreeViewCurrentChanged(const QModelIndex &idx)
{
    if (StageNode *node = qVariantValue<StageNode*>(idx.data(ModelItem::StageNodeRole))) {
        if (Act *act = stageModelNodeCast<Act*>(node)) {
            d.stageView->stageScene()->setAct(act);
//         } else if (Frame *frame = stageModelNodeCast<Frame*>(node)) {
//             d.stageView->stageScene()->setCurrentFrame(frame);
        }
    }
}

void Window::onComboBoxCurrentChanged(int current)
{
    d.widgetStack->setCurrentIndex(current);
}
