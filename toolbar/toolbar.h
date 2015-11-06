#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QMenu>
#include <QDebug>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent);

    QAction    *_hideAction;
    QAction    *_createAction;
    QAction    *_editAction;
    QAction    *_deleteAction;
    QAction    *_runAction;
    QAction    *_undockAction;
    QAction    *_stopAction;
    QAction    *_stopAllAction;
    QAction    *_exitAction;
    QAction    *_infoAction;

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
