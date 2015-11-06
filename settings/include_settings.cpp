#include "include_settings.h"

/* [-I includefile ] [[-i file ]...] */

IncludeSet::IncludeSet(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(1,1,1,1);
    commonLayout = new QGridLayout(this);
    commonLayout->setSpacing(1);
    fileName = QString();
    enabled = new QCheckBox("Enable Included Files", this);
    enabled->setChecked(false);
    enabled->setToolTip("Note: A permanent home directory and included files\
can be used together only once --\nwhen the home directory is empty");
    fileList = new QListWidget(this);
    fileList->setEnabled(false);
    fileList->setSelectionMode(QAbstractItemView::MultiSelection);
    addFile = new QPushButton(QIcon::fromTheme("archive-insert"),"", this);
    addFile->setEnabled(false);
    addFile->setToolTip("Add file(s) to list");
    addDir  = new QPushButton(QIcon::fromTheme("archive-insert-directory"),"", this);
    addDir->setEnabled(false);
    addDir->setToolTip("Add directory to list");
    delPath = new QPushButton(QIcon::fromTheme("archive-remove"),"", this);
    delPath->setEnabled(false);
    delPath->setToolTip("Delete path from list");
    connect(enabled, SIGNAL(toggled(bool)), this, SLOT(enableFileList(bool)));
    connect(addFile, SIGNAL(clicked()), this, SLOT(addFilesToList()));
    connect(addDir, SIGNAL(clicked()), this, SLOT(addDirToList()));
    connect(delPath, SIGNAL(clicked()), this, SLOT(delPathFromList()));

    commonLayout->addWidget(enabled, 0, 0, 1, 4);
    commonLayout->addWidget(fileList, 2, 0, 5, 4);
    commonLayout->addWidget(addFile, 7, 0);
    commonLayout->addWidget(addDir, 7, 1);
    commonLayout->addWidget(delPath, 7, 3);

    setLayout(commonLayout);
}

void IncludeSet::set_FileName(const QString s)
{
    fileName = s;
}
void IncludeSet::enableFileList(bool b)
{
    fileList->setEnabled(b);
    addFile->setEnabled(b);
    addDir->setEnabled(b);
    delPath->setEnabled(b);
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
    if ( !dir.isEmpty() ) {
        if ( !dir.endsWith(QDir::separator()) ) dir.append(QDir::separator());
        fileList->addItem(dir);
        removeDuplicates();
    };
}
void IncludeSet::delPathFromList()
{
    QList<QListWidgetItem *>::const_iterator i;
    QList<QListWidgetItem *> selected;
    selected = fileList->selectedItems();
    for (i = selected.begin(); i!=selected.end(); i++) {
        delItem(*i);
    };
    selected.clear();
}
void IncludeSet::delItem(QListWidgetItem *_item)
{
    if ( _item ) {
        fileList->takeItem(fileList->row(_item));
        //qDebug()<<"delete"<<_file;
    } else
        QMessageBox::information(this, "Info", "Item not exist.");
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
    for (int i = 0; i!=fileList->count(); i++) {
        list.append(fileList->item(i)->text());
    };
    return list;
}
const QString IncludeSet::get_FileName()
{
    if ( fileName.isEmpty() ) return QString();
    QString s = get_Included_Path().join("\n").append("\n");
    //qDebug()<<fileName<<s<<"Incl::get_FileName()";
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
    if ( s.isEmpty() ) return;
    fileName = s;
    QFile *f = new QFile(fileName);
    if ( f->exists() ) {
        QStringList _list;
        QByteArray _data;
        f->open(QIODevice::ReadOnly|QIODevice::Text);
        _data = f->readAll();
        f->close();
        _list = QTextStream(&_data).readAll().split("\n");
        _list.removeAll("");
        fileList->clear();
        fileList->addItems(_list);
    } else
        QMessageBox::information(this, "Info", "ListFile not exist.");
    f->deleteLater();
}
