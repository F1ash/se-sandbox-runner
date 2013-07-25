#include "toolbar.h"
#include <QDebug>

ToolBar::ToolBar (QWidget *parent = 0) : QToolBar(parent)
{
  setAllowedAreas(Qt::AllToolBarAreas);
  setMovable(true);
  setFloatable(true);

  initActions();
}
ToolBar::~ToolBar()
{
  delete _hideAction;
  _hideAction = 0;
  delete _createAction;
  _createAction = 0;
  delete _deleteAction;
  _deleteAction = 0;
  delete _editAction;
  _editAction = 0;
  delete _stopAction;
  _stopAction = 0;
  delete _stopAllAction;
  _stopAllAction = 0;
  delete _exitAction;
  _exitAction = 0;

}
void ToolBar::initActions()
{
  _hideAction = new QAction(QString("Hide to tray"), this);
  _hideAction->setIcon ( QIcon().fromTheme("arrow-down") );
  _createAction = new QAction(QString("Create new Job"), this);
  _createAction->setIcon ( QIcon().fromTheme("list-add") );
  _editAction = new QAction(QString("Edit selected Job"), this);
  _editAction->setIcon ( QIcon().fromTheme("edit-select") );
  _deleteAction = new QAction(QString("Delete Job"), this);
  _deleteAction->setIcon ( QIcon().fromTheme("list-remove") );
  _stopAction = new QAction(QString("Kill selected Job"), this);
  _stopAction->setIcon ( QIcon().fromTheme("process-stop") );
  _stopAllAction = new QAction(QString("Kill all Job"), this);
  _stopAllAction->setIcon ( QIcon().fromTheme("system-log-out") );
  _exitAction = new QAction(QString("Exit"), this);
  _exitAction->setIcon ( QIcon().fromTheme("system-shutdown") );

  addAction(_hideAction);
  addSeparator();
  addAction(_createAction);
  addAction(_editAction);
  addAction(_deleteAction);
  addSeparator();
  addAction(_stopAction);
  addAction(_stopAllAction);
  addSeparator();
  addAction(_exitAction);
}
Qt::ToolBarArea ToolBar::get_ToolBarArea(int i) const
{
  Qt::ToolBarArea result;
  switch (i) {
  case 1:
    result = Qt::LeftToolBarArea;
    break;
  case 2:
    result = Qt::RightToolBarArea;
    break;
  case 4:
    result = Qt::TopToolBarArea;
    break;
  case 8:
    result = Qt::BottomToolBarArea;
    break;
  default:
    result = Qt::TopToolBarArea;
    break;
  };
  return result;
}
