#include "mainwindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString name = QString("se-sandbox-runner");
    a.setOrganizationName(name);
    a.setApplicationName(name);
    MainWindow w;
    w.show();
    
    return a.exec();
}
