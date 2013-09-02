#include "settings/settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
  setWindowTitle("<noname>");
  restoreGeometry(settings.value("SetDlgGeometry").toByteArray());
  setModal(false);
  setContentsMargins(1,1,1,1);
  initTabWidget();
  initButtons();
  initParameters();
  fullCommandWdg = new QTextEdit(this);
  fullCommandWdg->setReadOnly(true);
  commonLayout = new QVBoxLayout(this);
  commonLayout->addWidget(tabWidget);
  commonLayout->addWidget(fullCommandWdg);
  commonLayout->addWidget(buttons);
  setLayout(commonLayout);
  commandLine = new String(this);
  timerId = startTimer(1000);
}
SettingsDialog::~SettingsDialog()
{
  disconnect(w1->guiApp, SIGNAL(stateChanged(int)),this, SLOT(windowSetsEnable(int)));
  disconnect(w1->guiApp, SIGNAL(stateChanged(int)),w4, SLOT(setGuiCheckState(int)));
  disconnect(w1->nameEdit, SIGNAL(textChanged(QString)), this, SLOT(set_Title_Name(QString)));
  disconnect(w4, SIGNAL(guiStateChanged(bool)), w1->guiApp, SLOT(setChecked(bool)));
  disconnect(ok, SIGNAL(clicked()), this, SLOT(saveJob()));
  disconnect(cancel, SIGNAL(clicked()), this, SLOT(cancelJob()));
  disconnect(w1, SIGNAL(securityLevelState(bool)), this, SLOT(changeSecLevelState(bool)));

  delete ok;
  ok = 0;
  delete cancel;
  cancel = 0;
  delete w1;
  w1 = 0;
  delete w2;
  w2 = 0;
  delete w3;
  w3 = 0;
  delete w4;
  w4 = 0;
  delete tabWidget;
  tabWidget = 0;
  delete fullCommandWdg;
  fullCommandWdg = 0;
  delete buttonsLayout;
  buttonsLayout = 0;
  delete buttons;
  buttons = 0;
  delete commonLayout;
  commonLayout = 0;
  delete commandLine;
  commandLine = 0;
}
void SettingsDialog::initTabWidget()
{
  /*
   sandbox [-C] [-c] [-s] [ -d DPI ] [-l level ] [[-M | -X]  -H homedir -T tempdir ] [-I includefile ] [
       -W windowmanager ] [ -w windowsize ] [[-i file ]...] [ -t type ] cmd
            * TABs *
    [-C] [-c] [-s] [-l level ] [-X] [ -t type ] [cmd || -S ]
    [ -d DPI ] [ -W windowmanager ] [ -w windowsize ]
    [-I includefile ] [[-i file ]...]
    [[-M | -X]  -H homedir -T tempdir ]
   */
  tabWidget = new QTabWidget(this);
  tabWidget->setContentsMargins(1,1,1,1);
  tabWidget->setIconSize(QSize(32,32));
  tabWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  w1 = new CommonSet(this);
  QIcon i1 = QIcon::fromTheme("preferences-other");
  QString n1 = QString("Common");
  w2 = new WindowSet(this);
  QIcon i2 = QIcon::fromTheme("preferences-desktop");
  QString n2 = QString("Window");
  w3 = new IncludeSet(this);
  QIcon i3 = QIcon::fromTheme("document-export");
  QString n3 = QString("Include files");
  w4 = new DirectorySet(this);
  QIcon i4 = QIcon::fromTheme("folder-sync");
  QString n4 = QString("Work directories");
  tabWidget->addTab(w1, i1, n1);
  tabWidget->addTab(w2, i2, n2);
  tabWidget->addTab(w3, i3, n3);
  tabWidget->addTab(w4, i4, n4);
  connect(w1->guiApp, SIGNAL(stateChanged(int)),this, SLOT(windowSetsEnable(int)));
  connect(w1->guiApp, SIGNAL(stateChanged(int)),w4, SLOT(setGuiCheckState(int)));
  connect(w1->nameEdit, SIGNAL(textChanged(QString)), this, SLOT(set_Title_Name(QString)));
  connect(w4, SIGNAL(guiStateChanged(bool)), w1->guiApp, SLOT(setChecked(bool)));
  connect(w1, SIGNAL(securityLevelState(bool)), this, SLOT(changeSecLevelState(bool)));
}
void SettingsDialog::initButtons()
{
  ok = new QPushButton(QIcon::fromTheme("dialog-ok"),"Ok");
  cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"),"Cancel");
  buttonsLayout = new QHBoxLayout(this);
  buttonsLayout->addWidget(ok);
  buttonsLayout->addWidget(cancel);
  buttons = new QWidget(this);
  buttons->setLayout(buttonsLayout);
  connect(ok, SIGNAL(clicked()), this, SLOT(saveJob()));
  connect(cancel, SIGNAL(clicked()), this, SLOT(cancelJob()));
}
QString SettingsDialog::includesFileName(QString s) const
{
  QStringList fullPath = settings.fileName().split(QDir::separator());
  fullPath.removeLast();
  fullPath.append(QString("%1.included").arg(s));
  return fullPath.join(QDir::separator());
}
void SettingsDialog::setJobItem(QListWidgetItem *i)
{
  item = i;
  name = item->text();
  previousName = name;
  setWindowTitle(name);
  if (name!=QString("<noname>"))
    {
      w1->set_Job_Name(name);
      w3->set_FileName(includesFileName(name));
      initParameters();
    };
}
void SettingsDialog::saveJob()
{
  name = w1->get_Job_Name();
  w3->set_FileName(includesFileName(name));
  QStringList groups = settings.childGroups();
  if      ( w1->session->isChecked() &&
            (
              w4->selinuxLabel->text().isEmpty() || !w4->securityLevel->isChecked() ||
              w4->tempDir->text().isEmpty() || w4->homeDir->text().isEmpty()
            )
          )
    {
      QMessageBox::information(this, QString("Info"),
        QString("You must to specify\na Security Level,\nHOME and TEMP directories."));
      return;
    }
  else if ( w1->session->isChecked() &&
            !(
              w4->selinuxLabel->text().isEmpty() || !w4->securityLevel->isChecked() ||
              w4->tempDir->text().isEmpty() || w4->homeDir->text().isEmpty()
            )
          )
    {
      QMessageBox::information(this, QString("Info"),
        QString("In this case a TempDir and a HomeDir\nmust to be used together."));
      make_SpecifiedDirectories();
      return;
    }
  else if ( !w1->session->isChecked() &&
             w4->securityLevel->isChecked() &&
            !w4->selinuxLabel->text().isEmpty() &&
            ( w4->tempDir->text().isEmpty() ||
              w4->homeDir->text().isEmpty()
            )
          )
    {
      QMessageBox::information(this, QString("Info"),
        QString("In this case a TempDir and a HomeDir\nmust to be used together."));
      make_SpecifiedDirectories();
      return;
    };
  if ( w4->securityLevel->isChecked() && !w4->selinuxLabel->text().isEmpty() )
    {
      if ( !exist_Directory(w4->tempDir) ) return;
      if ( !exist_Directory(w4->homeDir) ) return;
    };
  if ( name.isEmpty() ) QMessageBox::information(this, QString("Info"), QString("JobName is empty."));
  else if ( groups.contains(name) && !newbe && name==previousName )
    {
      saveParameters();
      close();
    }
  else if ( groups.contains(name) && newbe )
    {
      QMessageBox::information(this, QString("Info"), QString("Same JobName is exist."));
    }
  else if ( groups.contains(name) && !newbe && name!=previousName )
    {
      QMessageBox::information(this, QString("Info"), QString("Same JobName is exist."));
    }
  else if ( !groups.contains(name) && newbe )
    {
      saveParameters();
      close();
    }
  else if ( !groups.contains(name) && !newbe )
    {
      settings.remove(previousName);
      QFile f;
      f.setFileName(includesFileName(previousName));
      f.remove();
      //qDebug()<<previousName<<"included deleted";
      saveParameters();
      close();
    };
}
void SettingsDialog::cancelJob()
{
  //qDebug()<<"cancel"<<name<< newbe;
  if ( newbe )
    {
      settings.remove(name);
      emit creatingJobCancelled();
    };
  close();
}
void SettingsDialog::initParameters()
{
  int c;
  QStringList groups = settings.childGroups();
  newbe = !groups.contains(name);
  //qDebug()<<"init"<<name<< newbe;
  settings.beginGroup(name);
  w1->autoRun->setChecked(settings.value("AutoRun", QVariant()).toBool() );
  w1->cgroups->setChecked( settings.value("CGroups", QVariant()).toBool() );
  w1->guiApp->setChecked( settings.value("GuiApp", QVariant()).toBool() );
  w1->runInTerm->setChecked( settings.value("RunInTerm", QVariant()).toBool() );
  w1->defaultTerminal->setChecked( settings.value("XDG-Utility-Term", QVariant()).toBool() );
  w1->customTerminal->setChecked( settings.value("CustomTerm", QVariant()).toBool() );
  w1->termCommand->setText( settings.value("TermCommand", QVariant()).toString() );
  w1->capabilities->setChecked( settings.value("Capabilities", QVariant()).toBool() );
  w1->shred->setChecked( settings.value("Shred", QVariant()).toBool() );
  c = w1->sandboxType->findText( settings.value("SType", QVariant()).toString() );
  w1->sandboxType->setCurrentIndex(c);
  w1->command->setText( settings.value("Command", QVariant()).toString() );
  w1->checkTimeout->setValue( settings.value("TimeOut", QVariant(TIMEOUT)).toInt() );
  w2->WM->setText( settings.value("WM", QVariant()).toString() );
  w2->DPI->setValue( settings.value("DPI", QVariant()).toInt() );
  w2->windowHeight->setValue( settings.value("wHeight", QVariant()).toInt() );
  w2->windowWidth->setValue( settings.value("wWidth", QVariant()).toInt() );
  w3->setIncludesList( settings.value("Includes", QVariant()).toString() );
  w4->mountDirs->setChecked( settings.value("Mount", QVariant()).toBool() );
  w4->securityLevel->setChecked( settings.value("SLevel", QVariant()).toBool() );
  w4->selinuxLabel->setText( settings.value("SELinuxLabel", QVariant()).toString() );
  w4->tempDir->setText( settings.value("TempDir", QVariant()).toString() );
  w4->homeDir->setText( settings.value("HomeDir", QVariant()).toString() );
  // it last settings because it define the directory_settings
  w1->execute->setChecked( settings.value("Execute", QVariant()).toBool() );
  w1->session->setChecked( settings.value("Session", QVariant()).toBool() );
  settings.endGroup();
}
void SettingsDialog::saveParameters()
{
  settings.beginGroup(name);
  settings.setValue("AutoRun", QVariant(w1->autoRun->isChecked()));
  settings.setValue("CGroups", QVariant(w1->cgroups->isChecked()));
  settings.setValue("GuiApp", QVariant(w1->guiApp->isChecked()));
  settings.setValue("RunInTerm", QVariant(w1->runInTerm->isChecked()));
  settings.setValue("XDG-Utility-Term", QVariant(w1->defaultTerminal->isChecked()));
  settings.setValue("CustomTerm", QVariant(w1->customTerminal->isChecked()));
  settings.setValue("TermCommand", QVariant(w1->termCommand->text()));
  settings.setValue("Capabilities", QVariant(w1->capabilities->isChecked()));
  settings.setValue("Shred", QVariant(w1->shred->isChecked()));
  settings.setValue("SType", QVariant(w1->sandboxType->currentText()));
  settings.setValue("Execute", QVariant(w1->execute->isChecked()));
  settings.setValue("Session", QVariant(w1->session->isChecked()));
  settings.setValue("Command", QVariant(w1->command->text()));
  settings.setValue("TimeOut", QVariant(w1->checkTimeout->value()));
  settings.setValue("DPI", QVariant(w2->DPI->value()));
  settings.setValue("WM", QVariant(w2->WM->text()));
  settings.setValue("wHeight", QVariant(w2->windowHeight->value()));
  settings.setValue("wWidth", QVariant(w2->windowWidth->value()));
  settings.setValue("Includes", QVariant(w3->get_FileName()));
  settings.setValue("SLevel", QVariant(w4->securityLevel->isChecked()));
  settings.setValue("SELinuxLabel", QVariant(w4->selinuxLabel->text()));
  settings.setValue("Mount", QVariant(w4->mountDirs->isChecked()));
  settings.setValue("TempDir", QVariant(w4->tempDir->text()));
  settings.setValue("HomeDir", QVariant(w4->homeDir->text()));
  settings.endGroup();
  item->setText(name);
  QString tT = item->toolTip();
  tT.append("\n-- EDITED --");
  item->setToolTip(tT);
  if ( item->data(Qt::UserRole).toMap().value("isRunning").toBool() )
    QMessageBox::information(this, "Info", "New settings apply\nat next job start.");
}
void SettingsDialog::closeEvent(QCloseEvent *ev)
{
  settings.setValue("SetDlgGeometry", saveGeometry());
  settings.sync();
  killTimer(timerId);
  ev->accept();
}
void SettingsDialog::windowSetsEnable(int i)
{
  Qt::CheckState _state = (i) ? Qt::Checked : Qt::Unchecked;
  w2->setEnabled(_state==Qt::Checked);
  int current_policy = w1->sandboxType->currentIndex();
  if ( _state==Qt::Checked )
    {
      if ( current_policy < 2 )
        {
          w1->sandboxType->setCurrentIndex(2);
        };
    }
  else
    {
      int c;
      settings.beginGroup(name);
      c = w1->sandboxType->findText( settings.value("SType", QVariant()).toString() );
      settings.endGroup();
      w1->sandboxType->setCurrentIndex(c);
    };
}
void SettingsDialog::set_Title_Name(QString s)
{
  setWindowTitle(s);
}
void SettingsDialog::timerEvent(QTimerEvent *event)
 {
   int _timerId = event->timerId();
   if ( _timerId && timerId==_timerId )
     {
       QString runApp;
       QStringList cmd;
       QStringList commandString;
       commandString = w1->termCommand->text().split(" ");
       commandString.removeAll("");

       if ( !w1->runInTerm->isChecked() )
         {
           runApp = QString("/usr/bin/sandbox");
           cmd = _commandBuild();
         }
       else
         {
           QStringList _cmd = _commandBuild();
           _cmd.prepend("/usr/bin/sandbox");
           if ( !w1->customTerminal->isChecked() )
             {
               cmd.append(_cmd.join(" "));
               runApp = QString("/usr/bin/xdg-terminal");
             }
           else
             {
               cmd = _cmd;
               while ( commandString.count() > 1 )
                 {
                   cmd.prepend( commandString.takeLast() );
                 };
               if ( !commandString.isEmpty() && !commandString.first().isEmpty() )
                 {
                   runApp = commandString.first();
                 }
               else
                 {
                   cmd.prepend("-e");
                   runApp = QString("xterm");
                 };
             };
         };
       //qDebug()<<runApp<<cmd.join(" ")<<name;
       fullCommandWdg->setText(QString("%1 %2").arg(runApp).arg(cmd.join(" ")));
     };
 }
