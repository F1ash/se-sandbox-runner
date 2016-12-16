#ifndef DIRECTORY_SETTINGS_H
#define DIRECTORY_SETTINGS_H

#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

class DirectorySet : public QWidget
{
    Q_OBJECT
public:
    explicit DirectorySet(QWidget *parent = 0);
    QCheckBox       *mountDirs;
    QCheckBox       *guiApp;
    QCheckBox       *securityLevel;
    QLineEdit       *selinuxLabel;
    QLineEdit       *tempDir;
    QLineEdit       *homeDir;
    bool             sessionUsed;

signals:
    void             guiStateChanged(bool);

private:
    QWidget         *tempDirWdg;
    QWidget         *homeDirWdg;
    QLabel          *tempLabel;
    QLabel          *homeLabel;
    QGridLayout     *commonLayout;
    QHBoxLayout     *tempWdgLayout;
    QHBoxLayout     *homeWdgLayout;
    QPushButton     *getTempDir;
    QPushButton     *getHomeDir;

public slots:
    QString          get_TempDir() const;
    QString          get_HomeDir() const;
    QString          get_Mount() const;
    void             setGuiCheckState(int);

private slots:
    void             initTempDirWidget();
    void             initHomeDirWidget();
    void             setTempDir();
    void             setHomeDir();
    void             setWorkDirsState(bool);
    void             gui_StateChanged(bool);
    void             check_SecLevelState();

public slots:
    void             setSELinuxLabelState(bool);
};

#endif //DIRECTORY_SETTINGS_H
