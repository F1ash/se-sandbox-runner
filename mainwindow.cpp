#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  restoreGeometry(settings.value("Geometry").toByteArray());
  setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
  setWindowTitle("SELinux Sandbox Runner");
  setMinimumSize(100, 100);
  QStringList searchThemePath;
  searchThemePath.append(QIcon::themeSearchPaths());
  searchThemePath.insert(0, "/usr/share/sandbox-runner-data/");
  QIcon::setThemeSearchPaths(searchThemePath);
  QIcon::setThemeName("icons");
  setWindowIcon(QIcon::fromTheme("applications-safety-selinux"));
  initTrayIcon();
  initJobWidget();
  initToolBar();
  wait_thread = new Wait(this);
  connect(wait_thread, SIGNAL(finished()), this, SLOT(closeEvent()));
}
MainWindow::~MainWindow()
{
  disconnect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, \
                    SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
  disconnect(trayIcon->hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  disconnect(trayIcon->closeAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  disconnect(jobWidget, SIGNAL(removeJob(QString&)), this, SLOT(removeJobItem(QString&)));
  disconnect(toolBar->_hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  disconnect(toolBar->_createAction, SIGNAL(triggered()), this, SLOT(createNewJobItem()));
  disconnect(toolBar->_editAction, SIGNAL(triggered()), this, SLOT(editCurrentJobItem()));
  disconnect(toolBar->_deleteAction, SIGNAL(triggered()), this, SLOT(deleteCurrentJobItem()));
  disconnect(toolBar->_runAction, SIGNAL(triggered()), this, SLOT(runCurrentJob()));
  disconnect(toolBar->_stopAction, SIGNAL(triggered()), this, SLOT(stopCurrentJob()));
  disconnect(toolBar->_stopAllAction, SIGNAL(triggered()), this, SLOT(stopAllJob()));
  disconnect(toolBar->_exitAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  disconnect(wait_thread, SIGNAL(finished()), this, SLOT(closeEvent()));

  delete wait_thread;
  wait_thread = 0;

  delete trayIcon;
  trayIcon = 0;
  delete jobWidget;
  jobWidget = 0;
  delete toolBar;
  toolBar = 0;
}
void MainWindow::closeEvent(QCloseEvent *ev)
{
  settings.setValue("Geometry", saveGeometry());
  settings.setValue("ToolBarArea", toolBarArea(toolBar));
  settings.sync();
  if ( !this->isVisible() ) changeVisibility();
  if ( runningJobsExist() && !wait_thread->isRunning() && !wait_thread->isFinished() )
    {
      QString q;
      q.append("Running Jobs are exist.\nKill it at exit?");
      int answer = QMessageBox::question(this, "Action", q, QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
      if ( answer != QMessageBox::No )
        {
          if ( answer == QMessageBox::Yes )
            {
              jobWidget->setEnabled(false);
              toolBar->setEnabled(false);
              wait_thread->setWdgReference(jobWidget);
              wait_thread->start();
            };
          ev->ignore();
          return;
        };
    };
  if ( !wait_thread->isRunning() || wait_thread->isFinished() )
    ev->accept();
  else ev->ignore();
}
void MainWindow::closeEvent()
{
  this->close();
}
void MainWindow::initTrayIcon()
{
  trayIcon = new TrayIcon(this);
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, \
  SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
  connect(trayIcon->hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  connect(trayIcon->closeAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
}
void MainWindow::changeVisibility()
{
  if (this->isVisible())
    {
      this->hide();
      trayIcon->hideAction->setText ( QString("Up") );
      trayIcon->hideAction->setIcon ( QIcon::fromTheme("up") );
    }
  else
    {
      this->show();
      trayIcon->hideAction->setText ( QString("Down") );
      trayIcon->hideAction->setIcon ( QIcon::fromTheme("down") );
    };
}
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason r)
{
  if (r==QSystemTrayIcon::Trigger) changeVisibility();
}
void MainWindow::initJobWidget()
{
  jobWidget= new JobList(this);
  QStringList groups = settings.childGroups();
  QList<QString>::const_iterator i;
  for (i=groups.constBegin(); i!=groups.constEnd(); ++i)
    {
      QString s = (*i);
      jobWidget->addJobItem(s);
    };
  setCentralWidget(jobWidget);
  connect(jobWidget, SIGNAL(removeJob(QString&)), this, SLOT(removeJobItem(QString&)));
}
void MainWindow::initToolBar()
{
  toolBar = new ToolBar(this);
  connect(toolBar->_hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  connect(toolBar->_createAction, SIGNAL(triggered()), this, SLOT(createNewJobItem()));
  connect(toolBar->_editAction, SIGNAL(triggered()), this, SLOT(editCurrentJobItem()));
  connect(toolBar->_deleteAction, SIGNAL(triggered()), this, SLOT(deleteCurrentJobItem()));
  connect(toolBar->_runAction, SIGNAL(triggered()), this, SLOT(runCurrentJob()));
  connect(toolBar->_stopAction, SIGNAL(triggered()), this, SLOT(stopCurrentJob()));
  connect(toolBar->_stopAllAction, SIGNAL(triggered()), this, SLOT(stopAllJob()));
  connect(toolBar->_exitAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  int area_int = settings.value("ToolBarArea", 4).toInt();
  this->addToolBar(toolBar->get_ToolBarArea(area_int), toolBar);
}
void MainWindow::editCurrentJobItem()
{
  jobWidget->editItemAction();
}
void MainWindow::createNewJobItem()
{
  QString s = QString("<noname>");
  jobWidget->addJobItem(s);
}
void MainWindow::deleteCurrentJobItem()
{
  jobWidget->deleteCurrentJobItem();
}
void MainWindow::removeJobItem(QString &job)
{
  settings.remove(job);
  QStringList fullPath = settings.fileName().split(QDir::separator());
  fullPath.removeLast();
  fullPath.append(QString("%1.included").arg(job));
  QString _fn = fullPath.join(QDir::separator());
  QFile::remove(_fn);
  //qDebug()<<job<<"job deleted";
}
void MainWindow::runCurrentJob()
{
  QListWidgetItem *_item = jobWidget->currentItem();
  if (_item)
    {
      jobWidget->runJob(_item);
    };
}
void MainWindow::stopCurrentJob()
{
  QListWidgetItem *_item = jobWidget->currentItem();
  if (_item)
    {
      jobWidget->stopJob(_item);
    };
}
void MainWindow::stopAllJob()
{
  int count = jobWidget->count();
  for (int i = 0; i< count; i++) stopJob(i);
}
void MainWindow::stopJob(int i)
{
  //qDebug()<<i<<" item to stop";
  jobWidget->stopJob(jobWidget->item(i));
}
bool MainWindow::runningJobsExist()
{
  bool result = false;
  for (int i=0; i<jobWidget->count(); i++)
    {
      //qDebug()<<jobWidget->item(i)->text()<< jobWidget->item(i)->data(Qt::UserRole).toMap().value("isRunning").toBool();
      if ( jobWidget->item(i)->data(Qt::UserRole).toMap().value("isRunning").toBool() ||
           !jobWidget->item(i)->data(Qt::UserRole).toMap().value("availability").toBool() )
        {
          result = true;
          break;
        };
    };
  return result;
}
