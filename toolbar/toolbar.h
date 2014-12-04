#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtCore/QDebug>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    ToolBar(QWidget *parent);
    ~ToolBar();

    QAction    *_hideAction;
    QAction    *_createAction;
    QAction    *_editAction;
    QAction    *_deleteAction;
    QAction    *_runAction;
    QAction    *_undockAction;
    QAction    *_stopAction;
    QAction    *_stopAllAction;
    QAction    *_exitAction;

signals:

private:
    QMenu      *itemControlMenu;
    QAction    *itemControlAction;

private slots:
    void initActions();
    void showHoveredMenu();

public slots:
    Qt::ToolBarArea get_ToolBarArea(int) const;
};

#endif //TOOLBAR_H
