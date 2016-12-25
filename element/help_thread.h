#ifndef HELP_THREAD_H
#define HELP_THREAD_H

#include <QThread>

class HelpThread : public QThread
{
    Q_OBJECT
public:
    explicit HelpThread(QObject *parent = nullptr);
    void        setDisplay(const QString&);
    QString     getSelection() const;

private:
    QString     display;
    QString     selection;
};

#endif // HELP_THREAD_H
