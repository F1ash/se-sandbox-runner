#ifndef OUTPUT_THREAD_H
#define OUTPUT_THREAD_H

#include "help_thread.h"

class OutputThread : public HelpThread
{
    Q_OBJECT
public:
    explicit OutputThread(QObject *parent = nullptr);
    void        run();
};

#endif // OUTPUT_THREAD_H
