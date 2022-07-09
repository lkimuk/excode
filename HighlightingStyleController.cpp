#include "HighlightingStyleController.h"
#include <HighlightingStyleModel.h>


HighlightingStyleController::HighlightingStyleController(QObject *parent)
    : QObject{parent}
{
    m_model = new HighlightingStyleModel();
}

HighlightingStyleModel *HighlightingStyleController::model() const
{
    return m_model;
}

void HighlightingStyleController::addHighlightingStyle(const QString &styleName)
{
    m_model->addHighlightingStyle(styleName);
}
