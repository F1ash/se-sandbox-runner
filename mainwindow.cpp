#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
  setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  restoreGeometry(settings.value("Geometry").toByteArray());
  setMinimumSize(100, 100);
  initTrayIcon();
  initJobWidget();
  initToolBar();
}
MainWindow::~MainWindow()
{

  disconnect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, \
                    SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
  disconnect(trayIcon->hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  //disconnect(trayIcon->reloadAction, SIGNAL(triggered()), this, SLOT(reloadAppAction()));
  disconnect(trayIcon->closeAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
  disconnect(jobWidget, SIGNAL(removeJob(QString &)), this, SLOT(removeJobItem(QString &)));
  disconnect(toolBar->_hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  disconnect(toolBar->_createAction, SIGNAL(triggered()), this, SLOT(createNewJobItem()));
  disconnect(toolBar->_editAction, SIGNAL(triggered()), this, SLOT(editCurrentJobItem()));
  disconnect(toolBar->_deleteAction, SIGNAL(triggered()), this, SLOT(deleteCurrentJobItem()));
  disconnect(toolBar->_stopAction, SIGNAL(triggered()), this, SLOT(stopCurrentJob()));
  disconnect(toolBar->_stopAllAction, SIGNAL(triggered()), this, SLOT(stopAllJob()));

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
  ev->accept();
}
void MainWindow::closeEvent()
{
  this->close();
}
void MainWindow::initTrayIcon()
{
  trayIcon = new TrayIcon(this);
  trayIcon->setIcon(QIcon("/home/Flash/se-sandbox-runner/icons/box_closed.png"));
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, \
  SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
  connect(trayIcon->hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  //connect(trayIcon->reloadAction, SIGNAL(triggered()), this, SLOT(reloadAppAction()));
  connect(trayIcon->closeAction, SIGNAL(triggered()), this, SLOT(closeEvent()));
}
void MainWindow::changeVisibility()
{
  if (this->isVisible())
    {
      this->hide();
      trayIcon->hideAction->setText (QString("Up"));
      trayIcon->hideAction->setIcon ( QIcon().fromTheme("arrow-up"));
    }
  else
    {
      this->show();
      trayIcon->hideAction->setText (QString("Down"));
      trayIcon->hideAction->setIcon ( QIcon().fromTheme("arrow-down"));
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
  connect(jobWidget, SIGNAL(removeJob(QString &)), this, SLOT(removeJobItem(QString &)));
}
void MainWindow::initToolBar()
{
  toolBar = new ToolBar(this);
  connect(toolBar->_hideAction, SIGNAL(triggered()), this, SLOT(changeVisibility()));
  connect(toolBar->_createAction, SIGNAL(triggered()), this, SLOT(createNewJobItem()));
  connect(toolBar->_editAction, SIGNAL(triggered()), this, SLOT(editCurrentJobItem()));
  connect(toolBar->_deleteAction, SIGNAL(triggered()), this, SLOT(deleteCurrentJobItem()));
  connect(toolBar->_stopAction, SIGNAL(triggered()), this, SLOT(stopCurrentJob()));
  connect(toolBar->_stopAllAction, SIGNAL(triggered()), this, SLOT(stopAllJob()));
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
  QString _fn = QDir::homePath();
  _fn.append("/.config/se-sandbox-runner/");
  _fn.append(QString("%1.included").arg(job));
  QFile f;
  f.setFileName(_fn);
  f.remove();
  f.deleteLater();
  qDebug()<<job<<"job deleted";
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
  qDebug()<<i<<" item to stop";
  jobWidget->stopJob(jobWidget->item(i));
}
