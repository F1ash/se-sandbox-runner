#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QTreeView>
#include <QMessageBox>
#include "jobmenu.h"
#include "jobitem_model.h"
#include "progressbar_delegate.h"
#include "settings/settings.h"
#include "element/elemprocess.h"
#include <QDebug>

#define TO_RUN  true
#define TO_STOP false

class JobList : public QTreeView
{
    Q_OBJECT

public:
    JobList(QWidget *parent);
    ~JobList();

    QMap<QString, ElemProcess*>  *jobProcess;
    JobItemModel                 *jobItemModel;

signals:
    void clickedItem(QString);
    void removeJob(QString&);

public slots:
    void editItemAction();
    void addJobItem(const QString&);
    void deleteCurrentJobItem();
    void runJob(QModelIndex&);
    void stopJob(QModelIndex&);

private :
    SettingsDialog       *sDialog;
    ProgressBarDelegate  *progressBarDlg;

private slots:
    void jobItemClicked(const QPoint&);
    void createJobProcess(QModelIndex&);
    void jobItemDoubleClicked(const QModelIndex&);
    void jobItemKillAction();
    void jobItemRunAction();
    void checkJob(QModelIndex&, bool);
    void deleteCancelledCreation();
    void showMessage(QString, QString);

};

#endif //LISTWIDGET_H
