#include "filterproxymodel.h"

FilterProxyModel::FilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent) {}

void FilterProxyModel::setFilterForColumn(int column, const QString &text,FilterType type) {
    if (text.isEmpty())
        columnFilters.remove(column);
    else
        columnFilters[column] = {text, type};

    invalidateFilter();
}

bool FilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    for (auto it = columnFilters.constBegin(); it != columnFilters.constEnd(); ++it) {
        int col = it.key();
        const FilterRule &rule = it.value();

        QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
        QString data = sourceModel()->data(idx).toString();

        switch (rule.type) {
        case Contains:
            if (!data.contains(rule.text, Qt::CaseInsensitive))
                return false;
            break;
        case StartsWith:
            if (!data.startsWith(rule.text, Qt::CaseInsensitive))
                return false;
            break;
        case EndsWith:
            if (!data.endsWith(rule.text, Qt::CaseInsensitive))
                return false;
            break;
        case ExactMatch:
            if (data.compare(rule.text, Qt::CaseInsensitive) != 0)
                return false;
            break;
        }
    }
    return true; // row accepted
}
