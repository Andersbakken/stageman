#include "window.h"
#include "stageview.h"
#include "stagemodel.h"
#include "stagescene.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(tr("&Load"), this, SLOT(load()))->setEnabled(false);
    file->addAction(tr("&Save"), this, SLOT(save()))->setEnabled(false);
    file->addAction(tr("Save &as"), this, SLOT(saveAs()))->setEnabled(false);
    file->addSeparator();
    file->addAction(tr("&Quit"), this, SLOT(close()));
    QWidget *w = new QWidget(this);
    QVBoxLayout *l = new QVBoxLayout(w);
    l->addWidget(d.cmbView = new QComboBox, 0, Qt::AlignLeft);
    d.cmbView->addItem(tr("Scenes"));
    d.cmbView->addItem(tr("Acts"));
    d.cmbView->addItem(tr("Roles"));
    l->addWidget(d.splitter = new QSplitter(Qt::Horizontal));
    d.splitter->addWidget(d.treeView = new QTreeView);
    d.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d.splitter->addWidget(d.widgetStack = new QStackedWidget);
    d.widgetStack->insertWidget(Scenes, d.stageView = new StageView);
    d.widgetStack->insertWidget(Acts, d.actsTable = new QTableView);
    setCentralWidget(w);
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    d.splitter->restoreState(settings.value("splitter").toByteArray());
    d.play = Play::createRandomPlay();
    d.model = new StageModel(this);
    d.model->setPlay(d.play);
    d.actsTable->setModel(d.model);
    d.treeView->setModel(d.model);
    connect(d.treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTreeViewCurrentChanged(QModelIndex)));
    connect(d.stageView->stageScene(), SIGNAL(statusChanged(QString)),
            statusBar(), SLOT(showMessage(QString)));
    d.actsTable->setRootIndex(d.model->indexFromItem(d.model->actsRoot()));
    d.actsTable->horizontalHeader()->hide();
    d.actsTable->verticalHeader()->hide();
    connect(d.cmbView, SIGNAL(activated(int)), this, SLOT(onComboBoxCurrentChanged(int)));
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
    if (current < d.widgetStack->count())
        d.widgetStack->setCurrentIndex(current);
}

void Window::saveAs()
{

}

void Window::load()
{

}

void Window::save()
{

}
