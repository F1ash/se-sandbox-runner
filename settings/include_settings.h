#ifndef INCLUDE_SETTINGS_H
#define INCLUDE_SETTINGS_H

#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QListWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
#include <QtCore/QFile>
#include <QtCore/QDebug>

class IncludeSet : public QWidget
{
    Q_OBJECT
public:
    IncludeSet(QWidget *parent=0);
    ~IncludeSet();
    QString          fileName;
    QCheckBox       *enabled;

signals:

private:
    QGridLayout     *commonLayout;
    QListWidget     *fileList;
    QPushButton     *addFile;
    QPushButton     *addDir;
    QPushButton     *delPath;

public slots:
    void            set_FileName(const QString);
    QStringList     get_Included_Path() const;
    const QString   get_FileName();
    void            setIncludesList(QString);

private slots:
    void enableFileList(bool);
    void addFilesToList();
    void addDirToList();
    void delPathFromList();
    void delItem(QListWidgetItem*);
    void removeDuplicates();
};

#endif //INCLUDE_SETTINGS_H
