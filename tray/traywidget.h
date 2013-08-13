
#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT;
public :
  TrayIcon(QWidget *parent);
  ~TrayIcon();

  QAction *hideAction;
  QAction *closeAction;

private :
  QMenu *trayIconMenu;
};

