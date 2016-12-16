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
    explicit JobList(QWidget *parent = 0);

    QMap<QString, ElemProcess*>
                        *jobProcess;
    JobItemModel        *jobItemModel;

signals:
    void                 clickedItem(QString);
    void                 removeJob(QString&);
    void                 sendMsg(QString&);

public slots:
    void                 editItemAction();
    void                 addJobItem(const QString&);
    void                 deleteCurrentJobItem();
    void                 runJob(QModelIndex&);
    void                 undockJob(QModelIndex&);
    void                 stopJob(QModelIndex&);

private :
    SettingsDialog      *sDialog;
    ProgressBarDelegate *progressBarDlg;

private slots:
    void                 jobItemClicked(const QPoint&);
    void                 createJobProcess(QModelIndex&);
    void                 jobItemDoubleClicked(const QModelIndex&);
    void                 jobItemUndockAction();
    void                 jobItemKillAction();
    void                 jobItemRunAction();
    void                 checkJob(QModelIndex&, bool);
    void                 deleteCancelledCreation();
    void                 showMessage(QString, QString);

};

#endif //LISTWIDGET_H
