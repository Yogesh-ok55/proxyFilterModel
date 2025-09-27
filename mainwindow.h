#ifndef SEARCHABLETABLE_H
#define SEARCHABLETABLE_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QShortcut>
#include "filterproxymodel.h"

class SearchableTable : public QWidget {
    Q_OBJECT
public:
    explicit SearchableTable(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QTableView *table;
    QStandardItemModel *model;
    FilterProxyModel *proxy;
    bool filterRowVisible = false;

    void addFilterRow();
    void removeFilterRow();
    void applyFilter();
};

#endif // SEARCHABLETABLE_H
