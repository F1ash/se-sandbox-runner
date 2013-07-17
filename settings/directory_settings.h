#ifndef DIRECTORY_SETTINGS_H
#define DIRECTORY_SETTINGS_H

#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class DirectorySet : public QWidget
{
  Q_OBJECT;
public:
    DirectorySet(QWidget *parent=0);
    ~DirectorySet();
    QCheckBox *mountDirs;
    QCheckBox *guiApp;
    QLineEdit *tempDir;
    QLineEdit *homeDir;

signals:
    void guiStateChanged(bool);

private:
    QWidget *tempDirWdg;
    QWidget *homeDirWdg;
    QLabel *tempLabel;
    QLabel *homeLabel;
    QGridLayout *commonLayout;
    QHBoxLayout *tempWdgLayout;
    QHBoxLayout *homeWdgLayout;
    QPushButton *getTempDir;
    QPushButton *getHomeDir;
    bool sessionUsed;

public slots:
    QString get_TempDir() const;
    QString get_HomeDir() const;
    QString get_Mount() const;
    void setGuiCheckState(int);
    void changeDirsState(bool);

private slots:
    void initTempDirWidget();
    void initHomeDirWidget();
    void setTempDir();
    void setHomeDir();
    void setWorkDirsState();
    void gui_StateChanged(bool);
};

#endif
