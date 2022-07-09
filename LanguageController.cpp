#include "LanguageController.h"
#include <LanguageModel.h>

LanguageController::LanguageController(QObject *parent)
    : QObject{parent}
{
    m_model = new LanguageModel();
}

LanguageModel *LanguageController::model() const
{
    return m_model;
}

void LanguageController::addLanguage(const QString &lang)
{
    m_model->addLanguage(lang);
}


