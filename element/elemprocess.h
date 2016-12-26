#ifndef ELEMPROCESS_H
#define ELEMPROCESS_H

#include <QStringList>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <QTime>
#include <QTimer>
#include <QTimerEvent>
#include <QClipboard>
#include "string_list.h"
#include "shred_thread.h"
#include "layout/jobitem_model.h"
#include "output_thread.h"
#include "input_thread.h"
#include <QDebug>

#define RUNNING         true
#define STOPPED         false
#define TO_RUN          true
#define TO_STOP         false
#define SIGZERO         0
#define AVAILABLE       true
#define NOT_AVAILABLE   false

#define TMP_DIR QString("TEMPORARY_DIR")

class ElemProcess : public QProcess
{
    Q_OBJECT
public:
    explicit ElemProcess(QObject *parent = 0);

public slots:
    void            setItemReference(JobItemModel*, JobItemIndex*);
    void            runJob();
    void            killJob();
    void            undockJob();

signals:
    void            processState(bool);
    void            procMsg(QString&);

private:
    JobItemModel   *own_model;
    JobItemIndex   *own_index;

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
    bool            copy_paste;
    int             cp_timerId;
    QString         selection;
    QClipboard     *clipboard;
    OutputThread   *outputThread;   // threads for operations
    InputThread    *inputThread;    // with sandboxed X session
    QString         tempDir;
    QString         homeDir;
    QString         SELabel;
    ShredThread    *shredder;

    void            startCopyPaste();
    void            stopCopyPaste();

private slots:
    QStringList     getCommand();
    void            appendChildren();
    void            readChildren();
    void            setProcessState(bool);
    void            timerEvent(QTimerEvent*);
    void            sendMessage();
    void            shreddingFinished();
    void            setShredState(uint);
    void            exchangeSelections();
    void            clipboardChanged(QClipboard::Mode);
};

#endif //ELEMPROCESS_H
