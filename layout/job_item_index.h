#ifndef JOB_ITEM_INDEX_H
#define JOB_ITEM_INDEX_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QVariant>

typedef QMap<QString, QVariant> DATA;

class JobItemIndex : public QObject
{
    Q_OBJECT
public:
    explicit JobItemIndex(QObject *parent = 0);

signals:

private:
    QString  Name;
    QString  State;
    QString  PID;
    DATA     Data;

public slots:
    void setName(const QString&);
    void setState(const QString&);
    void setPID(const QString&);
    void setData(const DATA&);
    QString getName() const;
    QString getState() const;
    QString getPID() const;
    DATA    getData() const;

};

#endif // JOB_ITEM_INDEX_H
