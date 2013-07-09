
#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT;
public :
  TrayIcon(QWidget *parent);
  ~TrayIcon();

  QAction *hideAction;
  QAction *reloadAction;
  QAction *closeAction;

private :
  QMenu *trayIconMenu;
};

