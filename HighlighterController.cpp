#include "HighlighterController.h"
#include "HighlighterCpp.h"


HighlighterController::HighlighterController(QObject *parent)
    : QObject{parent}
{
    m_highlighter = new HighlighterCpp(this);

    setLanguage("C++");
}

void HighlighterController::setLanguage(const QString& lang)
{
    m_highlighter->setCurrentLanguage(lang);
}

void HighlighterController::setHighlighter(const QString& style)
{
    m_highlighter->setCurrentHighlighter(style);
}

std::vector<QString> HighlighterController::getLanguages() const
{
    return m_highlighter->getLanguages();
}

std::vector<QString> HighlighterController::getHighlighters() const
{
    return m_highlighter->getHighlighters();
}

QQuickTextDocument *HighlighterController::getTextDocument() const
{
    return m_highlighter->getTextDocument();
}

void HighlighterController::setTextDocument(QQuickTextDocument *textDocument)
{
    m_highlighter->setTextDocument(textDocument);

    // change the tab size
    QTextDocument* document = textDocument->textDocument();
    QTextOption op = document->defaultTextOption();
    op.setTabStopDistance(40);
    document->setDefaultTextOption(op);

    emit textDocumentChanged();
}


