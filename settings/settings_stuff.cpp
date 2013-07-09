#include "settings_stuff.h"

SettingsStuff::SettingsStuff(QObject *parent=0) :
    QSettings(parent)
{
  setObjectName(applicationName());
}
SettingsStuff::~SettingsStuff()
{
  ;
}
