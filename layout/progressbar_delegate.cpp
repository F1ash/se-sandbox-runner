#include "progressbar_delegate.h"

ProgressBarDelegate::ProgressBarDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}
void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 1) {
        int progress = 0;
        bool ok = false;
        QStringList _data = index.data().toString().split("|");
        QString State = _data.first();
        if ( _data.count()>1 ) {
            _data.at(1).toInt(&ok);
            if ( ok ) {
                progress = _data.at(1).toInt();
            }
        };

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = State;
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(
                    QStyle::CE_ProgressBar,
                    &progressBarOption, painter);
    } else
        QStyledItemDelegate::paint(painter, option, index);
}
