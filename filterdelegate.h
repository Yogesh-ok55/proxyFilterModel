#ifndef FILTERDELEGATE_H
#define FILTERDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QTableView>

class FilterDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override {
        QLineEdit *editor = new QLineEdit(parent);

        // ✅ Live sync: update model immediately on text change
        connect(editor, &QLineEdit::textChanged, this, [this, editor, index]() {
            QAbstractItemModel *m = const_cast<QAbstractItemModel*>(index.model());
            m->setData(index, editor->text());   // push text into model
            emit const_cast<FilterDelegate*>(this)->filterTextChanged();
        });

        return editor;
    }

signals:
    void filterTextChanged();
};

#endif // FILTERDELEGATE_H
