#ifndef TRAY_WIDGET_H
#define TRAY_WIDGET_H

#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public :
    explicit TrayIcon(QWidget *parent);

    QAction     *hideAction;
    QAction     *closeAction;

private :
    QMenu       *trayIconMenu;
};

#endif //TRAY_WIDGET_H
