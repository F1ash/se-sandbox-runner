#include "layout/listwidget.h"

JobList::JobList(QWidget *parent = 0)
    : QTreeView(parent)
{
  this->setContextMenuPolicy ( Qt::CustomContextMenu );
  setContentsMargins (1, 1, 1, 1);
  setDefaultDropAction ( Qt::IgnoreAction );
  setCursor(Qt::ArrowCursor);
  setSortingEnabled(true);
  this->setItemsExpandable(false);
  this->setRootIsDecorated(false);
  jobItemModel = new JobItemModel(this);
  this->setModel(jobItemModel);
  progressBarDlg = new ProgressBarDelegate();
  this->setItemDelegate(progressBarDlg);
  jobProcess = new QMap<QString, ElemProcess*>();
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(jobItemClicked(const QPoint &)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(jobItemDoubleClicked(const QModelIndex&)));

}
JobList::~JobList()
{
    disconnect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(jobItemClicked(const QPoint &)));
    disconnect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(jobItemDoubleClicked(const QModelIndex&)));
    delete progressBarDlg;
    progressBarDlg = 0;
    delete jobItemModel;
    jobItemModel = 0;
    jobProcess->clear();
    delete jobProcess;
    jobProcess = 0;
}
void JobList::addJobItem(const QString &s)
{
    int count = jobItemModel->rowCount();
    bool exist = false;
    for (int i=0; i<count; i++) {
        JobItemIndex *idx = jobItemModel->jobItemDataList.at(i);
        if ( idx->getName()==s ) {
            exist = true;
            break;
        }
    };
    if ( !exist ) {
        jobItemModel->insertRow(0);
        QModelIndex item;
        for (int j=0; j<3; j++) {
            QString data;
            if (j) {
                data = "-";
            } else data = s;
            item = jobItemModel->index(0,j);
            jobItemModel->setData(item, data, Qt::EditRole);
        };
        if ( s==QString("<noname>") && item.isValid() ) {
            setCurrentIndex(item);
            editItemAction();
        };
        createJobProcess(item);
    };
}
void JobList::jobItemClicked(const QPoint &pos)
{
    QModelIndex _item = indexAt(pos);
    if ( !_item.isValid() ) {
        clearSelection();
        //showMessage("Info", "Item not exist.");
        return;
    };
    //qDebug()<<_item->text()<<" Job detected";
    DATA proc_Status;
    JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
    proc_Status = idx->getData();
    if ( !proc_Status.value("availability", NOT_AVAILABLE).toBool() ) return;
    bool to_run = TO_RUN;
    JobMenu *jobMenu = new JobMenu(this);
    if ( proc_Status.value("isRunning", STOPPED).toBool() )
      {
        jobMenu->act->setText("Kill Job");
        jobMenu->act->setIcon(QIcon::fromTheme("stop"));
        connect(jobMenu->act, SIGNAL(triggered()), this, SLOT(jobItemKillAction()));
        jobMenu->undock->setEnabled(true);
        to_run = TO_STOP;
      }
    else
      {
        jobMenu->act->setText("Run Job");
        jobMenu->act->setIcon(QIcon::fromTheme("run"));
        connect(jobMenu->act, SIGNAL(triggered()), this, SLOT(jobItemRunAction()));
        jobMenu->undock->setEnabled(false);
        to_run = TO_RUN;
      };
    idx->setData(proc_Status);
    connect(jobMenu->edit, SIGNAL(triggered()), this, SLOT(editItemAction()));
    connect(jobMenu->undock, SIGNAL(triggered()), this, SLOT(jobItemUndockAction()));
    jobMenu->move(mapToGlobal(pos));
    jobMenu->exec();
    if (to_run) disconnect(jobMenu->act, SIGNAL(triggered()), this, SLOT(jobItemRunAction()));
    else disconnect(jobMenu->act, SIGNAL(triggered()), this, SLOT(jobItemKillAction()));
    disconnect(jobMenu->edit, SIGNAL(triggered()), this, SLOT(editItemAction()));
    disconnect(jobMenu->undock, SIGNAL(triggered()), this, SLOT(jobItemUndockAction()));
    jobMenu->deleteLater();
}
void JobList::createJobProcess(QModelIndex &_item)
{
  JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
  QString key = idx->getName();
  ElemProcess *proc = new ElemProcess(this);
  proc->setItemReference(jobItemModel, idx);
  jobProcess->insert(key, proc);
  //qDebug()<<key<<" create Job process";
  clearSelection();
}
void JobList::jobItemDoubleClicked(const QModelIndex &_item)
{
  JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
  QString _name = idx->getName();
  DATA proc_Status;
  proc_Status = idx->getData();
  QString key = proc_Status.value(QString("initName")).toString();
  ElemProcess *proc;
  proc = jobProcess->value(key);
  if ( key != _name ) {
      proc_Status.insert(QString("initName"), QVariant(_name));
      jobProcess->insert(_name, proc);
      jobProcess->remove(key);
      proc->setItemReference(jobItemModel, idx);
  };
  //qDebug()<<key<<" Job doubleClicked"<<proc;
  bool proc_state;
  proc_state = proc_Status.value(QString("isRunning"), STOPPED).toBool();
  if ( !proc_Status.value(QString("availability"), NOT_AVAILABLE).toBool() ) {
      showMessage("Info", "Job is busy.");
  }
  else if ( !proc_state && proc->state()==QProcess::NotRunning )
      proc->runJob();
  else if ( proc_state && proc->state()==QProcess::Running )
      proc->killJob();
  clearSelection();
}
void JobList::jobItemUndockAction()
{
    QModelIndex idx = currentIndex();
    undockJob(idx);
}
void JobList::jobItemKillAction()
{
    QModelIndex idx = currentIndex();
    checkJob(idx, TO_STOP);
}
void JobList::jobItemRunAction()
{
    QModelIndex idx = currentIndex();
    checkJob(idx, TO_RUN);
}
void JobList::runJob(QModelIndex &_item)
{
    checkJob(_item, TO_RUN);
}
void JobList::undockJob(QModelIndex &_item)
{
    JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
    QString _name = idx->getName();
    DATA proc_Status;
    proc_Status = idx->getData();
    QString key = proc_Status.value(QString("initName")).toString();
    ElemProcess *proc;
    proc = jobProcess->value(key);
    if ( key != _name ) {
        proc_Status.insert(QString("initName"), QVariant(_name));
        jobProcess->insert(_name, proc);
        jobProcess->remove(key);
        proc->setItemReference(jobItemModel, idx);
    };
    //qDebug()<<key<<" Job doubleClicked"<<proc;
    bool proc_state;
    proc_state = proc_Status.value(QString("isRunning"), STOPPED).toBool();
    if ( !proc_Status.value(QString("availability"), NOT_AVAILABLE).toBool() ) {
        showMessage("Info", "Job is busy.");
    }
    else if ( proc_state && proc->state()==QProcess::Running )
        proc->undockJob();
    clearSelection();
}
void JobList::stopJob(QModelIndex &_item)
{
    checkJob(_item, TO_STOP);
}
void JobList::checkJob(QModelIndex &_item, bool to_run = TO_RUN)
{
    bool proc_state;
    JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
    proc_state = idx->getData().value(QString("isRunning"), STOPPED).toBool();
    if ( (to_run && !proc_state) || (!to_run && proc_state) )
        jobItemDoubleClicked(_item);
}
void JobList::editItemAction()
{
    QModelIndex _item = currentIndex();
    if ( !_item.isValid() ) {
        showMessage("Info", "Item not exist.");
        return;
    };
    JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
    sDialog = new SettingsDialog(this->parentWidget());
    sDialog->setJobItem(idx);
    connect(sDialog, SIGNAL(creatingJobCancelled()), this, SLOT(deleteCancelledCreation()));
    sDialog->exec();
    disconnect(sDialog, SIGNAL(creatingJobCancelled()), this, SLOT(deleteCancelledCreation()));
    sDialog->deleteLater();
}
void JobList::deleteCurrentJobItem()
{
    QModelIndex _item = currentIndex();
    if ( _item.isValid() ) {
        JobItemIndex *idx = jobItemModel->jobItemDataList.at(_item.row());
        QString job = idx->getName();
        ElemProcess *proc;
        proc = jobProcess->value(job);
        bool proc_state;
        proc_state = idx->getData().value(QString("availability"), AVAILABLE).toBool();
        if ( !proc_state ) {
            showMessage("Info", "Job is busy.");
            clearSelection();
            return;
        };
        if ( proc && proc->state()==QProcess::Running ) {
            proc->killJob();
        };
        jobProcess->remove(job);
        jobItemModel->removeRow(_item.row());
        emit removeJob(job);
    } else showMessage("Info", "Item not exist.");
}
void JobList::deleteCancelledCreation()
{
  deleteCurrentJobItem();
}
void JobList::showMessage(QString title, QString msg)
{
  QMessageBox::information(this, title, msg);
}
