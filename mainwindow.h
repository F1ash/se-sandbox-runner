#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QSettings>
#include <QCloseEvent>
#include <QMainWindow>
#include <QStringList>
#include "settings/settings_stuff.h"
#include "tray/traywidget.h"
#include "layout/listwidget.h"
#include "toolbar/toolbar.h"
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    SettingsStuff *settings;
    JobList  *jobWidget;

signals:

public slots:

private :
    TrayIcon *trayIcon;
    ToolBar *toolBar;

private slots:
    void closeEvent( QCloseEvent *event );
    void closeEvent();
    void changeVisibility();
    void initTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason r);
    void initJobWidget();
    void initToolBar();
    void editCurrentJobItem();
    void createNewJobItem();
    void restoreJobParameter(QString &, QString &, QVariant &);
    void deleteCurrentJobItem();
    void removeJobItem(QString &);
    void stopCurrentJob();
    void stopJob(int i);
    void stopAllJob();

};

#endif // MAINWINDOW_H
