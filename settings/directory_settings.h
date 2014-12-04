#ifndef DIRECTORY_SETTINGS_H
#define DIRECTORY_SETTINGS_H

#include <QtGui/QFileDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>

class DirectorySet : public QWidget
{
    Q_OBJECT
public:
    DirectorySet(QWidget *parent=0);
    ~DirectorySet();
    QCheckBox   *mountDirs;
    QCheckBox   *guiApp;
    QCheckBox   *securityLevel;
    QLineEdit   *selinuxLabel;
    QLineEdit   *tempDir;
    QLineEdit   *homeDir;
    bool         sessionUsed;

signals:
    void guiStateChanged(bool);

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
    QString     get_TempDir() const;
    QString     get_HomeDir() const;
    QString     get_Mount() const;
    void        setGuiCheckState(int);

private slots:
    void initTempDirWidget();
    void initHomeDirWidget();
    void setTempDir();
    void setHomeDir();
    void setWorkDirsState(bool);
    void gui_StateChanged(bool);
    void check_SecLevelState();

public slots:
    void setSELinuxLabelState(bool);
};

#endif //DIRECTORY_SETTINGS_H
