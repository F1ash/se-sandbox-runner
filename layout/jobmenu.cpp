#include "jobmenu.h"

JobMenu::JobMenu(QWidget *parent)
    : QMenu(parent)
{
  edit = addAction(QIcon::fromTheme("run-build-configure"), "Edit");
  act  = addAction(QIcon::fromTheme("run-build"), "Run Job");
}
JobMenu::~JobMenu()
{
  delete edit;
  edit = 0;
  delete act;
  act = 0;
}
