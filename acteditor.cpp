#include "acteditor.h"
#include "stagemodel.h"

ActEditor::ActEditor(StageModel *model, QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    d.tableView = new QTableView;
    d.tableView->horizontalHeader()->hide();
    d.tableView->verticalHeader()->hide();
    d.model = 0;
    if (model)
        setModel(model);
}

void ActEditor::setModel(StageModel *model)
{
    d.tableView->setModel(model);
    if (model) {
        d.tableView->setRootIndex(model->indexFromItem(model->actsRoot()));
        connect(d.tableView, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this, SLOT(onTableCurrentRowChanged(QModelIndex)));
        d.tableView->selectionModel()->select(d.tableView->rootIndex().child(0, 0),
                                              QItemSelectionModel::SelectCurrent|QItemSelectionModel::Rows);
    }
}
void ActEditor::onTableCurrentRowChanged(const QModelIndex &row)
{
    d.notes->setPlainText(row.data(ModelItem::NotesRole).toString());
    d.name->setText(row.data().toString());
}

void ActEditor::onLineEditTextChanged(const QString &text)
{

}
