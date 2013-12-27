#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QMessageBox>
#include "jobmenu.h"
#include "settings/settings.h"
#include "element/elemprocess.h"
#include <QDebug>

#define TO_RUN  true
#define TO_STOP false

class JobList : public QListWidget
{
    Q_OBJECT

public:
    JobList(QWidget *parent);
    ~JobList();

    QMap<QString, ElemProcess*> *jobProcess;

signals:
    void clickedItem(QString);
    void removeJob(QString&);

public slots:
    void editItemAction();
    void addJobItem(const QString&);
    void deleteCurrentJobItem();
    void runJob(QListWidgetItem*);
    void stopJob(QListWidgetItem*);

private :
    QIcon           stateIcon;
    SettingsDialog *sDialog;

private slots:
    void jobItemClicked(const QPoint&);
    void jobItemDoubleClicked(QListWidgetItem*);
    void createJobProcess(QListWidgetItem*);
    void jobItemKillAction();
    void jobItemRunAction();
    void checkJob(QListWidgetItem*, bool);
    void deleteCancelledCreation();
    void showMessage(QString, QString);

};

#endif //LISTWIDGET_H
