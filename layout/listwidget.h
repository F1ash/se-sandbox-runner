#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QMessageBox>
#include "jobmenu.h"
#include "settings/settings.h"
#include "element/elemprocess.h"

#define TO_RUN true
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
    void removeJob(QString &);

public slots:
    void editItemAction();
    void addJobItem(const QString &s);
    void deleteCurrentJobItem();
    void runJob(QListWidgetItem *_item);
    void stopJob(QListWidgetItem *_item);
    void showMessage(QString &, QString &);

private :
    QIcon stateIcon;
    SettingsDialog *sDialog;

private slots:
    void jobItemClicked(const QPoint &pos);
    void jobItemDoubleClicked(QListWidgetItem *_item);
    void createJobProcess(QListWidgetItem *_item);
    void jobItemAction();
    void deleteCancelledCreation();

};

#endif
