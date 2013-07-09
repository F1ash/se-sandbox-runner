#include "jobmenu.h"

JobMenu::JobMenu(QWidget *parent)
    : QMenu(parent)
{
  setMinimumSize(32, 32);
  edit = addAction("Edit");
  act  = addAction("Run Job");
}
JobMenu::~JobMenu()
{

}
