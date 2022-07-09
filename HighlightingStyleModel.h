#ifndef HIGHLIGHTINGSTYLEMODEL_H
#define HIGHLIGHTINGSTYLEMODEL_H

#include <QAbstractListModel>


class HighlightingStyleModel : public QAbstractListModel
{
public:
    enum HighlightingStyleRoles {
        NameRole = Qt::UserRole + 1
    };
    HighlightingStyleModel(QObject *parent = nullptr);

    void addHighlightingStyle(const QString& styleName);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QString> m_highlightingStyles;
};

#endif // HIGHLIGHTINGSTYLEMODEL_H
