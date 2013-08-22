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
  runInTerm = new QCheckBox("Run in terminal", this);
  runInTerm->setToolTip("Run in terminal\n(The process can be not controlled by application).");
  connect(runInTerm, SIGNAL(toggled(bool)), this, SLOT(showTerminalChioseWdg(bool)));
  shred = new QCheckBox("Shred", this);
  shred->setToolTip("Shred temporary files created \nin $HOME and /tmp,\nbefore deleting.");
  capabilities = new QCheckBox("Capabilities", this);
  capabilities->setToolTip("Use capabilities \nwithin the sandbox.");
  initTypeBox();
  initSLevelBox();
  initRadioButtons();
  initCmdWidget();
  initTimeoutWidget();
  gridLayout->addWidget(nameEdit, 0, 0);
  gridLayout->addWidget(autoRun, 1, 0);
  gridLayout->addWidget(cgroups, 1, 2);
  gridLayout->addWidget(guiApp, 2, 0);
  gridLayout->addWidget(runInTerm, 2, 2);
  gridLayout->addWidget(shred, 3, 0);
  gridLayout->addWidget(capabilities, 3, 2);
  gridLayout->addWidget(typeLabel, 4, 0);
  gridLayout->addWidget(secLabel, 4, 2);
  gridLayout->addWidget(sandboxType, 5, 0);
  gridLayout->addWidget(securityLayer, 5, 2);
  gridLayout->addWidget(timeoutLabel, 6, 0);
  gridLayout->addWidget(checkTimeout, 6, 2);
  gridLayout->addWidget(execute, 7, 0);
  gridLayout->addWidget(session, 7, 2);
  gridLayout->addWidget(commonWdg, 8, 0, 9, 5);

  setLayout(gridLayout);
}
CommonSet::~CommonSet()
{
  disconnect(sandboxType, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTypeToolTip(QString)));
  disconnect(execute, SIGNAL(toggled(bool)), this, SLOT(enableCommand(bool)));
  disconnect(session, SIGNAL(toggled(bool)), this, SLOT(enableSLevel(bool)));
  disconnect(selectFile, SIGNAL(clicked()), this, SLOT(setCommandPath()));
  disconnect(runInTerm, SIGNAL(toggled(bool)), this, SLOT(showTerminalChioseWdg(bool)));
  disconnect(customTerminal, SIGNAL(toggled(bool)), termCommand, SLOT(setVisible(bool)));

  delete guiApp;
  guiApp = 0;
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
  delete checkTimeout;
  checkTimeout = 0;
  delete timeoutLabel;
  timeoutLabel = 0;
  delete cmdLayout;
  cmdLayout = 0;
  delete cmdWidget;
  cmdWidget = 0;
  delete securityLayer;
  securityLayer = 0;
  delete runInTermLabel;
  runInTermLabel = 0;
  delete defaultTerminal;
  defaultTerminal = 0;
  delete customTerminal;
  customTerminal = 0;
  delete termCommand;
  termCommand = 0;
  delete termChoiseLayout;
  termChoiseLayout = 0;
  delete termChoiseWidget;
  termChoiseWidget = 0;
  delete commonCmdLayout;
  commonCmdLayout = 0;
  delete commonWdg;
  commonWdg = 0;
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
void CommonSet::initTimeoutWidget()
{
  timeoutLabel = new QLabel("Timeout for check child processes", this);
  checkTimeout = new QSpinBox(this);
  checkTimeout->setToolTip("If the task is not killed application,\nit means that the application\ndoes not have time to identify all child processes.\nTry to increase the timeout for.");
  checkTimeout->setMaximum(10000);
  checkTimeout->setMinimum(1);
  checkTimeout->setSingleStep(1);
  checkTimeout->setValue(TIMEOUT);
}
void CommonSet::initTermChoiseWdg()
{
  termChoiseWidget = new QWidget(this);
  termChoiseLayout = new QVBoxLayout();
  runInTermLabel = new QLabel("Run in Terminal:", this);
  defaultTerminal = new QRadioButton("use xdg-terminal util", this);
  customTerminal = new QRadioButton("use custom terminal command (Recommended)", this);
  termCommand = new QLineEdit(this);
  termCommand->setVisible(false);
  termCommand->setPlaceholderText("konsole --nofork -e");
  termChoiseLayout->addWidget(runInTermLabel);
  termChoiseLayout->addWidget(defaultTerminal);
  termChoiseLayout->addWidget(customTerminal);
  termChoiseLayout->addWidget(termCommand);
  termChoiseWidget->setLayout(termChoiseLayout);
  termChoiseWidget->setVisible(false);
  connect(customTerminal, SIGNAL(toggled(bool)), termCommand, SLOT(setVisible(bool)));
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
  commonWdg = new QWidget(this);
  cmdWidget = new QWidget(this);
  commonCmdLayout = new QVBoxLayout();
  cmdLayout = new QHBoxLayout();
  command = new QLineEdit(this);
  command->setPlaceholderText("Enter command or path");
  selectFile = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
  selectFile->setToolTip("Search File");
  cmdLayout->addWidget(command);
  cmdLayout->addWidget(selectFile);
  cmdWidget->setLayout(cmdLayout);
  connect(selectFile, SIGNAL(clicked()), this, SLOT(setCommandPath()));

  commonCmdLayout->addWidget(cmdWidget);
  initTermChoiseWdg();
  commonCmdLayout->addWidget(termChoiseWidget);
  commonWdg->setLayout(commonCmdLayout);
}
void CommonSet::setCommandPath()
{
  QString s = QFileDialog::getOpenFileName(this, "Command Path", "~" );
  command->setText(s);
}
void CommonSet::showTerminalChioseWdg(bool b)
{
  termChoiseWidget->setVisible(b);
}

QString CommonSet::get_Job_Name() const
{
  return nameEdit->text();
}
void CommonSet::set_Job_Name(QString &s)
{
  nameEdit->setText(s);
}
