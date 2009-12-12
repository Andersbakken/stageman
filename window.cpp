#include "window.h"
#include "stageview.h"
#include "stagemodel.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(d.splitter = new QSplitter(Qt::Horizontal));
    d.verticalSplitter = new QSplitter(Qt::Vertical);
    d.splitter->addWidget(d.verticalSplitter);
    d.splitter->addWidget(d.stageView = new StageView);
    d.verticalSplitter->addWidget(d.objectTable = new QTableView);
    d.verticalSplitter->addWidget(d.sceneTable = new QTableView);
    d.verticalSplitter->addWidget(d.treeView = new QTreeView);
    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    d.splitter->restoreState(settings.value("splitter").toByteArray());
    d.verticalSplitter->restoreState(settings.value("verticalSplitter").toByteArray());
    d.play = Play::createRandomPlay();
    d.model = new PlayModel(this);
    d.model->setPlay(d.play);
    d.objectTable->setModel(d.model);
    d.treeView->setModel(d.model);
//    d.verticalSplitter->setModel(d.model);
}

void Window::closeEvent(QCloseEvent *e)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", d.splitter->saveState());
    settings.setValue("verticalSplitter", d.verticalSplitter->saveState());
    QMainWindow::closeEvent(e);
}
