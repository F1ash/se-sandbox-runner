#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>

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
    QCheckBox *shred;
    QCheckBox *capabilities;
    QComboBox *sandboxType;
    QComboBox *securityLayer;
    QRadioButton *execute;
    QRadioButton *session;
    QLineEdit *command;

signals:

private:
    QGridLayout *gridLayout;
    QLabel *secLabel;
    QLabel *typeLabel;
    QPushButton *selectFile;
    QWidget *cmdWidget;
    QHBoxLayout *cmdLayout;

public slots:
    QString get_Job_Name() const;
    void set_Job_Name(QString&);

private slots:
    void initTypeBox();
    void setTypeToolTip(QString);
    void initSLevelBox();
    void initRadioButtons();
    void enableCommand(bool b);
    void enableSLevel(bool);
    void initCmdWidget();
    void setCommandPath();
};
