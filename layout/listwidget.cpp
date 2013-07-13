#include "layout/listwidget.h"
#include <QDebug>

JobList::JobList(QWidget *parent = 0)
    : QListWidget(parent)
{
  this->setContextMenuPolicy ( Qt::CustomContextMenu );
  setContentsMargins (1, 1, 1, 1);
  setDefaultDropAction ( Qt::IgnoreAction );
  setCursor(Qt::ArrowCursor);
  setSortingEnabled(true);
  jobProcess = new QMap<QString, ElemProcess*>();
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(jobItemClicked(const QPoint &)));
  connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(jobItemDoubleClicked(QListWidgetItem*)));
  stateIcon = QIcon::fromTheme("system-run");
}
JobList::~JobList()
{
  delete jobProcess;
  jobProcess = 0;

  disconnect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(jobItemClicked(const QPoint &)));
  disconnect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(jobItemDoubleClicked(QListWidgetItem*)));
  clear();
}
void JobList::addJobItem(QString const &s)
{
  QListWidgetItem *_item;
  insertItem(0, s);
  _item = findItems(s, Qt::MatchExactly).at(0);
  _item->setIcon( stateIcon );
  _item->setTextAlignment(Qt::AlignLeft);
  createJobProcess(_item);
  if ( _item->text() == QString("<noname>") )
    {
      setCurrentItem(_item);
      editItemAction();
    };
}
void JobList::jobItemClicked(const QPoint &pos)
{
  QListWidgetItem *_item;
  _item = itemAt(pos);
  if (_item==0)
    {
      QMessageBox::information(this, QString("Info"), QString("Item not exist."));
      return;
    };
  qDebug()<<_item->text()<<" Job detected";
  if ( !_item->data(Qt::UserRole).toMap().value("availability", false).toBool() ) return;
  JobMenu *jobMenu = new JobMenu(this);
  if ( _item->data(Qt::UserRole).toMap().value("isRunned", false).toBool() )
    {
      jobMenu->act->setText("Stop Job");
    }
  else
    {
      jobMenu->act->setText("Run Job");
    };
  connect(jobMenu->act, SIGNAL(triggered()), this, SLOT(jobItemAction()));
  connect(jobMenu->edit, SIGNAL(triggered()), this, SLOT(editItemAction()));
  jobMenu->move(mapToGlobal(pos));
  jobMenu->exec();
  disconnect(jobMenu->act, SIGNAL(triggered()), this, SLOT(jobItemAction()));
  disconnect(jobMenu->edit, SIGNAL(triggered()), this, SLOT(editItemAction()));
  jobMenu->deleteLater();
}
void JobList::createJobProcess(QListWidgetItem *_item)
{
  QString key = _item->text();
  ElemProcess *proc = new ElemProcess(this);
  proc->setItemReference(_item);
  jobProcess->insert(key, proc);
  qDebug()<<key<<" create Job process";
  clearSelection();
}
void JobList::jobItemClicked(QListWidgetItem *_item)
{
  qDebug()<<_item->text()<<"  Job clicked";
}
void JobList::jobItemDoubleClicked(QListWidgetItem *_item)
{
  QString key = _item->data(Qt::UserRole).toMap().value(QString("initName")).toString();
  QString _name = _item->text();
  ElemProcess *proc;
  proc = jobProcess->value(key);
  if ( key != _name )
    {
      _item->data(Qt::UserRole).toMap().insert(QString("initName"), QVariant(_name));
      jobProcess->insert(_name, proc);
      jobProcess->remove(key);
      proc->setItemReference(_item);
    };
  qDebug()<<key<<" Job doubleClicked"<<proc;
  bool reason;
  reason = _item->data(Qt::UserRole).toMap().value(QString("reason"), TO_STOP).toBool();
  if ( !_item->data(Qt::UserRole).toMap().value(QString("availability"), NOT_AVAILABLE).toBool() )
    return;
  else if ( proc->state()==QProcess::NotRunning && reason)
    proc->runJob();
  else if ( proc->state()==QProcess::Running )
    proc->killJob();
  clearSelection();
}
void JobList::jobItemAction()
{
  jobItemDoubleClicked(currentItem());
}
void JobList::stopJob(QListWidgetItem *_item)
{
  QMap<QString, QVariant> proc_Status;
  proc_Status = _item->data(Qt::UserRole).toMap();
  proc_Status.insert(QString("reason"), QVariant(TO_STOP));
  _item->setData(Qt::UserRole, QVariant(proc_Status));
  jobItemDoubleClicked(_item);
  proc_Status.insert(QString("reason"), QVariant(TO_RUN));
  _item->setData(Qt::UserRole, QVariant(proc_Status));
}
void JobList::editItemAction()
{
  QListWidgetItem *_item = currentItem();
  if (!_item)
    {
      QMessageBox::information(this, QString("Info"), QString("Item not exist."));
      return;
    };
  sDialog = new SettingsDialog(this->parentWidget());
  sDialog->setJobItem(_item);
  sDialog->exec();
  sDialog->deleteLater();
}
void JobList::deleteCurrentJobItem()
{
  QListWidgetItem *_item;
  _item = currentItem();
  if (_item)
    {
      QString job = _item->text();
      QString msg;
      msg = QString("%1 will delete.\nYou sure?").arg(job);
      int answer = QMessageBox::question(this, "Job delete", msg, QMessageBox::Ok, QMessageBox::Cancel);
      if (answer==QMessageBox::Ok)
        {
          takeItem(currentRow());
          ElemProcess *proc;
          proc = jobProcess->value(job);
          if ( proc && proc->state()==QProcess::Running )
            {
              proc->killJob();
              jobProcess->remove(job);
              qDebug()<<"delete"<<job;
            };
          emit removeJob(job);
        };
    }
  else QMessageBox::information(this, QString("Info"), QString("Item not exist."));
}
