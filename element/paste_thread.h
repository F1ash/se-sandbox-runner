#ifndef PASTE_THREAD_H
#define PASTE_THREAD_H

#include "help_thread.h"

class PasteThread : public HelpThread
{
    Q_OBJECT
public:
    explicit PasteThread(QObject *parent = nullptr);
    void        run();
};

#endif // PASTE_THREAD_H
