#ifndef SHRED_THREAD_H
#define SHRED_THREAD_H

#include <QThread>
#include <QDir>
#include <QFileInfo>
#include "settings/settings.h"

typedef class {
private:
    qint64 size = 1;
public:
    qint64 current = 0;
    uint   one_percent = 0;
    uint   curr_percent = 0;
    void   setSize(quint64 _s) {
        size = _s;
        one_percent = (uint )size/100.0;
    }
    uint   percent () {
        curr_percent = (uint) (current*100.0/size);
        return curr_percent;
    }
    void clear () {
        current = 0;
        size = 1;
        curr_percent = 0;
        one_percent = 0;
    }
} PERCENT;

class ShredThread : public QThread
{
    Q_OBJECT
public:
    explicit ShredThread(QObject *parent = 0);
    QString  tempDir;
    QString  homeDir;

signals:
    void stateChanged(uint);

public slots:
    void run();
    void shredStateChanged(uint);

private slots:
    qint64 getDirSize(QString&);
};

#endif // SHRED_THREAD_H
