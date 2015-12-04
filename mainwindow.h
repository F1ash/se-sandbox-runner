#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QStringList>
#include "tray/traywidget.h"
#include "layout/listwidget.h"
#include "toolbar/toolbar.h"
#include "wait_thread.h"
#include "log/log_widget.h"
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    QSettings     settings;
    JobList      *jobWidget;
    LogWidget    *logWdg;

signals:

public slots:

private :
    TrayIcon     *trayIcon;
    ToolBar      *toolBar;
    Wait         *wait_thread = NULL;

private slots:
    void closeEvent(QCloseEvent*);
    void closeEvent();
    void changeVisibility();
    void changeLogVisibility();
    void initTrayIcon();
    void trayIconActivated(QSystemTrayIcon::ActivationReason);
    void initJobWidget();
    void initToolBar();
    void editCurrentJobItem();
    void createNewJobItem();
    void deleteCurrentJobItem();
    void removeJobItem(QString&);
    void runCurrentJob();
    void undockCurrentJob();
    void stopCurrentJob();
    void stopJob(int);
    void stopAllJob();
    bool runningJobsExist();
    void showAbout();

};

#endif // MAINWINDOW_H
