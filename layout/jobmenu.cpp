#include "jobmenu.h"

JobMenu::JobMenu(QWidget *parent)
    : QMenu(parent)
{
    edit   = addAction(QIcon::fromTheme("configure"), "Edit");
    undock = addAction(QIcon::fromTheme("undock"), "Undock Job");
    act    = addAction(QIcon::fromTheme("run"), "Run Job");
}

