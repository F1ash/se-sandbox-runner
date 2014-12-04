#ifndef WAIT_THREAD_H
#define WAIT_THREAD_H

#include <QThread>
#include "layout/listwidget.h"

class Wait : public QThread
{
    Q_OBJECT
public:
    Wait(QObject *parent);
    ~Wait();

signals:

private:
    JobList  *wdg;
    bool      to_kill;

public slots:
    void setWdgReference(JobList*);
    void setMode(bool);

private slots:
    void run();
};

#endif //WAIT_THREAD_H
