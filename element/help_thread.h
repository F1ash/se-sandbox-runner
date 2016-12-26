#ifndef HELP_THREAD_H
#define HELP_THREAD_H

#include <QThread>

class HelpThread : public QThread
{
    Q_OBJECT
public:
    explicit HelpThread(QObject *parent = nullptr);
    QString     getDisplay() const;
    void        setDisplay(const QString&);
    QString     getSelection() const;
    void        setSelection(const QString&);

private:
    QString     display;
    QString     selection;
};

#endif // HELP_THREAD_H
