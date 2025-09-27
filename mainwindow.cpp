#include "mainwindow.h"
#include "filterdelegate.h"
#include <QVBoxLayout>
#include <QStandardItem>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QStyledItemDelegate>


SearchableTable::SearchableTable(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);

    // Data model
    model = new QStandardItemModel(10, 3, this);
    model->setHorizontalHeaderLabels({"Name", "City", "Job"});
    for (int r = 0; r < 10; r++) {
        model->setItem(r, 0, new QStandardItem(QString("Person %1").arg(r)));
        model->setItem(r, 1, new QStandardItem(QString("City %1").arg(r % 3)));
        model->setItem(r, 2, new QStandardItem(QString("Job %1").arg(r % 2)));
    }

    // Proxy
    proxy = new FilterProxyModel(this);
    proxy->setSourceModel(model);

    // Table
    table = new QTableView(this);
    table->setModel(proxy);
    auto *delegate = new FilterDelegate(this);
    table->setItemDelegate(delegate);
    table->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(table);

    connect(delegate, &FilterDelegate::filterTextChanged, this, [=]() {
        if (filterRowVisible) applyFilter();
    });

    // Ctrl+F to toggle filter row
    auto *shortcut = new QShortcut(QKeySequence::Find, this);
    connect(shortcut, &QShortcut::activated, this, [=]() {
        if (!filterRowVisible)
            addFilterRow();
        else
            removeFilterRow();
    });

    // Event filter to catch key presses
    table->installEventFilter(this);
}

void SearchableTable::addFilterRow() {
    model->insertRow(0);
    filterRowVisible = true;
    table->selectRow(0); // focus first row
}

void SearchableTable::removeFilterRow() {
    if (filterRowVisible) {
        model->removeRow(0);
        filterRowVisible = false;
        for (int c = 0; c < model->columnCount(); ++c) {
            proxy->setFilterForColumn(c, "",FilterProxyModel::StartsWith);
        }
        proxy->invalidate(); // reset filters
    }
}

void SearchableTable::applyFilter() {
    if (!filterRowVisible) return;

    for (int c = 0; c < model->columnCount(); ++c) {
        QModelIndex idx = model->index(0, c);
        QString text = model->data(idx).toString();
        proxy->setFilterForColumn(c, text, FilterProxyModel::StartsWith);
    }
}

bool SearchableTable::eventFilter(QObject *obj, QEvent *event) {
    if (obj == table && event->type() == QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_Escape) {
            removeFilterRow();   // Esc hides the filter row
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
