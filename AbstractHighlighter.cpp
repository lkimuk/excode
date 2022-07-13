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

std::vector<QString> AbstractHighlighter::getLanguages() const
{
    std::vector<QString> results;
    for (const auto& [key, value] : m_languages) {
        results.push_back(key);
    }
    return results;
}

std::vector<QString> AbstractHighlighter::getHighlighters() const
{
    std::vector<QString> results;
    for (const auto& [key, value] : m_highlighters) {
        results.push_back(key);
    }
    return results;
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

    setGeneralRules(m_currentHighlighter.functionColor, m_currentLanguage.syntax.functionPattern);
    setGeneralRules(m_currentHighlighter.quotationColor, m_currentLanguage.syntax.quotationPattern);
    setGeneralRules(m_currentHighlighter.commentColor, m_currentLanguage.syntax.singlelineCommentPattern);
    setGeneralRules(m_currentHighlighter.attributesColor, m_currentLanguage.syntax.attributesPattern);
    setGeneralRules(m_currentHighlighter.includeColor, m_currentLanguage.syntax.includePattern);
    setGeneralRules(m_currentHighlighter.numberColor, m_currentLanguage.syntax.numberPattern);
    // setGeneralRules(m_currentHighlighter.testColor, m_currentLanguage.syntax.testPattern);

    const auto& [start, end] = m_currentLanguage.syntax.multilineCommentPattern;
    setMultiLineCommentRules(m_currentHighlighter.commentColor, start, end);

    for (const auto& pattern : m_currentLanguage.keywords) {
        setGeneralRules(m_currentHighlighter.keywordsColor, "\\b" + pattern + "\\b");
    }

    for (auto& pattern : m_currentLanguage.operators) {
        escape(pattern); // eliminate escape characters
        // qDebug() << "operator: " << pattern;
        setGeneralRules(m_currentHighlighter.operatorColor, pattern);
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

void AbstractHighlighter::escape(QString &character)
{
    character.replace("+", "\\+");
    character.replace("^", "\\^");
    character.replace("$", "\\$");
    character.replace(".", "\\.");
    character.replace("[", "\\[");
    character.replace("]", "\\]");
    character.replace("(", "\\(");
    character.replace(")", "\\)");
    character.replace("*", "\\*");
    character.replace("?", "\\?");
    character.replace("|", "\\|");
    character.replace("\\\\", "\\");
}

void AbstractHighlighter::appendLanguages(const language_t &lang)
{
    m_languages[lang.name] = [this, lang]() {
        m_currentLanguage.name = lang.name;
        m_currentLanguage.keywords = lang.keywords;
        m_currentLanguage.operators = lang.operators;
        m_currentLanguage.syntax.functionPattern = lang.syntax.functionPattern;
        m_currentLanguage.syntax.quotationPattern = lang.syntax.quotationPattern;
        m_currentLanguage.syntax.singlelineCommentPattern = lang.syntax.singlelineCommentPattern;
        m_currentLanguage.syntax.multilineCommentPattern = lang.syntax.multilineCommentPattern;

        m_currentLanguage.syntax.attributesPattern = lang.syntax.attributesPattern;
        m_currentLanguage.syntax.includePattern = lang.syntax.includePattern;
        m_currentLanguage.syntax.numberPattern = lang.syntax.numberPattern;

        // m_currentLanguage.syntax.testPattern = lang.syntax.testPattern;

        updateStyle();
    };
}

void AbstractHighlighter::appendHighlighters(const highlighter_t &style)
{
    m_highlighters[style.name] = [this, style]() {
        m_currentHighlighter.name = style.name;
        m_currentHighlighter.keywordsColor = style.keywordsColor;
        m_currentHighlighter.functionColor = style.functionColor;
        m_currentHighlighter.quotationColor = style.quotationColor;
        m_currentHighlighter.commentColor = style.commentColor;

        m_currentHighlighter.attributesColor = style.attributesColor;
        m_currentHighlighter.includeColor = style.includeColor;
        m_currentHighlighter.numberColor = style.numberColor;
        m_currentHighlighter.operatorColor = style.operatorColor;

        // m_currentHighlighter.testColor = style.testColor;

        updateStyle();
    };
}
