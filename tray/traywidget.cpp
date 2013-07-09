#include "traywidget.h"

TrayIcon::TrayIcon(QWidget *parent = 0)
  : QSystemTrayIcon(parent)
{
  setIcon(QIcon::fromTheme("xorg"));
  hideAction = new QAction(QString().fromUtf8(""), this);
  reloadAction = new QAction(QString("Reload"), this);
  reloadAction->setIcon ( QIcon().fromTheme("view-refresh") );
  closeAction = new QAction(QString("Exit"), this);
  closeAction->setIcon ( QIcon().fromTheme("shutdown") );

  trayIconMenu = new QMenu(parent);
  trayIconMenu->addAction(hideAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(reloadAction);
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
