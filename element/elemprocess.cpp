#include "element/elemprocess.h"
#include <QDebug>

ElemProcess::ElemProcess(QObject *parent) :
    QProcess(parent)
{
  setWorkingDirectory(QDir::homePath());
  connect(this, SIGNAL(processState(bool)), this, SLOT(setProcessState(bool)));
  connect(this, SIGNAL(readyRead()), this, SLOT(sendMessage()));
  timerId = 0;
  commandLine = new String(this);
  _diff = 0;
}
ElemProcess::~ElemProcess()
{
  disconnect(this, SIGNAL(processState(bool)), this, SLOT(setProcessState(bool)));
  disconnect(this, SIGNAL(readyRead()), this, SLOT(sendMessage()));

  delete commandLine;
  commandLine = 0;
}
void ElemProcess::setItemReference(QListWidgetItem *i)
{
  item = i;
  name = item->text();
  proc_Status.insert("availability", QVariant(AVAILABLE));
  proc_Status.insert("isRunning", QVariant(STOPPED));
  proc_Status.insert("reason", QVariant(TO_RUN));
  proc_Status.insert("initName", QVariant(name));
  item->setData(Qt::UserRole, QVariant(proc_Status));
  item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg("-- STANDBY --"));
  fgBrush = item->foreground();
  bgBrush = item->foreground();
  settings.beginGroup(name);
  bool autoRun = settings.value("AutoRun", QVariant()).toBool();
  settings.endGroup();
  if ( autoRun ) runJob();
}
QStringList ElemProcess::getCommand()
{
  settings.beginGroup(name);
  guiApp = settings.value("GuiApp", QVariant()).toBool();
  runInTerm = settings.value("RunInTerm", QVariant()).toBool();
  cgroups = settings.value("CGroups", QVariant()).toBool();
  capabilities = settings.value("Capabilities", QVariant()).toBool();
  shred = settings.value("Shred", QVariant()).toBool();
  securityLayer = settings.value("SLeyer", QVariant()).toString();
  sandboxType = settings.value("SType", QVariant()).toString();
  execute = settings.value("Execute", QVariant()).toBool();
  session = settings.value("Session", QVariant()).toBool();
  command = settings.value("Command", QVariant()).toString();
  checkTimeout = settings.value("TimeOut", QVariant(10)).toInt();
  DPI = settings.value("DPI", QVariant()).toInt();
  WM = settings.value("WM", QVariant()).toString();
  windowHeight = settings.value("wHeight", QVariant()).toInt();
  windowWidth = settings.value("wWidth", QVariant()).toInt();
  includes = settings.value("Includes", QVariant()).toString();
  mountDirs = settings.value("Mount", QVariant()).toBool();
  tempDir = settings.value("TempDir", QVariant()).toString();
  homeDir = settings.value("HomeDir", QVariant()).toString();
  settings.endGroup();
  commandLine->clear();
  _commandBuild();
  return commandLine->getList();
}
void ElemProcess::appendChildren()
{
  children<<PID;
  //qDebug()<<children.join(" ")<<" begin";
  readChildren();
  //qDebug()<<children.join(" ")<<" end";
  emit processState(RUNNING);
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
  name = item->text();  // for use real Job name
  settings.beginGroup(name);
  runInTerm = settings.value("RunInTerm", QVariant()).toBool();
  settings.endGroup();

  proc_Status.insert("availability", QVariant(NOT_AVAILABLE));
  proc_Status.insert("isRunning", QVariant(RUNNING));
  item->setData(Qt::UserRole, QVariant(proc_Status));
  item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg("-- STARTED --"));
  setUnAvailableItemBrush();
  /* testing command */
  //start("/usr/bin/sandbox", QStringList()<<"-X"<<"-W"<<"kwin"<<"-t"<<"sandbox_web_t"<< "firefox");
  QStringList cmd;
  QString runApp;
  if (!runInTerm)
    {
      runApp = QString("/usr/bin/sandbox");
      cmd.append(getCommand());
    }
  else
    {
      QStringList _cmd;
      _cmd.append(getCommand());
      _cmd.prepend("/usr/bin/sandbox");
      //_cmd.prepend("'");
      //_cmd.append("'");
      cmd.append(_cmd.join(" "));
      runApp = QString("/usr/bin/xdg-terminal");
    };
  qDebug()<<runApp<<cmd.join(" ")<<name;
  start(runApp, cmd);

  bool started = waitForStarted();
  PID = QString::number(pid());
  if ( started )
    {
      QTimer::singleShot(checkTimeout * 1000, this, SLOT(appendChildren()));
      waitTimerId = startTimer(1000);
    }
  else emit processState(STOPPED);
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
  proc_Status.insert("isRunning", QVariant(STOPPED));
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
  if ( status )
    {
      item->setIcon(QIcon::fromTheme("process-stop"));
      item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg(PID));
      proc_Status.insert("isRunning", QVariant(RUNNING));
      proc_Status.insert("reason", QVariant(TO_STOP));
    }
  else
    {
      item->setIcon(QIcon::fromTheme("system-run"));
      item->setToolTip(QString("Process %1\nPID: %2").arg(name).arg("-- STANDBY --"));
      children.clear();
      PID.clear();
      proc_Status.insert("isRunning", QVariant(STOPPED));
      proc_Status.insert("reason", QVariant(TO_RUN));
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
     }
   else if ( _timerId && waitTimerId==_timerId )
     {
       if ( checkTimeout - _diff + 1 )
         {
           int percent = int ((float(checkTimeout-_diff)/checkTimeout)*100.0);
           if ( percent > 50 )
             item->setBackground(QBrush(QColor ( 255 - int(255.0*(percent-50)/50), 255, 0, 128 ) ));
           else
             item->setBackground(QBrush(QColor ( 255, int(255*percent/50), 0, 128 ) ));
           _diff++;
         }
       else
         {
           killTimer(waitTimerId);
           _diff = 0;
           item->setText(name);
           item->setForeground(fgBrush);
           item->setBackground(bgBrush);
         };
     };
 }

void ElemProcess::_commandBuild()
{
  if (capabilities) commandLine->appendCapabilities();
  if (cgroups) commandLine->appendCGroups();
  if (shred) commandLine->appendShred();
  if (guiApp && DPI) commandLine->appendDPI(DPI);
  if (!securityLayer.isEmpty() && session) commandLine->appendSecurityLayer(securityLayer);
  if (mountDirs) commandLine->appendMountDirs();
  if (guiApp) commandLine->appendGuiApp();
  if ( ( guiApp || mountDirs ) && !homeDir.isEmpty() ) commandLine->appendHomeDir(homeDir);
  if ( ( guiApp || mountDirs ) && !tempDir.isEmpty()) commandLine->appendTempDir(tempDir);
  if (!includes.isEmpty()) commandLine->appendIncludes(includes);
  if (guiApp)
    {
      if (!WM.isEmpty()) commandLine->appendWM(WM);
      if ( windowWidth && windowHeight )
          commandLine->appendWindowSize(windowWidth, windowHeight);
    };
  if (!sandboxType.isEmpty()) commandLine->appendSandboxType(sandboxType);
  if      (session) commandLine->appendSession();
  else if (execute) commandLine->appendCommand(command);
}
void ElemProcess::sendMessage()
{
  QByteArray _data;
  _data = readAllStandardOutput();
  _data.append("\n");
  _data.append(readAllStandardError());
  qDebug()<<QTextStream(&_data).readAll();
}
