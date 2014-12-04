#ifndef SHRED_THREAD_H
#define SHRED_THREAD_H

#include <QtCore/QThread>
#include "settings/settings.h"

class ShredThread : public QThread
{
    Q_OBJECT
public:
    explicit ShredThread(QObject *parent = 0);
    QString  tempDir;
    QString  homeDir;

signals:

public slots:
    void run();

};

#endif // SHRED_THREAD_H
