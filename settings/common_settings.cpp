#include "common_settings.h"

/* [-C] [-c] [-s] [-X] [ -t type ] [cmd || -S ] */

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
    gridLayout->addWidget(sandboxType, 5, 0);
    gridLayout->addWidget(timeoutLabel, 4, 2);
    gridLayout->addWidget(checkTimeout, 5, 2);
    gridLayout->addWidget(execute, 6, 0);
    gridLayout->addWidget(session, 6, 2);
    gridLayout->addWidget(commonWdg, 8, 0, 9, 5);

    setLayout(gridLayout);
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
    sandboxType->addItem("Ports required for Web","sandbox_web_t");
    sandboxType->addItem("All network ports", "sandbox_net_t");
    connect(sandboxType, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTypeToolTip(QString)));
}
void CommonSet::setTypeToolTip(QString s)
{
    sandboxType->setToolTip(
        QString("Alternate sandbox type:\n%1\n(%2)")
                .arg(s)
                .arg(sandboxType->itemData(sandboxType->currentIndex()).toString()));
}
void CommonSet::initRadioButtons()
{
    execute = new QRadioButton("Command", this);
    execute->setAutoExclusive(true);
    execute->setToolTip("Run  the <Command> application \nwithin a tightly \nconfined SELinux domain.");
    session = new QRadioButton("Session", this);
    session->setAutoExclusive(true);
    session->setToolTip("Run a full desktop session, \nRequires level, \nand home and tmpdir.");

    connect(execute, SIGNAL(toggled(bool)), this, SLOT(enableCommand(bool)));
    connect(session, SIGNAL(toggled(bool)), this, SLOT(enableSLevel(bool)));
}
void CommonSet::initTimeoutWidget()
{
    timeoutLabel = new QLabel("Timeout for check\nchild processes", this);
    checkTimeout = new QSpinBox(this);
    checkTimeout->setToolTip("If the task is not killed application,\
it means that the application\ndoes not have time to identify all child processes.\
Try to increase the timeout for.");
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
    cmdWidget->setEnabled(b);
    session->setChecked(!b);
}
void CommonSet::enableSLevel(bool b)
{
    execute->setChecked(!b);
    //enable SLevel, homeDir, tempDir
    emit sessionUsed(b);
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
