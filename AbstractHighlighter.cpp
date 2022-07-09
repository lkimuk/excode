#include "AbstractHighlighter.h"

AbstractHighlighter::AbstractHighlighter(QObject *parent)
    : QSyntaxHighlighter{parent}
{
}

QQuickTextDocument* AbstractHighlighter::getTextDocument() const
{
    return m_textDocument;
}

void AbstractHighlighter::setTextDocument(QQuickTextDocument *textDocument)
{
    if (textDocument == m_textDocument)
        return;

    m_textDocument = textDocument;
    setDocument(m_textDocument->textDocument());
}

// This function will be called automatically whenever
// it is necessary by the rich text engine.
void AbstractHighlighter::highlightBlock(const QString &text)
{
    // define our own highlighting rules.
    for (const HighlightingRule& rule : qAsConst(m_highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(m_commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = m_commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, m_multiLineCommentFormat);
        startIndex = text.indexOf(m_commentStartExpression, startIndex + commentLength);
    }
}

void AbstractHighlighter::clearHighlightingRules()
{
    m_highlightingRules.clear();
}

void AbstractHighlighter::registerHighlighterRules(const QColor &color, const char *pattern)
{
    QTextCharFormat syntaxFormat;
    HighlightingRule rule;

    syntaxFormat.setForeground(color);
    rule.format = syntaxFormat;
    rule.pattern = QRegularExpression(pattern);
    m_highlightingRules.append(rule);
}

void AbstractHighlighter::setMultiLineCommentRules(const QColor &color, const char *startPattern, const char *endPattern)
{
    m_multiLineCommentFormat.setForeground(color);
    m_commentStartExpression = QRegularExpression(startPattern);
    m_commentEndExpression = QRegularExpression(endPattern);
}

void AbstractHighlighter::clearKeywords()
{
    m_keywords.clear();
}

void AbstractHighlighter::registerKeywords(std::initializer_list<QString> keywords)
{
    for (const auto& elem : keywords)
        m_keywords.push_back("\\b" + elem + "\\b");
}

const std::vector<QString> &AbstractHighlighter::keywords() const
{
    return m_keywords;
}
