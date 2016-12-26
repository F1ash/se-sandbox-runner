#include "help_thread.h"

HelpThread::HelpThread(QObject *parent) :
    QThread(parent)
{

}

QString HelpThread::getDisplay() const
{
    return display;
}

void HelpThread::setDisplay(const QString &d)
{
    display = d;
}

QString HelpThread::getSelection() const
{
    return selection;
}

void HelpThread::setSelection(const QString &sel)
{
    selection = sel;
}
