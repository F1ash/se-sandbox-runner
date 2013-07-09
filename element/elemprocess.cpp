#include "element/elemprocess.h"
#include <QDebug>

ElemProcess::ElemProcess(QObject *parent) :
    QProcess(parent)
{
  setWorkingDirectory(QDir::homePath());
  connect(this, SIGNAL(processState(bool)), this, SLOT(setProcessState(bool)));
  timerId = 0;
}
ElemProcess::~ElemProcess()
{

}
void ElemProcess::setItemReference(QListWidgetItem *i)
{
  item = i;
  name = item->text();
  proc_Status.insert("availability", QVariant(AVAILABLE));
  proc_Status.insert("isRunned", QVariant(STOPPED));
  proc_Status.insert("reason", QVariant(TO_RUN));
  proc_Status.insert("initName", QVariant(name));
  item->setData(Qt::UserRole, QVariant(proc_Status));
  item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg("-- STANDBY --"));
  fgBrush = item->foreground();
  bgBrush = item->foreground();
}
void ElemProcess::setSettingsReference(SettingsStuff &s)
{
  settings = s;
}
QStringList ElemProcess::CommandBuild()
{
  QStringList result;
  return result;
}
void ElemProcess::appendChildren()
{
  children<<PID;
  qDebug()<<children.join(" ")<<" begin";
  readChildren();
  qDebug()<<children.join(" ")<<" end";
  emit processState(RUNNED);
  timerId = startTimer(1000);
}
void ElemProcess::readChildren()
{
  children.removeDuplicates();
  QList<QString>::const_iterator i;
  QStringList _children;
  QString filename;
  filename = QString("/proc/%1/task/%1/children").arg(children.last());
  //qDebug()<<filename;
  QFile f;
  f.setFileName(filename);
  if (f.exists())
    {
      f.open(QIODevice::ReadOnly|QIODevice::Text);
      f.waitForReadyRead(10000);
      f.setTextModeEnabled(true);
      QByteArray _data;
      _data = f.readAll();
      //qDebug()<<_data<<" ???"<<f.bytesAvailable();
      _children.append((QString().append(_data.data())).split(" "));
    };
  f.close();

  //_children << QString().append(pgrep(pid)).split(" ");
  //qDebug()<<_children.join(" ")<<"--||--";

  for (i = _children.constBegin(); i != _children.constEnd(); ++i)
    {
      if ( !children.contains((*i).toUtf8().data()) && !(*i).isEmpty() )
            {
              //qDebug()<<(*i).toUtf8().data();
              children<<(*i).toUtf8().data();
              readChildren();
            };
    };
}
void ElemProcess::runJob()
{
  proc_Status.insert("availability", QVariant(NOT_AVAILABLE));
  proc_Status.insert("isRunned", QVariant(RUNNED));
  item->setData(Qt::UserRole, QVariant(proc_Status));
  item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg("-- STARTED --"));
  setUnAvailableItemBrush();
  start("/usr/bin/sandbox", QStringList()<<"-X"<<"-W"<<"kwin"<<"-t"<<"sandbox_web_t"<< "firefox");
  waitForStarted();
  PID = QString::number(pid());
  if (state()==QProcess::Running )
      QTimer::singleShot(10000, this, SLOT(appendChildren()));
}
void ElemProcess::setUnAvailableItemBrush()
{
  item->setForeground(QBrush(QColor ( 64, 64, 64, 168 )));
  item->setBackground(QBrush(QColor ( 168, 168, 168, 64 ), Qt::Dense6Pattern));
}
void ElemProcess::killJob()
{
  if (timerId)
    {
      killTimer(timerId);
      timerId = 0;
    };
  proc_Status.insert("availability", QVariant(NOT_AVAILABLE));
  proc_Status.insert("isRunned", QVariant(STOPPED));
  item->setData(Qt::UserRole, QVariant(proc_Status));
  setUnAvailableItemBrush();
  QList<QString>::const_iterator i;
  for (i = children.constBegin(); i != children.constEnd(); ++i)
    {
      pid_t _pid = (*i).toInt();
      if ( ::kill(_pid, SIGZERO)==0 )
        {
          //qDebug() <<_pid<< ::kill(_pid, SIGZERO);
          ::kill(_pid, SIGKILL);
        };
    };
  emit processState(STOPPED);
}
void ElemProcess::setProcessState(bool status)
{
  if (status)
    {
      item->setIcon(QIcon::fromTheme("process-stop"));
      item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg(PID));
      proc_Status.insert("isRunned", QVariant(RUNNED));
    }
  else
    {
      item->setIcon(QIcon::fromTheme("system-run"));
      item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg("-- STANDBY --"));
      children.clear();
      PID.clear();
      proc_Status.insert("isRunned", QVariant(STOPPED));
    };
  proc_Status.insert("availability", QVariant(AVAILABLE));
  item->setData(Qt::UserRole, QVariant(proc_Status));
  item->setForeground(fgBrush);
  item->setBackground(bgBrush);
}
void ElemProcess::timerEvent(QTimerEvent *event)
 {
   int _timerId = event->timerId();
   //qDebug() << "Timer ID:" << _timerId<< timerId;
   if ( _timerId && timerId==_timerId )
     {
       if ( ::kill(PID.toInt(), SIGZERO)!=0 ) killJob();
     };
 }
