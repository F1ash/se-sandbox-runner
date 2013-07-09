//#include <QSettings>
#include <QDialog>
#include <QTabWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QCloseEvent>
#include "settings_stuff.h"
#include "common_settings.h"
#include "window_settings.h"
#include "include_settings.h"
#include "directory_settings.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT;
public:
    SettingsDialog(QWidget *parent=0);
    ~SettingsDialog();
    SettingsStuff *settings;
    QTabWidget *tabWidget;

signals:

private:
    QVBoxLayout *commonLayout;
    QHBoxLayout *buttonsLayout;
    QWidget *buttons;
    QListWidgetItem *item;
    QString name;
    QPushButton *ok;
    QPushButton *cancel;
    CommonSet *w1;
    WindowSet *w2;
    IncludeSet *w3;
    DirectorySet *w4;
    bool newbe;
    QString previousName;

public slots:
    void setSettingsReference(SettingsStuff&);
    void setJobItem(QListWidgetItem *);
    void setGuiCheckState(int);

private slots:
    void initTabWidget();
    void initButtons();
    void initParameters();
    void saveJob();
    void cancelJob();
    void saveParameters();
    void closeEvent(QCloseEvent *);
    void windowSetsEnable(int);
    void set_Title_Name(QString);
};

