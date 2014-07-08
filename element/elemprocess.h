#ifndef ELEMPROCESS_H
#define ELEMPROCESS_H

#include <QStringList>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QTimerEvent>
#include "signal.h"
#include "string_list.h"
#include "shred_thread.h"
#include "layout/jobitem_model.h"
#include <QDebug>

#define RUNNING         true
#define STOPPED         false
#define TO_RUN          true
#define TO_STOP         false
#define SIGZERO         0
#define AVAILABLE       true
#define NOT_AVAILABLE   false

#define TMP_FILE QString("TEMPORARY_FILE")

class ElemProcess : public QProcess
{
    Q_OBJECT
public:
    ElemProcess(QObject *parent = 0);
    ~ElemProcess();

public slots:
    void setItemReference(JobItemModel*, JobItemIndex*);
    void runJob();
    void killJob();
    void undockJob();

signals:
    void processState(bool);
    void procMsg(QString, QString);

private:
    JobItemModel  *own_model;
    JobItemIndex  *own_index;

    QStringList     children;
    QString         PID;
    QString         name;
    DATA            proc_Status;
    QSettings       settings;
    int             waitTimerId;
    int             timerId;

    String         *commandLine;
    int             checkTimeout;
    int             _diff;
    bool            shred;
    bool            mountDirs;
    QString         tempDir;
    QString         homeDir;
    QString         SELabel;
    ShredThread    *shredder;

private slots:
    QStringList getCommand();
    void appendChildren();
    void readChildren();
    void setProcessState(bool);
    void timerEvent(QTimerEvent*);
    void sendMessage();
    void shreddingFinished();
};

#endif //ELEMPROCESS_H
