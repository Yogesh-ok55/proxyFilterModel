#ifndef FILTERPROXYMODEL_H
#define FILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QVector>
#include <QString>

class FilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:

    enum FilterType {
        Contains,
        StartsWith,
        EndsWith,
        ExactMatch
    };

    explicit FilterProxyModel(QObject *parent = nullptr);

    void setFilterForColumn(int column, const QString &pattern,FilterType type);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    struct FilterRule {
        QString text;
        FilterType type;
    };

    QHash<int, FilterRule> columnFilters;
};

#endif // FILTERPROXYMODEL_H
