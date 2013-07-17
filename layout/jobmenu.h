#ifndef JOBMENU_H
#define JOBMENU_H
#include <QMenu>

class JobMenu : public QMenu
{
    Q_OBJECT;
public:
    JobMenu(QWidget *parent = 0);
    ~JobMenu();

    QAction *edit;
    QAction *act;

signals:

public slots:

private:

private slots:

};
#endif
