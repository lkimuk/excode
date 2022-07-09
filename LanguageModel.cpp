#include "LanguageModel.h"

LanguageModel::LanguageModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void LanguageModel::addLanguage(const QString &lang)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_languages << lang;
    endInsertRows();
}

int LanguageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_languages.count();
}

QVariant LanguageModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_languages.count())
        return QVariant();

    if (role == NameRole)
        return m_languages[index.row()];

    return QVariant();
}

QHash<int, QByteArray> LanguageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}
