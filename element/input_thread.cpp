#include "input_thread.h"
extern "C" {
#include "xsel.h"
}

InputThread::InputThread(QObject *parent) :
    HelpThread(parent)
{

}

void InputThread::run()
{
    cp_input_to_display(
                getDisplay().toUtf8().data(),
                getSelection().toUtf8().data());
}
