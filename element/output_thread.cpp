#include "output_thread.h"
extern "C" {
#include "xsel.h"
}

OutputThread::OutputThread(QObject *parent) :
    HelpThread(parent)
{

}

void OutputThread::run()
{
    char *o = cp_output_from_display(
                getDisplay().toUtf8().data());
    setSelection(o);
}
