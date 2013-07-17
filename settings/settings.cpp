#include "settings/settings.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
  setWindowTitle("<noname>::Settings");
  restoreGeometry(settings.value("SetDlgGeometry").toByteArray());
  setModal(false);
  setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  setContentsMargins(1,1,1,1);
  setWindowIcon(QIcon::fromTheme("preferences-system"));
  initTabWidget();
  initButtons();
  commonLayout = new QVBoxLayout(this);
  commonLayout->addWidget(tabWidget, 0, Qt::AlignHCenter);
  commonLayout->addWidget(buttons, 0, Qt::AlignHCenter);
  setLayout(commonLayout);
}
SettingsDialog::~SettingsDialog()
{
  disconnect(w1->guiApp, SIGNAL(stateChanged(int)),this, SLOT(windowSetsEnable(int)));
  disconnect(w1->guiApp, SIGNAL(stateChanged(int)),w4, SLOT(setGuiCheckState(int)));
  disconnect(w1->nameEdit, SIGNAL(textChanged(QString)), this, SLOT(set_Title_Name(QString)));
  disconnect(w4, SIGNAL(guiStateChanged(bool)), w1->guiApp, SLOT(setChecked(bool)));
  disconnect(w1->session, SIGNAL(toggled(bool)), w4, SLOT(changeDirsState(bool)));
  disconnect(ok, SIGNAL(clicked()), this, SLOT(saveJob()));
  disconnect(cancel, SIGNAL(clicked()), this, SLOT(cancelJob()));

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
  delete buttonsLayout;
  buttonsLayout = 0;
  delete buttons;
  buttons = 0;
  delete commonLayout;
  commonLayout = 0;
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
  connect(w1->session, SIGNAL(toggled(bool)), w4, SLOT(changeDirsState(bool)));
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
void SettingsDialog::setJobItem(QListWidgetItem *i)
{
  item = i;
  name = item->text();
  previousName = name;
  setWindowTitle(QString("%1::Settings").arg(name));
  if (name!=QString("<noname>"))
    {
      w1->set_Job_Name(name);
      w3->set_JobName(name);
      initParameters();
    };
}
void SettingsDialog::saveJob()
{
  name = w1->get_Job_Name();
  w3->set_JobName(name);
  qDebug()<<"ok"<<name;
  QStringList groups = settings.childGroups();
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
      QString _fn = QDir::homePath();
      _fn.append("/.config/se-sandbox-runner/");
      _fn.append(QString("%1.included").arg(previousName));
      QFile f;
      f.setFileName(_fn);
      f.remove();
      qDebug()<<previousName<<"included deleted";
      saveParameters();
      close();
    }
}
void SettingsDialog::cancelJob()
{
  qDebug()<<"cancel"<<name;
  close();
}
void SettingsDialog::initParameters()
{
  int c;
  QStringList groups = settings.childGroups();
  newbe = !groups.contains(name);
  settings.beginGroup(name);
  w1->autoRun->setChecked(settings.value("AutoRun", QVariant()).toBool() );
  w1->guiApp->setChecked( settings.value("GuiApp", QVariant()).toBool() );
  w1->cgroups->setChecked( settings.value("CGroups", QVariant()).toBool() );
  w1->capabilities->setChecked( settings.value("Capabilities", QVariant()).toBool() );
  w1->shred->setChecked( settings.value("Shred", QVariant()).toBool() );
  c = w1->securityLayer->findText( settings.value("SLeyer", QVariant()).toString() );
  w1->securityLayer->setCurrentIndex(c);
  c = w1->sandboxType->findText( settings.value("SType", QVariant()).toString() );
  w1->sandboxType->setCurrentIndex(c);
  w1->command->setText( settings.value("Command", QVariant()).toString() );
  w1->execute->setChecked( settings.value("Execute", QVariant()).toBool() );
  w1->session->setChecked( settings.value("Session", QVariant()).toBool() );
  w2->WM->setText( settings.value("WM", QVariant()).toString() );
  w2->DPI->setValue( settings.value("DPI", QVariant()).toInt() );
  w2->windowHeight->setValue( settings.value("wHeight", QVariant()).toInt() );
  w2->windowWidth->setValue( settings.value("wWidth", QVariant()).toInt() );
  w3->setIncludesList( settings.value("Includes", QVariant()).toString() );
  w4->mountDirs->setChecked( settings.value("Mount", QVariant()).toBool() );
  w4->tempDir->setText( settings.value("TempDir", QVariant()).toString() );
  w4->homeDir->setText( settings.value("HomeDir", QVariant()).toString() );
  settings.endGroup();
}
void SettingsDialog::saveParameters()
{
  settings.beginGroup(name);
  settings.setValue("AutoRun", QVariant(w1->autoRun->isChecked()));
  settings.setValue("GuiApp", QVariant(w1->guiApp->isChecked()));
  settings.setValue("CGroups", QVariant(w1->cgroups->isChecked()));
  settings.setValue("Capabilities", QVariant(w1->capabilities->isChecked()));
  settings.setValue("Shred", QVariant(w1->shred->isChecked()));
  settings.setValue("SLeyer", QVariant(w1->securityLayer->currentText()));
  settings.setValue("SType", QVariant(w1->sandboxType->currentText()));
  settings.setValue("Execute", QVariant(w1->execute->isChecked()));
  settings.setValue("Session", QVariant(w1->session->isChecked()));
  settings.setValue("Command", QVariant(w1->command->text()));
  settings.setValue("DPI", QVariant(w2->DPI->value()));
  settings.setValue("WM", QVariant(w2->WM->text()));
  settings.setValue("wHeight", QVariant(w2->windowHeight->value()));
  settings.setValue("wWidth", QVariant(w2->windowWidth->value()));
  settings.setValue("Includes", QVariant(w3->get_FileName()));
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
  ev->accept();
}
void SettingsDialog::windowSetsEnable(int i)
{
  w2->setEnabled(i==Qt::Checked);
}
void SettingsDialog::set_Title_Name(QString s)
{
  setWindowTitle(QString("%1::Settings").arg(s));
}
void SettingsDialog::setGuiCheckState(int i)
{
  w1->guiApp->setCheckState((i) ? Qt::Checked : Qt::Unchecked);
}
