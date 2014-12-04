#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui/QDialog>
#include <QtGui/QTabWidget>
#include <QtGui/QPushButton>
#include <QtGui/QListWidgetItem>
#include <QtGui/QTextEdit>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtCore/QProcess>
#include "common_settings.h"
#include "window_settings.h"
#include "include_settings.h"
#include "directory_settings.h"
#include "element/string_list.h"
#include "layout/jobitem_model.h"
#include <QtCore/QDebug>

#define TMP_FILE QString("TEMPORARY_FILE")

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent=0);
    ~SettingsDialog();
    QSettings           settings;
    QTabWidget         *tabWidget;

signals:
    void creatingJobCancelled();

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
    static bool         clean_Directory(QString);

private slots:
    void initTabWidget();
    void initButtons();
    QString includesFileName(QString) const;
    void initParameters();
    void saveJob();
    void cancelJob();
    void saveParameters();
    void closeEvent(QCloseEvent*);
    void windowSetsEnable(int);
    void set_Title_Name(QString);
    void timerEvent(QTimerEvent*);
    QStringList _commandBuild();
    void sessionStateChanged(bool);
    bool make_Directory(QString, QString);
    bool make_SpecifiedDirectories();
    bool set_SpecifiedLabel(QString);
    bool exist_Directory(QLineEdit*);
};

#endif
