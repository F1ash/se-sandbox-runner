#include "job_item_index.h"

JobItemIndex::JobItemIndex(QObject *parent) :
    QObject(parent)
{
    Name = "";
    State = "";
    PID = "";
    Data = DATA();
}
void JobItemIndex::setName(const QString &s)
{
    Name = s;
}
void JobItemIndex::setState(const QString &s)
{
    State = s;
}
void JobItemIndex::setPID(const QString &s)
{
    PID = s;
}
void JobItemIndex::setData(const DATA &_data)
{
    Data = _data;
}
QString JobItemIndex::getName() const
{
    return Name;
}
QString JobItemIndex::getState() const
{
    return State;
}
QString JobItemIndex::getPID() const
{
    return PID;
}
DATA JobItemIndex::getData() const
{
    return Data;
}
