#ifndef JOBMENU_H
#define JOBMENU_H

#include <QMenu>

class JobMenu : public QMenu
{
    Q_OBJECT
public:
    explicit JobMenu(QWidget *parent = 0);

    QAction  *edit;
    QAction  *undock;
    QAction  *act;

signals:

public slots:

private:

private slots:

};
#endif //JOBMENU_H
