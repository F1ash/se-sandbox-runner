#include "include_settings.h"
#include <QDebug>

/* [-I includefile ] [[-i file ]...] */

IncludeSet::IncludeSet(QWidget *parent) :
    QWidget(parent)
{
  setContentsMargins(1,1,1,1);
  commonLayout = new QGridLayout(this);
  fileList = new QListWidget(this);
  fileList->setSelectionMode(QAbstractItemView::MultiSelection);
  addFile = new QPushButton(QIcon::fromTheme("list-add"),"", this);
  addFile->setToolTip("Add file(s) to list");
  addDir  = new QPushButton(QIcon::fromTheme("list-add"),"", this);
  addDir->setToolTip("Add directory to list");
  delPath = new QPushButton(QIcon::fromTheme("list-remove"),"", this);
  delPath->setToolTip("Delete path from list");
  connect(addFile, SIGNAL(clicked()), this, SLOT(addFilesToList()));
  connect(addDir, SIGNAL(clicked()), this, SLOT(addDirToList()));
  connect(delPath, SIGNAL(clicked()), this, SLOT(delPathFromList()));

  commonLayout->addWidget(fileList, 0, 0, 5, 5);
  commonLayout->addWidget(addFile, 6, 0);
  commonLayout->addWidget(addDir, 6, 1);
  commonLayout->addWidget(delPath, 6, 4);

  setLayout(commonLayout);
}
IncludeSet::~IncludeSet()
{
  disconnect(addFile, SIGNAL(clicked()), this, SLOT(addFilesToList()));
  disconnect(addDir, SIGNAL(clicked()), this, SLOT(addDirToList()));
  disconnect(delPath, SIGNAL(clicked()), this, SLOT(delPathFromList()));

  delete addFile;
  addFile = 0;
  delete addDir;
  addDir = 0;
  delete delPath;
  delPath = 0;
  delete fileList;
  fileList = 0;
  delete commonLayout;
  commonLayout = 0;
}
void IncludeSet::set_JobName(const QString s)
{
  fileName = QDir::homePath();
  fileName.append("/.config/se-sandbox-runner/");
  fileName.append(QString("%1.included").arg(s));
  qDebug()<<fileName<<s<<"Incl::setJobName()";
}
void IncludeSet::addFilesToList()
{
  QStringList sL = QFileDialog::getOpenFileNames(this, "Include files", "~");
  fileList->addItems(sL);
  removeDuplicates();
}
void IncludeSet::addDirToList()
{
  QString dir = QFileDialog::getExistingDirectory(this, "Include directory", "~");
  fileList->addItem(dir);
  removeDuplicates();
}
void IncludeSet::delPathFromList()
{
  QList<QListWidgetItem *>::const_iterator i;
  QList<QListWidgetItem *> selected;
  selected = fileList->selectedItems();
  for (i = selected.begin(); i!=selected.end(); i++)
    { delItem(*i); };
  selected.clear();
}
void IncludeSet::delItem(QListWidgetItem *_item)
{
  if (_item)
    {
      QString _file = _item->text();
      QString msg;
      msg = QString("%1 \nwill delete.\nYou sure?").arg(_file);
      int answer = QMessageBox::question(this, "File delete", msg, QMessageBox::Ok, QMessageBox::Cancel);
      if (answer==QMessageBox::Ok)
        {
          fileList->takeItem(fileList->row(_item));
          qDebug()<<"delete"<<_file;
        };
    }
  else QMessageBox::information(this, "Info", "Item not exist.");
}
void IncludeSet::removeDuplicates()
{
  QStringList _items = get_Included_Path();
  _items.removeDuplicates();
  fileList->clear();
  fileList->addItems(_items);
  _items.clear();
}
QStringList IncludeSet::get_Included_Path() const
{
  QStringList list;
  for (int i = 0; i!=fileList->count(); i++)
    { list.append(fileList->item(i)->text()); };
  return list;
}
const QString IncludeSet::get_FileName()
{
  const QString res;
  QString s = get_Included_Path().join("\n");
  qDebug()<<fileName<<s<<"Incl::get_FileName()";
  int l;
  QFile *f = new QFile(fileName, this);
  f->open(QIODevice::WriteOnly|QIODevice::Text);
  l = f->write(s.toUtf8());
  f->close();
  f->deleteLater();
  return ( l == s.toUtf8().length() ) ? fileName : QString();
}
void IncludeSet::setIncludesList(QString s)
{
  if (s.isEmpty()) return;
  fileName = s;
  QFile *f = new QFile(fileName);
  if (f->exists())
    {
      QStringList _list;
      QByteArray _data;
      f->open(QIODevice::ReadOnly|QIODevice::Text);
      _data = f->readAll();
      f->close();
      _list = QTextStream(&_data).readAll().split("\n");
      _list.removeAll("");
      fileList->clear();
      fileList->addItems(_list);
    }
  else QMessageBox::information(this, "Info", "ListFile not exist.");
  f->deleteLater();
}
