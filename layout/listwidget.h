#include <QListWidget>
#include <QMessageBox>
#include "jobmenu.h"
//#include "settings/settings_stuff.h"
#include "settings/settings.h"
#include "element/elemprocess.h"
//#include "utils/pgrep.h"

#define TO_RUN true
#define TO_STOP false

class JobList : public QListWidget
{
    Q_OBJECT

public:
    JobList(QWidget *parent = 0);
    ~JobList();

    QMap<QString, ElemProcess*> *jobProcess;

signals:
    void clickedItem(QString);
    void removeJob(QString &);
    void restoreJobParameter(QString&, QString&, QVariant&);

public slots:
    void setSettingsReference(SettingsStuff &);
    void editItemAction();
    void addJobItem(QString &s);
    void deleteCurrentJobItem();
    void stopJob(QListWidgetItem *_item);

private :
    SettingsStuff settings;

private slots:
    void jobItemClicked(const QPoint &pos);
    void jobItemClicked(QListWidgetItem *_item);
    void jobItemDoubleClicked(QListWidgetItem *_item);
    void createJobProcess(QListWidgetItem *_item);
    void jobItemAction();

};
