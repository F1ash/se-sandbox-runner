#include "startup.h"

StartUp::StartUp(QObject *parent=0) :
    QThread(parent)
{
  ;
}
StartUp::~StartUp()
{
  ;
}
void StartUp::setElemProcess(ElemProcess *jP)
{
  jobProc = jP;
}
void StartUp::start()
{
  ;
}
void StartUp::stop()
{
  ;
}
