#ifndef ELEMPROCESS_H
#define ELEMPROCESS_H

#include <QStringList>
#include <QSettings>
#include <QListWidgetItem>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QTimerEvent>
#include "signal.h"
#include "string_list.h"
#include "settings/settings.h"
#include <QDebug>

#define RUNNING       true
#define STOPPED       false
#define TO_RUN        true
#define TO_STOP       false
#define SIGZERO       0
#define AVAILABLE     true
#define NOT_AVAILABLE false

class ElemProcess : public QProcess
{
    Q_OBJECT;

public:
    ElemProcess(QObject *parent = 0);
    ~ElemProcess();
    QListWidgetItem *item;

public slots:
    void setItemReference(QListWidgetItem*);
    void runJob();
    void killJob();

signals:
    void processState(bool);

private:
    QStringList  children;
    QString      PID;
    QString      name;
    QBrush       bgBrush;
    QBrush       fgBrush;
    QMap<QString, QVariant> proc_Status;
    QSettings    settings;
    int          waitTimerId;
    int          timerId;

    String  *commandLine;
    int      checkTimeout;
    int      _diff;
    bool     shred;
    bool     mountDirs;

private slots:
    QStringList getCommand();
    void appendChildren();
    void readChildren();
    void setUnAvailableItemBrush();
    void setProcessState(bool);
    void timerEvent(QTimerEvent*);
    void sendMessage();
};

#endif //ELEMPROCESS_H
