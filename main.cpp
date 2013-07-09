#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("se-sandbox-runner");
    a.setApplicationName("se-sandbox-runner");
    MainWindow w;
    w.show();
    
    return a.exec();
}
