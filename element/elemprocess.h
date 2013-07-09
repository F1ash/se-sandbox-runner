#include <QStringList>
#include <QListWidgetItem>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QTimerEvent>
#include <QMap>
#include "signal.h"
#include "settings/settings_stuff.h"

#define RUNNED true
#define STOPPED false
#define TO_RUN true
#define TO_STOP false
#define SIGZERO 0
#define AVAILABLE true
#define NOT_AVAILABLE false

class ElemProcess : public QProcess
{
    Q_OBJECT

public:
    ElemProcess(QObject *parent = 0);
    ~ElemProcess();
    QListWidgetItem *item;

public slots:
    void setItemReference(QListWidgetItem*);
    void setSettingsReference(SettingsStuff&);
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
    SettingsStuff settings;

private slots:
    QStringList CommandBuild();
    void appendChildren();
    void readChildren();
    void setUnAvailableItemBrush();
    void setProcessState(bool status);
    void timerEvent(QTimerEvent *);
};
