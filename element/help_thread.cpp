#include "help_thread.h"
extern "C" {
#include "xsel.h"
}

HelpThread::HelpThread(QObject *parent) :
    QThread(parent)
{

}

void HelpThread::setDisplay(const QString &d)
{
    display = d;
}

QString HelpThread::getSelection() const
{
    return selection;
}
