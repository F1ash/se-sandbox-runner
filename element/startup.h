#include <QThread>
#include "elemprocess.h"

class StartUp : public QThread
{
  Q_OBJECT;
public:
    StartUp(QObject *parent);
    ~StartUp();
signals:

private:
    ElemProcess *jobProc;

public slots:
    void setElemProcess(ElemProcess*);
    void start();
    void stop();
private slots:

};
