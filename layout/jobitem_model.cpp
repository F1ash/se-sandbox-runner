#include "jobitem_model.h"

JobItemModel::JobItemModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    activeIcon = QIcon::fromTheme("run");
    no_activeIcon = QIcon::fromTheme("job");
}

Qt::ItemFlags JobItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return defaultFlags;
    };

    JobItemIndex *item = static_cast<JobItemIndex *>(index.internalPointer());
    if ( !item ) {
        //qDebug()<<"item not valid";
        return defaultFlags;
    };
    Qt::ItemFlags flags;
    flags = (defaultFlags | Qt::ItemIsEditable);
    return flags;
}
int JobItemModel::rowCount(const QModelIndex &parent) const
{
    return jobItemDataList.count();
}
int JobItemModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}
QVariant JobItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( orientation == Qt::Horizontal ) {
        if ( role == Qt::DisplayRole ) {
            switch (section) {
            case 0:
                return QString("Name");
                break;
            case 1:
                return QString("State");
                break;
            case 2:
                return QString("PID");
                break;
            default:
                break;
            }
        } else if ( role==Qt::TextAlignmentRole ) {
            return Qt::AlignCenter;
        };
    };
    return QAbstractTableModel::headerData( section, orientation, role );
}
QVariant JobItemModel::data(const QModelIndex &index, int role) const
{
    QVariant res;
    if ( role==Qt::DisplayRole ) {
        switch (index.column()) {
            case 0:
                res = jobItemDataList.at(index.row())->getName();
                break;
            case 1:
                res = jobItemDataList.at(index.row())->getState();
                break;
            case 2:
                res = jobItemDataList.at(index.row())->getPID();
                break;
            default:
                break;
        };
    };
    if ( role==Qt::DecorationRole ) {
        switch (index.column()) {
            case 0:
                if ( jobItemDataList.at(index.row())->getState().toLower()=="running" ) {
                    res = activeIcon;
                } else res = no_activeIcon;
                break;
            default:
                break;
        }
    };
    if ( role==Qt::ToolTipRole ) {
        QString s, a;
        switch (index.column()) {
            case 0:
                s.append(QString("Name: %1").arg(jobItemDataList.at(index.row())->getName()));
                a = ( jobItemDataList.at(index.row())->getData()
                      .value("availability").toBool() )? "available":"busy";
                s.append(QString("\nState: %1").arg(a));
                res = s;
                break;
            case 1:
                res = QString("State: %1").arg(jobItemDataList.at(index.row())->getState());
                break;
            case 2:
                res = QString("PID: %1").arg(jobItemDataList.at(index.row())->getPID());
                break;
            default:
                break;
        }
    };
    if ( role==Qt::TextAlignmentRole ) {
        switch (index.column()) {
        case 2:
            res = Qt::AlignCenter;
            break;
        default:
            break;
        }
    };
    //qDebug()<<res<<"data";
    return res;
}
bool JobItemModel::setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole )
{
    if ( !index.isValid() ) {
        //qDebug()<<"index not valid";
        return false;
    };

    if ( role == Qt::EditRole ) {
        switch( index.column() ) {
        case 0:
            jobItemDataList.at(index.row())->setName ( value.toString() );
            break;
        case 1:
            jobItemDataList.at(index.row())->setState ( value.toString() );
            break;
        case 2:
            jobItemDataList.at(index.row())->setPID ( value.toString() );
            break;
        default:
            break;
        };
    };
    emit dataChanged(index.sibling(0,0), index.sibling(rowCount(), columnCount()));
    return true;
}
bool JobItemModel::insertRow(int row)
{
    if (row == -1) row = 0;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    JobItemIndex *newItem = new JobItemIndex;
    jobItemDataList.insert(row, newItem);
    endInsertRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
bool JobItemModel::removeRow(int row)
{
    beginRemoveRows(QModelIndex(), rowCount(), rowCount());
    jobItemDataList.removeAt(row);
    endRemoveRows();
    emit dataChanged(index(0,0), index(0,0).sibling(rowCount(), columnCount()));
    return true;
}
