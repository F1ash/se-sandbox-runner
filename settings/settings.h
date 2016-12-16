#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QTabWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QSettings>
#include <QCloseEvent>
#include <QProcess>
#include "common_settings.h"
#include "window_settings.h"
#include "include_settings.h"
#include "directory_settings.h"
#include "element/string_list.h"
#include "layout/jobitem_model.h"
#include "element/shred_thread.h"
#include <QDebug>

#define TMP_FILE QString("TEMPORARY_FILE")

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    QSettings           settings;
    QTabWidget         *tabWidget;

signals:
    void                creatingJobCancelled();

private:
    JobItemIndex       *own_index;

    QVBoxLayout        *commonLayout;
    QHBoxLayout        *buttonsLayout;
    QWidget            *buttons;
    QListWidgetItem    *item;
    QString             name;
    QPushButton        *ok;
    QPushButton        *cancel;
    CommonSet          *w1;
    WindowSet          *w2;
    IncludeSet         *w3;
    DirectorySet       *w4;
    bool                newbe;
    QString             previousName;
    QTextEdit          *fullCommandWdg;
    int                 timerId;
    String             *commandLine;

public slots:
    void                setJobItem(JobItemIndex*);
    static bool         clean_Directory(
            QString, void* opaque = NULL, void *p = NULL);

private slots:
    void                initTabWidget();
    void                initButtons();
    QString             includesFileName(QString) const;
    void                initParameters();
    void                saveJob();
    void                cancelJob();
    void                saveParameters();
    void                closeEvent(QCloseEvent*);
    void                windowSetsEnable(int);
    void                set_Title_Name(QString);
    void                timerEvent(QTimerEvent*);
    QStringList         _commandBuild();
    void                sessionStateChanged(bool);
    bool                make_Directory(QString, QString);
    bool                make_SpecifiedDirectories();
    bool                set_SpecifiedLabel(QString);
    bool                exist_Directory(QLineEdit*);
};

#endif