QStringList SettingsDialog::_commandBuild()
{
  commandLine->clear();

  if ( w1->capabilities->isChecked() ) commandLine->appendCapabilities();
  if ( w1->cgroups->isChecked() ) commandLine->appendCGroups();
  if ( w1->shred->isChecked() ) commandLine->appendShred();
  if ( w1->guiApp->isChecked() && w2->DPI->value() ) commandLine->appendDPI( w2->DPI->value() );
  QString str;
  str = w4->selinuxLabel->text();
  if ( !str.isEmpty() && w4->securityLevel->isChecked() )
    commandLine->appendSecurityLevel( str );
  if ( w4->mountDirs->isChecked() ) commandLine->appendMountDirs();
  if ( w1->guiApp->isChecked() ) commandLine->appendGuiApp();
  str = w4->homeDir->text();
  if ( ( w1->guiApp->isChecked() || w4->mountDirs->isChecked() ) && !str.isEmpty() )
    commandLine->appendHomeDir( str );
  str = w4->tempDir->text();
  if ( ( w1->guiApp->isChecked() || w4->mountDirs->isChecked() ) && !str.isEmpty() )
    commandLine->appendTempDir( str );
  str = w3->get_FileName();
  if ( !str.isEmpty() ) commandLine->appendIncludes( str );
  if ( w1->guiApp->isChecked() )
    {
      str = w2->WM->text();
      if ( !str.isEmpty() ) commandLine->appendWM( str );
      if ( w2->windowWidth->value() && w2->windowHeight->value() )
          commandLine->appendWindowSize( w2->windowWidth->value(), w2->windowHeight->value() );
    };
  str = w1->sandboxType->currentText();
  if ( !str.isEmpty() ) commandLine->appendSandboxType( str );
  str = w1->command->text();
  if      ( w1->session->isChecked() ) commandLine->appendSession();
  else if ( w1->execute->isChecked() ) commandLine->appendCommand( str );

  return commandLine->getList();
}
void SettingsDialog::changeSecLevelState(bool b)
{
  w4->sessionUsed = b;
  if (b) w4->setSELinuxLabelState(b);
  else
    {
      settings.beginGroup(name);
      w4->securityLevel->setChecked( settings.value("SLevel", QVariant()).toBool() );
      settings.endGroup();
    };
}
bool SettingsDialog::make_Directory(QString s, QString Dir)
{
  QString dirPath;
  QDir d;
  QString q = QString(" not specified.\nCreate temporary directory?");
  QString l = QString("can`t marked by label\n");
  QString p = QString("cleaning not completed.\nTo continue anyway?");
  if ( s.isEmpty() )
    {
      int answer = QMessageBox::question(this, "Settings", QString("%1 %2").arg(Dir).arg(q),
                   QMessageBox::Yes, QMessageBox::No);
      if ( answer == QMessageBox::Yes )
        {
          QString suff;
          QString user = QString("HOME");
          QStringList env = QProcess::systemEnvironment();
          QList<QString>::const_iterator i;
          for ( i=env.constBegin(); i<env.constEnd(); i++ )
            {
              QString item = *i;
              if ( (item).startsWith("USER=") )
                {
                  user = item.split("=")[1];
                  break;
                };
            };
          suff = ( Dir == "TempDir" ) ? "tmp" : user;
          dirPath = QString("/tmp/.sandbox-%1-XXXXXX").arg(suff);
        }
      else return false;
    }
  else dirPath = s;

  if ( !clean_Directory(dirPath) )
    {
      int answer = QMessageBox::question(this, "Clean Directory",
                   QString("\"%1\" %2").arg(dirPath).arg(p), QMessageBox::Yes, QMessageBox::No);
      if ( answer == QMessageBox::No ) return false;
    };
  if ( !d.mkpath(dirPath) )
    {
      QMessageBox::information(this, QString("Info"), QString("Path\n\"%1\"\nis failed.").arg(dirPath));
      return false;
    }
  else if ( !set_SpecifiedLabel(dirPath) )
    {
      QMessageBox::information(this, QString("Info"),
      QString("\"%1\"\n%2\n%3").arg(dirPath).arg(l).arg(w4->selinuxLabel->text()));
      return false;
    };
  QLineEdit *obj = ( Dir == "TempDir" ) ? w4->tempDir : w4->homeDir;
  obj->setText(dirPath);
  return true;
}
bool SettingsDialog::make_SpecifiedDirectories()
{
  /* set specify label to specified directories
   * if directories don`t exists, then create them
   */
  if ( !exist_Directory(w4->tempDir) )
    if ( !make_Directory(w4->tempDir->text(), "TempDir") ) return false;
  if ( !exist_Directory(w4->homeDir) )
    if ( !make_Directory(w4->homeDir->text(), "HomeDir") ) return false;
  return true;
}
bool SettingsDialog::set_SpecifiedLabel(QString s)
{
  /*
   * chcon -t sandbox_file_t -l s0:c123,c456 /home/Flash/Example_HOME
   */
  QStringList args = QStringList()<<"-t"<<"sandbox_file_t"<<"-l"<< w4->selinuxLabel->text()<< s;
  int exitCode = QProcess::execute(QString("chcon"), args);
  if ( exitCode != 0 ) return false;
  return true;
}
bool SettingsDialog::clean_Directory(QString dir)
{
  bool result = true;
  QDir d;
  QStringList redundant = QStringList()<<"."<<"..";
  d.setPath(dir);
  if ( d.exists() )
    {
      QList<QFileInfo> entry = d.entryInfoList();
      if ( !entry.isEmpty() )
        {
          bool _result = true;
          QList<QFileInfo>::const_iterator i;
          for ( i=entry.constBegin(); i<entry.constEnd(); i++ )
            {
              QFileInfo item = *i;
              item.makeAbsolute();
              if ( item.isDir() )
                {
                  QString path;
                  QString fileName;
                  fileName = item.fileName();
                  path.append(item.dir().path());
                  if ( !redundant.contains( fileName ) )
                    {
                      path.append(QDir::separator());
                      path.append(fileName);
                    };
                  if ( !dir.contains( path ) ) _result = _result && clean_Directory(path) && d.rmdir(path);
                }
              else if ( item.isFile() )
                {
                  _result = _result && QFile::remove(item.canonicalFilePath());
                };
            };
          result = result && _result;
        };
    };
  return result;
}
bool SettingsDialog::exist_Directory(QLineEdit *obj)
{
  QDir d;
  QString dir = obj->text();
  d.setPath(dir);
  if ( !d.exists() )
    {
      QMessageBox::information(this, QString("Info"), QString("Directory\n\"%1\"\n is not exist.").arg(dir));
      obj->clear();
      return false;
    };
  return true;
}
