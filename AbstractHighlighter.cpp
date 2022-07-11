#include "AbstractHighlighter.h"


AbstractHighlighter::AbstractHighlighter(QObject *parent)
    : QSyntaxHighlighter{parent}
{
}

void AbstractHighlighter::setCurrentLanguage(const QString& lang)
{
    if (m_languages.find(lang) != m_languages.end()) {
        m_languages[lang]();
    }
}

void AbstractHighlighter::setCurrentHighlighter(const QString& style)
{
    if (m_highlighters.find(style) != m_highlighters.end()) {
        m_highlighters[style]();
    }
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

void AbstractHighlighter::updateStyle()
{
    m_highlightingRules.clear();

    setGeneralRules(m_currentHighlighter.classColor, m_currentLanguage.syntax.classPattern);
    setGeneralRules(m_currentHighlighter.functionColor, m_currentLanguage.syntax.functionPattern);
    setGeneralRules(m_currentHighlighter.quotationColor, m_currentLanguage.syntax.quotationPattern);
    setGeneralRules(m_currentHighlighter.singlelineCommentColor, m_currentLanguage.syntax.singlelineCommentPattern);
    const auto& [start, end] = m_currentLanguage.syntax.multilineCommentPattern;
    setMultiLineCommentRules(m_currentHighlighter.multilineCommentColor, start, end);

    for (const auto& pattern : m_currentLanguage.keywords) {
        setGeneralRules(m_currentHighlighter.keywordsColor, "\\b" + pattern + "\\b");
    }
}

void AbstractHighlighter::setGeneralRules(const QString& color, const QString& pattern)
{
    QTextCharFormat syntaxFormat;
    HighlightingRule rule;

    syntaxFormat.setForeground(QColor(color));
    rule.format = syntaxFormat;
    rule.pattern = QRegularExpression(pattern);
    m_highlightingRules.append(rule);
}

void AbstractHighlighter::setMultiLineCommentRules(const QColor &color, const QString& startPattern, const QString& endPattern)
{
    m_multiLineCommentFormat.setForeground(color);
    m_commentStartExpression = QRegularExpression(startPattern);
    m_commentEndExpression = QRegularExpression(endPattern);
}

void AbstractHighlighter::appendLanguages(const language_t &lang)
{
    m_languages[lang.name] = [this, lang]() {
        m_currentLanguage.name = lang.name;
        m_currentLanguage.keywords = lang.keywords;
        m_currentLanguage.syntax.classPattern = lang.syntax.classPattern;
        m_currentLanguage.syntax.functionPattern = lang.syntax.functionPattern;
        m_currentLanguage.syntax.quotationPattern = lang.syntax.quotationPattern;
        m_currentLanguage.syntax.singlelineCommentPattern = lang.syntax.singlelineCommentPattern;
        m_currentLanguage.syntax.multilineCommentPattern = lang.syntax.multilineCommentPattern;

        updateStyle();
    };
}

void AbstractHighlighter::appendHighlighters(const highlighter_t &style)
{
    m_highlighters[style.name] = [this, style]() {
        m_currentHighlighter.name = style.name;
        m_currentHighlighter.keywordsColor = style.keywordsColor;
        m_currentHighlighter.classColor = style.classColor;
        m_currentHighlighter.functionColor = style.functionColor;
        m_currentHighlighter.quotationColor = style.quotationColor;
        m_currentHighlighter.singlelineCommentColor = style.singlelineCommentColor;
        m_currentHighlighter.multilineCommentColor = style.multilineCommentColor;

        updateStyle();
    };
}
