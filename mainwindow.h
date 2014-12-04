#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtCore/QStringList>
#include "tray/traywidget.h"
#include "layout/listwidget.h"
#include "toolbar/toolbar.h"
#include "wait_thread.h"
#include <QtCore/QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSettings   settings;
    JobList    *jobWidget;

signals:

public slots:

private :
    TrayIcon   *trayIcon;
    ToolBar    *toolBar;
    Wait       *wait_thread = NULL;

private slots:
    void closeEvent(QCloseEvent*);
    void closeEvent();
    void changeVisibility();
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

};

#endif // MAINWINDOW_H
