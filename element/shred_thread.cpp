#include "shred_thread.h"

ShredThread::ShredThread(QObject *parent) :
    QThread(parent)
{
}

/* public slots */
void ShredThread::run()
{
    PERCENT *p = new PERCENT();
    quint64 _size = getDirSize(tempDir) + getDirSize(homeDir);
    //qDebug()<<_size<<"common size";
    p->setSize( _size );
    SettingsDialog::clean_Directory( tempDir, this, p);
    SettingsDialog::clean_Directory( homeDir, this, p);
    p->clear();
    msleep(1333);
}
void ShredThread::shredStateChanged(uint i)
{
    emit stateChanged(i);
}

/* private slots */
qint64 ShredThread::getDirSize(QString &dir)
{
    uint _size;
    QDir::Filters flags =
            QDir::AllEntries |
            QDir::NoDotAndDotDot |
            QDir::Hidden |
            QDir::System |
            QDir::AllDirs;
    QDir d;
    d.setPath(dir);
    QList<QFileInfo> entry = d.entryInfoList(flags);
    if ( !entry.isEmpty() ) {
        QList<QFileInfo>::const_iterator i;
        for ( i=entry.constBegin(); i<entry.constEnd(); i++ ) {
            QFileInfo item = *i;
            if ( !item.exists() ) continue;
            QString path = item.canonicalFilePath();
            if ( path==d.absoluteFilePath(dir)
                 || item.isSymLink() ) continue;
            if ( item.isDir() ) {
                _size += getDirSize(path);
            } else {
                _size += item.size();
            };
        };
    };
    return _size;
}
