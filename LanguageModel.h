#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <QAbstractListModel>

class LanguageModel : public QAbstractListModel
{
public:
    enum HighlightingStyleRoles {
        NameRole = Qt::UserRole + 1
    };
    explicit LanguageModel(QObject *parent = nullptr);

    void addLanguage(const QString& lang);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<QString> m_languages;
};

#endif // LANGUAGEMODEL_H
