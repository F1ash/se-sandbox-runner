#ifndef JOBITEM_MODEL_H
#define JOBITEM_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include "job_item_index.h"
#include <QDebug>

class JobItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit JobItemModel(QObject *parent = 0);
    ~JobItemModel();
    QList<JobItemIndex *>  jobItemDataList;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

signals:

private:
    QIcon  activeIcon;
    QIcon  no_activeIcon;

};

#endif // JOBITEM_MODEL_H
