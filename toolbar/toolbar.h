#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QMenu>

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
  QAction *_stopAction;
  QAction *_stopAllAction;

signals:

private:

private slots:
  void initActions();

public slots:
  Qt::ToolBarArea get_ToolBarArea(int) const;
};

