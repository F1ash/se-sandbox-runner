#ifndef TRAY_WIDGET_H
#define TRAY_WIDGET_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimerEvent>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public :
    explicit TrayIcon(QWidget *parent = NULL);

    QAction     *hideAction;
    QAction     *hideLogAction;
    QAction     *closeAction;

private :
    int          timerId;
    bool         _mark;
    QMenu       *trayIconMenu;
    QIcon        appIcon, warningIcon;

public slots:
    void         changeWarningState(bool);

private slots:
    void         timerEvent(QTimerEvent*);
};

#endif //TRAY_WIDGET_H
