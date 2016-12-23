#include "toolbar.h"

ToolBar::ToolBar (QWidget *parent) : QToolBar(parent)
{
    setAllowedAreas(Qt::AllToolBarAreas);
    setMovable(true);
    setFloatable(true);

    initActions();
}

void ToolBar::initActions()
{
    _hideAction = new QAction(QString("Hide to tray"), this);
    _hideAction->setIcon ( QIcon::fromTheme("down") );
    _createAction = new QAction(QString("Create new Job"), this);
    _createAction->setIcon ( QIcon::fromTheme("install") );
    _editAction = new QAction(QString("Edit selected Job"), this);
    _editAction->setIcon ( QIcon::fromTheme("configure") );
    _deleteAction = new QAction(QString("Delete selected Job"), this);
    _deleteAction->setIcon ( QIcon::fromTheme("clean") );
    _runAction = new QAction(QString("Run selected Job"), this);
    _runAction->setIcon ( QIcon::fromTheme("run") );
    _undockAction = new QAction(QString("Undock selected Job"), this);
    _undockAction->setIcon ( QIcon::fromTheme("undock") );
    _stopAction = new QAction(QString("Kill selected Job"), this);
    _stopAction->setIcon ( QIcon::fromTheme("stop") );
    _stopAllAction = new QAction(QString("Kill all Job"), this);
    _stopAllAction->setIcon ( QIcon::fromTheme("stop-all") );
    _exitAction = new QAction(QString("Exit"), this);
    _exitAction->setIcon ( QIcon::fromTheme("exit") );
    _infoAction = new QAction(QString("About"), this);
    _infoAction->setIcon( QIcon::fromTheme("dialog-information") );

    itemControlMenu = new QMenu(this);

    itemControlAction = new QAction(this);
    itemControlAction->setIcon(QIcon::fromTheme("job"));
    itemControlAction->setToolTip("Job Control");
    //connect(_hideAction, SIGNAL(hovered()),
    //        this, SLOT(showHoveredMenu()));
    //connect(itemControlAction, SIGNAL(hovered()),
    //        this, SLOT(showHoveredMenu()));
    //connect(_exitAction, SIGNAL(hovered()),
    //        this, SLOT(showHoveredMenu()));

    itemControlMenu->addAction(_createAction);
    itemControlMenu->addAction(_editAction);
    itemControlMenu->addAction(_deleteAction);
    itemControlMenu->addSeparator();
    itemControlMenu->addAction(_runAction);
    itemControlMenu->addAction(_undockAction);
    itemControlMenu->addAction(_stopAction);
    itemControlMenu->addAction(_stopAllAction);
    itemControlAction->setMenu(itemControlMenu);

    addAction(_hideAction);
    addSeparator();
    addAction( itemControlAction );
    addSeparator();
    addAction(_exitAction);
    addSeparator();
    addAction(_infoAction);
}
void ToolBar::showHoveredMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act==itemControlAction ) {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    } else {
        if ( itemControlAction->menu()->isVisible() )
            itemControlAction->menu()->hide();
    }
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
