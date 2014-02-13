#include "wait_thread.h"

Wait::Wait(QObject *parent = 0) :
    QThread(parent)
{
    to_kill = true;
}
Wait::~Wait()
{

}
void Wait::run()
{
    while (wdg->jobItemModel->jobItemDataList.count()) {
        int count = wdg->jobItemModel->jobItemDataList.count();
        QList<QString> to_Delete;
        for (int i=0; i<count; i++) {
            JobItemIndex *idx = wdg->jobItemModel->jobItemDataList.at(i);
            DATA map = idx->getData();
            if ( map.value("availability").toBool() && !map.value("isRunning").toBool() ) {
                to_Delete.append(idx->getName());
            } else if ( map.value("availability").toBool() && map.value("isRunning").toBool() ) {
                if ( to_kill ) wdg->jobProcess->value(idx->getName())->killJob();
                else wdg->jobProcess->value(idx->getName())->undockJob();
            };
        };
        JobItemIndex *idx;
        QList<QString>::const_iterator j;
        for (j=to_Delete.constBegin(); j!=to_Delete.constEnd(); j++) {
            int count = wdg->jobItemModel->rowCount();
            bool exist = false;;
            for (int i=0; i<count; i++) {
                idx = wdg->jobItemModel->jobItemDataList.at(i);
                if ( idx->getName()==*j ) {
                    exist = true;
                    break;
                }
            };
            int row = wdg->jobItemModel->jobItemDataList.indexOf(idx);
            if (exist) wdg->jobItemModel->removeRow(row);
        };
        msleep(500);
    };
    msleep(1000);
}
void Wait::setWdgReference(JobList *w)
{
  wdg = w;
}
void Wait::setMode(bool mode)
{
    to_kill = mode;
}
