#include "wait_thread.h"

Wait::Wait(QObject *parent = 0) :
    QThread(parent)
{

}
Wait::~Wait()
{

}
void Wait::run()
{
  while (wdg->count())
    {
      int count = wdg->count();
      QList<int> to_Delete;
      for (int i=0; i<count; i++)
        {
          QListWidgetItem *_item;
          _item = wdg->item(i);
          QMap<QString, QVariant> map = _item->data(Qt::UserRole).toMap();
          if ( map.value("availability").toBool() && !map.value("isRunning").toBool() )
            to_Delete.append(i);
          else if ( map.value("availability").toBool() && map.value("isRunning").toBool() )
            {
              wdg->jobProcess->value(_item->text())->killJob();
            };
        };
      QList<int>::const_iterator j;
      for (j=to_Delete.constBegin(); j!=to_Delete.constEnd(); j++)
        wdg->takeItem(*j);
      msleep(500);
    };
  msleep(1000);
}
void Wait::setWdgReference(JobList *w)
{
  wdg = w;
}
