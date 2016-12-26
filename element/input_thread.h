#ifndef INPUT_THREAD_H
#define INPUT_THREAD_H

#include "help_thread.h"

class InputThread : public HelpThread
{
    Q_OBJECT
public:
    explicit InputThread(QObject *parent = nullptr);
    void        run();
};

#endif // INPUT_THREAD_H
