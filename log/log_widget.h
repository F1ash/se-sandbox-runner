#ifndef LOG_DOCK_H
#define LOG_DOCK_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QTextBrowser>
#include <QTime>
#include <QTimerEvent>
#include <QUrl>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QSettings>
#include <QCheckBox>
#include <QSpinBox>
#include <QDebug>

#define ONE_MB 1048576

class LogWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = NULL);
    ~LogWidget();

private:
    bool              lastProbe;
    int               timerId, currLogSize;
    QSettings         settings;
    QSize             _size;
    QVBoxLayout      *baseLayout;
    QLabel           *currentTime;
    QSpinBox         *logSize;
    QCheckBox        *autoSaveLog;
    QPushButton      *saveLog;
    QHBoxLayout      *titleLayout;
    QWidget          *title, *baseWdg;
    QTextBrowser     *Log;

signals:
    void              overflow(bool);

public slots:
    void              appendMsgToLog(QString&);

private slots:
    void              timerEvent(QTimerEvent*);
    void              openLink(QUrl);
    void              _saveLogToFile();
    void              saveLogToFile();
    QString           getTemplateFilename() const;
    void              changeLogSize(int);

};

#endif // LOG_DOCK_H
