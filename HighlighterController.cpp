#include "HighlighterController.h"
#include "HighlighterCpp.h"

HighlighterController::HighlighterController(QObject *parent)
    : QObject{parent}
{
    m_highlighter = new HighlighterCpp(this);
}

void HighlighterController::setLanguage(const QString &lang)
{
    if (lang == "C++") {
        m_highlighter = new HighlighterCpp(this);
    } else {
        qDebug() << "Error! do not support language " << lang;
    }
}

void HighlighterController::setHighlighter(const QString &highlighter)
{
    m_highlighter->setHighlighter(highlighter);
}

QQuickTextDocument *HighlighterController::getTextDocument() const
{
    return m_highlighter->getTextDocument();
}

void HighlighterController::setTextDocument(QQuickTextDocument *textDocument)
{
    m_highlighter->setTextDocument(textDocument);

    emit textDocumentChanged();
}


