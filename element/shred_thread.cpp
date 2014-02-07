#include "shred_thread.h"

ShredThread::ShredThread(QObject *parent) :
    QThread(parent)
{
}
void ShredThread::run()
{
    SettingsDialog::clean_Directory( tempDir );
    SettingsDialog::clean_Directory( homeDir );
}
