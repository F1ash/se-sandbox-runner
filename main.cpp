#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(se_sandbox_runner);
    QString name = QString("se-sandbox-runner");
    a.setOrganizationName(name);
    a.setApplicationName(name);
    MainWindow w;
    w.show();
    
    return a.exec();
}
