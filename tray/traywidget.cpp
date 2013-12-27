#include "traywidget.h"

TrayIcon::TrayIcon(QWidget *parent = 0)
  : QSystemTrayIcon(parent)
{
  setIcon(QIcon("/usr/share/pixmaps/applications-safety-selinux.png"));
  hideAction = new QAction(QString("Down"), this);
  hideAction->setIcon ( QIcon::fromTheme("arrow-down"));
  closeAction = new QAction(QString("Exit"), this);
  closeAction->setIcon ( QIcon::fromTheme("application-exit") );

  trayIconMenu = new QMenu(parent);
  trayIconMenu->addAction(hideAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(closeAction);

  setToolTip(QString("SELinux SandBox Runner"));
  setContextMenu(trayIconMenu);
  show();
}
TrayIcon::~TrayIcon()
{
  trayIconMenu->clear();
  delete trayIconMenu;
  trayIconMenu = 0;
  delete hideAction;
  hideAction = 0;
  delete closeAction;
  closeAction = 0;
}
