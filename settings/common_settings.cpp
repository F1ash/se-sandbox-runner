#include "common_settings.h"

/* [-C] [-c] [-s] [-l level ] [-X] [ -t type ] [cmd || -S ] */

CommonSet::CommonSet(QWidget *parent) :
    QWidget(parent)
{
  setContentsMargins(1,1,1,1);
  gridLayout = new QGridLayout();
  gridLayout->setAlignment(Qt::AlignRight);
  cgroups = new QCheckBox("CGroups", this);
  cgroups->setToolTip("Use control groups to control this copy of sandbox. \nSpecify parameters in /etc/sysconfig/sandbox.");
  nameEdit = new QLineEdit(this);
  nameEdit->setPlaceholderText("Enter Job Name");
  autoRun = new QCheckBox("AutoRun", this);
  autoRun->setToolTip("Run Job at startup.");
  guiApp = new QCheckBox("GUI App", this);
  guiApp->setToolTip("Create an X based Sandbox for gui apps, \ntemporary files for $HOME and /tmp, \nsecondary Xserver");
  shred = new QCheckBox("Shred", this);
  shred->setToolTip("Shred temporary files created \nin $HOME and /tmp,\nbefore deleting.");
  capabilities = new QCheckBox("Capabilities", this);
  capabilities->setToolTip("Use capabilities \nwithin the sandbox.");
  initTypeBox();
  initSLevelBox();
  initRadioButtons();
  initCmdWidget();
  gridLayout->addWidget(nameEdit, 0, 0);
  gridLayout->addWidget(autoRun, 0, 2);
  gridLayout->addWidget(cgroups, 2, 0);
  gridLayout->addWidget(guiApp, 2, 2);
  gridLayout->addWidget(shred, 3, 0);
  gridLayout->addWidget(capabilities, 3, 2);
  gridLayout->addWidget(typeLabel, 4, 0);
  gridLayout->addWidget(secLabel, 4, 2);
  gridLayout->addWidget(sandboxType, 5, 0);
  gridLayout->addWidget(securityLayer, 5, 2);
  gridLayout->addWidget(execute, 6, 0);
  gridLayout->addWidget(session, 6, 2);
  gridLayout->addWidget(cmdWidget, 7, 0, 8, 5);

  setLayout(gridLayout);
  setFocusProxy(nameEdit);
}
CommonSet::~CommonSet()
{
  delete shred;
  shred = 0;
  delete autoRun;
  autoRun = 0;
  delete capabilities;
  capabilities = 0;
  delete cgroups;
  cgroups = 0;
  delete nameEdit;
  nameEdit = 0;
  delete sandboxType;
  sandboxType = 0;
  delete typeLabel;
  typeLabel = 0;
  delete secLabel;
  secLabel = 0;
  delete execute;
  execute = 0;
  delete session;
  session = 0;
  delete selectFile;
  selectFile = 0;
  delete command;
  command = 0;
  delete cmdLayout;
  cmdLayout = 0;
  delete cmdWidget;
  cmdWidget = 0;
  delete securityLayer;
  securityLayer = 0;
  delete gridLayout;
  gridLayout = 0;
}
void CommonSet::initTypeBox()
{
  typeLabel = new QLabel("Sandbox Type", this);
  sandboxType = new QComboBox(this);
  sandboxType->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  sandboxType->setToolTip("Alternate sandbox type");
  sandboxType->addItem("Default", "sandbox_t");
  sandboxType->addItem("No Network Access", "sandbox_min_t");
  sandboxType->addItem("Printer Ports", "sandbox_x_t");
  sandboxType->addItem("Ports required for web browsing","sandbox_web_t");
  sandboxType->addItem("All network ports", "sandbox_net_t");
  connect(sandboxType, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTypeToolTip(QString)));
}
void CommonSet::setTypeToolTip(QString s)
{
  sandboxType->setToolTip(QString("Alternate sandbox type:\n%1\n(%2)").arg(s).arg(sandboxType->itemData(sandboxType->currentIndex()).toString()));
}
void CommonSet::initSLevelBox()
{
  secLabel = new QLabel("Secuity Level", this);
  securityLayer = new QComboBox(this);
  securityLayer->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  securityLayer->setToolTip("MLS/MCS Security Level");
  securityLayer->addItem("MLS", "MLS");
  securityLayer->addItem("MCS", "MCS");
}
void CommonSet::initRadioButtons()
{
  execute = new QRadioButton("Command", this);
  execute->setToolTip("Run  the <Command> application \nwithin a tightly \nconfined SELinux domain.");
  session = new QRadioButton("Session", this);
  session->setToolTip("Run a full desktop session, \nRequires level, \nand home and tmpdir.");

  connect(execute, SIGNAL(toggled(bool)), this, SLOT(enableCommand(bool)));
  connect(session, SIGNAL(toggled(bool)), this, SLOT(enableSLevel(bool)));
}
void CommonSet::enableCommand(bool b)
{
  if (b)
    {
      securityLayer->setEnabled(!b);
      cmdWidget->setEnabled(b);
    }
  else
    {
      securityLayer->setEnabled(b);
      cmdWidget->setEnabled(!b);
    };
}
void CommonSet::enableSLevel(bool b)
{
  if (b)
    {
      securityLayer->setEnabled(b);
      cmdWidget->setEnabled(!b);
    }
  else
    {
      securityLayer->setEnabled(!b);
      cmdWidget->setEnabled(b);
    };
}
void CommonSet::initCmdWidget()
{
  cmdWidget = new QWidget(this);
  cmdLayout = new QHBoxLayout(this);
  command = new QLineEdit(this);
  command->setPlaceholderText("Enter command or path");
  selectFile = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
  selectFile->setToolTip("Search File");
  cmdLayout->addWidget(command);
  cmdLayout->addWidget(selectFile);
  cmdWidget->setLayout(cmdLayout);
  connect(selectFile, SIGNAL(clicked()), this, SLOT(setCommandPath()));
}
void CommonSet::setCommandPath()
{
  QString s = QFileDialog::getOpenFileName(this, "Command Path", "~" );
  command->setText(s);
}

QString CommonSet::get_Job_Name() const
{
  return nameEdit->text();
}
void CommonSet::set_Job_Name(QString &s)
{
  nameEdit->setText(s);
}
