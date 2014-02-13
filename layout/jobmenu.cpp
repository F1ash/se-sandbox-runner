#include "jobmenu.h"

JobMenu::JobMenu(QWidget *parent)
    : QMenu(parent)
{
  edit   = addAction(QIcon::fromTheme("configure"), "Edit");
  undock = addAction(QIcon::fromTheme("undock"), "Undock Job");
  act    = addAction(QIcon::fromTheme("run"), "Run Job");
}
JobMenu::~JobMenu()
{
  delete edit;
  edit = 0;
  delete undock;
  undock = 0;
  delete act;
  act = 0;
}
