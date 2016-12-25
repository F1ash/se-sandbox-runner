#ifndef COPY_THREAD_H
#define COPY_THREAD_H

#include "help_thread.h"

class CopyThread : public HelpThread
{
    Q_OBJECT
public:
    explicit CopyThread(QObject *parent = nullptr);
    void        run();
};

#endif // COPY_THREAD_H
