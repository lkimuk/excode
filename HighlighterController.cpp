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

    // I think this post can get you in trouble. I have found that with large strings
    // it seems like the std string can end up a dangling pointer before it gets converted
    // to a cstring. This is even one of those bugs that will show up in production code but
    // not when running from the IDE. Here is a post about it:
    // https://stackoverflow.com/questions/6208565/const-char-vs-char-c and here is my fix:
    m_highlighter->setCurrentLanguage(lang);
}

void HighlighterController::setHighlighter(const QString& style)
{
    m_highlighter->setCurrentHighlighter(style);
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


