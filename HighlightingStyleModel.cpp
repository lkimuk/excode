#include "HighlightingStyleModel.h"

HighlightingStyleModel::HighlightingStyleModel(QObject *parent)
    : QAbstractListModel{ parent }
{
}

void HighlightingStyleModel::addHighlightingStyle(const QString &styleName)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_highlightingStyles << styleName;
    endInsertRows();
}

int HighlightingStyleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_highlightingStyles.count();
}

QVariant HighlightingStyleModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_highlightingStyles.count())
        return QVariant();

    if (role == NameRole)
        return m_highlightingStyles[index.row()];

    return QVariant();
}

QHash<int, QByteArray> HighlightingStyleModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
