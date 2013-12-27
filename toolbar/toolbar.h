#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QDebug>

class ToolBar : public QToolBar
{
  Q_OBJECT;
public:
  ToolBar(QWidget *parent);
  ~ToolBar();

  QAction *_hideAction;
  QAction *_createAction;
  QAction *_editAction;
  QAction *_deleteAction;
  QAction *_runAction;
  QAction *_stopAction;
  QAction *_stopAllAction;
  QAction *_exitAction;

signals:

private:

private slots:
  void initActions();

public slots:
  Qt::ToolBarArea get_ToolBarArea(int) const;
};

#endif //TOOLBAR_H
