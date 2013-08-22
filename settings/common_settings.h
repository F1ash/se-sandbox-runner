#ifndef COMMON_SETTINGS_H
#define COMMON_SETTINGS_H

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

#define TIMEOUT 10

class CommonSet : public QWidget
{
  Q_OBJECT;
public:
    CommonSet(QWidget *parent=0);
    ~CommonSet();
    QCheckBox *autoRun;
    QCheckBox *cgroups;
    QLineEdit *nameEdit;
    QCheckBox *guiApp;
    QCheckBox *runInTerm;
    QCheckBox *shred;
    QCheckBox *capabilities;
    QComboBox *sandboxType;
    QComboBox *securityLayer;
    QRadioButton *execute;
    QRadioButton *session;
    QLineEdit *command;
    QSpinBox *checkTimeout;
    QRadioButton *defaultTerminal;
    QRadioButton *customTerminal;
    QLineEdit *termCommand;

signals:

private:
    QGridLayout *gridLayout;
    QLabel *secLabel;
    QLabel *typeLabel;
    QLabel *timeoutLabel;
    QPushButton *selectFile;
    QWidget *commonWdg;
    QVBoxLayout *commonCmdLayout;
    QLabel *runInTermLabel;
    QWidget *cmdWidget;
    QHBoxLayout *cmdLayout;
    QWidget *termChoiseWidget;
    QVBoxLayout *termChoiseLayout;

public slots:
    QString get_Job_Name() const;
    void set_Job_Name(QString&);

private slots:
    void initTypeBox();
    void setTypeToolTip(QString);
    void initSLevelBox();
    void initRadioButtons();
    void initTimeoutWidget();
    void initTermChoiseWdg();
    void enableCommand(bool b);
    void enableSLevel(bool);
    void initCmdWidget();
    void setCommandPath();
    void showTerminalChioseWdg(bool);
};

#endif
