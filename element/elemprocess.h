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

#define RUNNED true
#define STOPPED false
#define TO_RUN true
#define TO_STOP false
#define SIGZERO 0
#define AVAILABLE true
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
    void processState(bool state);

private:
    QStringList children;
    QString PID;
    QString name;
    QBrush bgBrush;
    QBrush fgBrush;
    QMap<QString, QVariant> proc_Status;
    int timerId;
    QSettings settings;

    bool guiApp;
    bool cgroups;
    bool capabilities;
    bool shred;
    QString securityLayer;
    QString sandboxType;
    bool execute;
    bool session;
    QString command;
    int DPI;
    QString WM;
    int windowHeight;
    int windowWidth;
    QString includes;
    bool mountDirs;
    QString tempDir;
    QString homeDir;
    String *commandLine;

private slots:
    QStringList getCommand();
    void appendChildren();
    void readChildren();
    void setUnAvailableItemBrush();
    void setProcessState(bool status);
    void timerEvent(QTimerEvent *);
    void _commandBuild();
};

#endif
