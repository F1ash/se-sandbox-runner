#include "traywidget.h"

TrayIcon::TrayIcon(QWidget *parent)
    : QSystemTrayIcon(parent)
{
    _mark = false;
    timerId = 0;
    appIcon = QIcon::fromTheme("applications-safety-selinux");
    warningIcon = QIcon::fromTheme("dialog-warning");
    setIcon(appIcon);
    hideAction = new QAction(QString("Down"), this);
    hideAction->setIcon ( QIcon::fromTheme("down"));
    hideLogAction = new QAction(QString("Log Down"), this);
    hideLogAction->setIcon ( QIcon::fromTheme("down"));
    closeAction = new QAction(QString("Exit"), this);
    closeAction->setIcon ( QIcon::fromTheme("exit") );

    trayIconMenu = new QMenu(parent);
    trayIconMenu->addAction(hideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(hideLogAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(closeAction);

    setToolTip(QString("SELinux SandBox Runner"));
    setContextMenu(trayIconMenu);
    show();
}

void TrayIcon::changeWarningState(bool state)
{
    if ( state ) {
        if ( timerId==0 ) timerId = startTimer(1000);
    } else {
        if ( timerId>0 ) {
            killTimer(timerId);
            timerId = 0;
        };
        setIcon(appIcon);
    };
}

void TrayIcon::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() ) {
        QIcon icon = (_mark)? appIcon : warningIcon;
        setIcon(icon);
        _mark=!_mark;
    };
}
