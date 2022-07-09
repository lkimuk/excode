#ifndef LANGUAGECONTROLLER_H
#define LANGUAGECONTROLLER_H

#include <QObject>

class LanguageModel;

class LanguageController : public QObject
{
    Q_OBJECT
public:
    explicit LanguageController(QObject *parent = nullptr);

    Q_INVOKABLE LanguageModel* model() const;

    void addLanguage(const QString& lang);

private:
    LanguageModel* m_model;
};

#endif // LANGUAGECONTROLLER_H
