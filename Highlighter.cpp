#include "Highlighter.h"


Highlighter::Highlighter(QObject* parent)
    : QSyntaxHighlighter(parent)
{
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\bchar\\b")
    };

    HighlightingRule rule;
    for (const QString& pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

// This function will be called automatically whenever
// it is necessary by the rich text engine.
void Highlighter::highlightBlock(const QString &text)
{
    // define our own highlighting rules.
    for (const HighlightingRule& rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // setCurrentBlockState(0);
}

QQuickTextDocument *Highlighter::getTextDocument() const
{
    return m_TextDocument;
}

void Highlighter::setTextDocument(QQuickTextDocument *textDocument)
{
    if (textDocument == m_TextDocument) return;

    m_TextDocument = textDocument;
    setDocument(m_TextDocument->textDocument());

    emit textDocumentChanged();
}
