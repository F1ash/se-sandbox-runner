#include "directory_settings.h"

/* [[-M | -X]  -H homedir -T tempdir ] */

DirectorySet::DirectorySet(QWidget *parent) :
    QWidget(parent)
{
  sessionUsed = false;
  setContentsMargins(1,1,1,1);
  commonLayout = new QGridLayout();
  mountDirs = new QCheckBox("Mount", this);
  mountDirs->setToolTip("Create a Sandbox with temporary files \nfor $HOME and /tmp.");
  guiApp = new QCheckBox("GUI App", this);
  guiApp->setToolTip("Create an X based Sandbox for gui apps, \ntemporary files for $HOME and /tmp, \nsecondary Xserver");
  initTempDirWidget();
  initHomeDirWidget();
  commonLayout->addWidget(mountDirs, 0, 0);
  commonLayout->addWidget(guiApp, 0, 1);
  commonLayout->addWidget(tempLabel, 1, 0, 1, 2);
  commonLayout->addWidget(tempDirWdg, 2, 0, 4, 2);
  commonLayout->addWidget(homeLabel, 5, 0, 5, 2);
  commonLayout->addWidget(homeDirWdg, 6, 0, 7, 2);

  setLayout(commonLayout);
  connect(mountDirs, SIGNAL(clicked()), this, SLOT(setWorkDirsState()));
  connect(guiApp,    SIGNAL(clicked()), this, SLOT(setWorkDirsState()));
  connect(guiApp,    SIGNAL(clicked(bool)), this, SLOT(gui_StateChanged(bool)));
}
DirectorySet::~DirectorySet()
{
  delete mountDirs;
  mountDirs = 0;
  delete guiApp;
  guiApp = 0;
  delete tempLabel;
  tempLabel = 0;
  delete homeLabel;
  homeLabel = 0;
  delete tempDir;
  tempDir = 0;
  delete homeDir;
  homeDir = 0;
  delete getTempDir;
  getTempDir = 0;
  delete getHomeDir;
  getHomeDir = 0;
  delete tempWdgLayout;
  tempWdgLayout = 0;
  delete homeWdgLayout;
  homeWdgLayout = 0;
  delete tempDirWdg;
  tempDirWdg = 0;
  delete homeDirWdg;
  homeDirWdg = 0;
  delete commonLayout;
  commonLayout = 0;
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
  connect(getTempDir, SIGNAL(clicked()), this, SLOT(setTempDir()));

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
  connect(getHomeDir, SIGNAL(clicked()), this, SLOT(setHomeDir()));

  homeDirWdg->setLayout(homeWdgLayout);
  homeDirWdg->setEnabled(false);
}
void DirectorySet::setTempDir()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Get Temporary Directory", "~");
  tempDir->setText(dir);
}
void DirectorySet::setHomeDir()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Get Home Directory", "~");
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
void DirectorySet::setWorkDirsState()
{
  bool b;
  b = ( guiApp->isChecked() || mountDirs->isChecked() || sessionUsed );
  tempDirWdg->setEnabled(b);
  homeDirWdg->setEnabled(b);
}
void DirectorySet::setGuiCheckState(int i)
{
  guiApp->setCheckState((i) ? Qt::Checked : Qt::Unchecked);
  setWorkDirsState();
}
void DirectorySet::gui_StateChanged(bool b)
{
  emit guiStateChanged(b);
}
void DirectorySet::changeDirsState(bool b)
{
  sessionUsed = b;
  setWorkDirsState();
}
