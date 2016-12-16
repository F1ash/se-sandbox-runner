#include "directory_settings.h"

/* [-l level ] [[-M | -X]    -H homedir -T tempdir ] */

DirectorySet::DirectorySet(QWidget *parent) :
    QWidget(parent)
{
    sessionUsed = false;
    setContentsMargins(1,1,1,1);
    commonLayout = new QGridLayout();
    mountDirs = new QCheckBox("Mount", this);
    mountDirs->setToolTip("Create a Sandbox with temporary files \nfor $HOME and /tmp.");
    guiApp = new QCheckBox("GUI App", this);
    guiApp->setToolTip("Create an X based Sandbox for gui apps, \
temporary files for $HOME and /tmp, \nsecondary Xserver");
    securityLevel = new QCheckBox("Set the Security Level", this);
    selinuxLabel = new QLineEdit(this);
    selinuxLabel->setPlaceholderText("s0:c111,c222");
    selinuxLabel->setEnabled(false);
    initTempDirWidget();
    initHomeDirWidget();
    commonLayout->addWidget(mountDirs, 0, 0);
    commonLayout->addWidget(guiApp, 0, 1);
    commonLayout->addWidget(securityLevel, 1, 0);
    commonLayout->addWidget(selinuxLabel, 1, 1);
    commonLayout->addWidget(tempLabel, 2, 0, 2, 2);
    commonLayout->addWidget(tempDirWdg, 3, 0, 5, 2);
    commonLayout->addWidget(homeLabel, 6, 0, 6, 2);
    commonLayout->addWidget(homeDirWdg, 7, 0, 8, 2);

    setLayout(commonLayout);
    connect(guiApp,    SIGNAL(clicked(bool)),
            this, SLOT(gui_StateChanged(bool)));
    connect(mountDirs, SIGNAL(toggled(bool)),
            this, SLOT(setWorkDirsState(bool)));
    connect(securityLevel, SIGNAL(clicked()),
            this, SLOT(check_SecLevelState()));
    connect(securityLevel, SIGNAL(toggled(bool)),
            this, SLOT(setSELinuxLabelState(bool)));
}

void DirectorySet::initTempDirWidget()
{
    tempLabel = new QLabel("Temporary Directory", this);
    tempDirWdg = new QWidget(this);
    tempWdgLayout = new QHBoxLayout(this);
    tempDir = new QLineEdit(this);
    tempDir->setPlaceholderText("Enter Temporary Directory");
    getTempDir = new QPushButton(QIcon::fromTheme("edit-find"),"", this);
    tempWdgLayout->addWidget(tempDir);
    tempWdgLayout->addWidget(getTempDir);
    connect(getTempDir, SIGNAL(clicked()),
            this, SLOT(setTempDir()));

    tempDirWdg->setLayout(tempWdgLayout);
    tempDirWdg->setEnabled(false);
}
void DirectorySet::initHomeDirWidget()
{
    homeLabel = new QLabel("Home Directory", this);
    homeDirWdg = new QWidget(this);
    homeWdgLayout = new QHBoxLayout(this);
    homeDir = new QLineEdit(this);
    homeDir->setPlaceholderText("Enter Home Directory");
    getHomeDir = new QPushButton(QIcon::fromTheme("edit-find"),"", this);
    homeWdgLayout->addWidget(homeDir);
    homeWdgLayout->addWidget(getHomeDir);
    connect(getHomeDir, SIGNAL(clicked()),
            this, SLOT(setHomeDir()));

    homeDirWdg->setLayout(homeWdgLayout);
    homeDirWdg->setEnabled(false);
}
void DirectorySet::setTempDir()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                "Get Temporary Directory",
                "~");
    tempDir->setText(dir);
}
void DirectorySet::setHomeDir()
{
    QString dir = QFileDialog::getExistingDirectory(
                this,
                "Get Home Directory",
                "~");
    homeDir->setText(dir);
}
QString DirectorySet::get_HomeDir() const
{
    return homeDir->text();
}
QString DirectorySet::get_TempDir() const
{
    return tempDir->text();
}
QString DirectorySet::get_Mount() const
{
    return (mountDirs->isChecked()) ? "-M" : "";
}
void DirectorySet::setWorkDirsState(bool b)
{
    // this directories strictly used when mountDir is enabled
    // or Session used
    if ( sessionUsed && !b ) {
        mountDirs->setCheckState(Qt::Checked);
        tempDirWdg->setEnabled(true);
        homeDirWdg->setEnabled(true);
        QMessageBox::information(
                    this,
                    "Info",
                    "\"Mount\" can't be changed\n\
because \"Session\" key is enabled.");
    } // workDirs can to be used in case running graphic application
      else if ( !b && !guiApp->isChecked() ) {
        tempDirWdg->setEnabled(b);
        homeDirWdg->setEnabled(b);
    } else if ( b ) {
        tempDirWdg->setEnabled(b);
        homeDirWdg->setEnabled(b);
    };
}
void DirectorySet::setGuiCheckState(int i)
{
    guiApp->setCheckState((i) ? Qt::Checked : Qt::Unchecked);
}
void DirectorySet::gui_StateChanged(bool b)
{
    emit guiStateChanged(b);
    // workDirs can to be used in case running graphic application
    if ( b ) {
        tempDirWdg->setEnabled(b);
        homeDirWdg->setEnabled(b);
    } else if ( !b && !mountDirs->isChecked() ) {
        tempDirWdg->setEnabled(b);
        homeDirWdg->setEnabled(b);
    };
}
void DirectorySet::setSELinuxLabelState(bool b)
{
    if ( sessionUsed && ( !securityLevel->isChecked()
         || !mountDirs->isChecked() ) ) {
        securityLevel->setCheckState(Qt::Checked);
        selinuxLabel->setEnabled(true);
        mountDirs->setChecked(true);
    } else selinuxLabel->setEnabled(b);
}
void DirectorySet::check_SecLevelState()
{
    if ( sessionUsed && securityLevel->isChecked() )
        QMessageBox::information(
            this,
            "Info",
            "\"Security Level\" can't be changed\n\
because \"Session\" key is enabled.");
}
