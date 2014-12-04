#ifndef JOBMENU_H
#define JOBMENU_H

#include <QtGui/QMenu>

class JobMenu : public QMenu
{
    Q_OBJECT
public:
    JobMenu(QWidget *parent = 0);
    ~JobMenu();

    QAction  *edit;
    QAction  *undock;
    QAction  *act;

signals:

public slots:

private:

private slots:

};
#endif //JOBMENU_H
