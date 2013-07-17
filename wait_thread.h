#include <QThread>
#include "layout/listwidget.h"
//#include "element/elemprocess.h"

class Wait : public QThread
{
    Q_OBJECT;
public:
    Wait(QObject *parent);
    ~Wait();

signals:

private:
    JobList *wdg;

public slots:
    void setWdgReference(JobList *);

private slots:
    void run();
};

