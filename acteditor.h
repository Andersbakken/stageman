#ifndef ACTEDITOR_H
#define ACTEDITOR_H

#include <QtGui>
class StageModel;
class ActEditor : public QWidget
{
    Q_OBJECT
public:
    ActEditor(StageModel *model, QWidget *parent = 0);
    void setModel(StageModel *model);
public slots:
    void onTableCurrentRowChanged(const QModelIndex &row);
    void onLineEditTextChanged(const QString &text);
private:
    struct Data {
        StageModel *model;
        QTableView *tableView;
        QLineEdit *name;
        QTextEdit *notes;
        QPushButton *pbCommit;
    } d;
};


#endif
