#ifndef INCLUDE_SETTINGS_H
#define INCLUDE_SETTINGS_H

#include <QPushButton>
#include <QListWidget>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>
#include <QFile>

class IncludeSet : public QWidget
{
  Q_OBJECT;
public:
    IncludeSet(QWidget *parent=0);
    ~IncludeSet();
    QString fileName;

signals:

private:
    QGridLayout *commonLayout;
    QListWidget *fileList;
    QPushButton *addFile;
    QPushButton *addDir;
    QPushButton *delPath;

public slots:
    void set_JobName(const QString);
    QStringList get_Included_Path() const;
    const QString get_FileName();
    void setIncludesList(QString s);

private slots:
    void addFilesToList();
    void addDirToList();
    void delPathFromList();
    void delItem(QListWidgetItem*);
    void removeDuplicates();
};

#endif
