#include "AbstractHighlighter.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


AbstractHighlighter::AbstractHighlighter(QObject *parent)
    : QSyntaxHighlighter{parent}
{
    initializeHighlighters();
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
    std::sort(results.begin(), results.end());

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

    setGeneralRules(m_currentHighlighter.textColor, ".*");
    setGeneralRules(m_currentHighlighter.functionColor, m_currentLanguage.syntax.functionPattern);
    setGeneralRules(m_currentHighlighter.attributesColor, m_currentLanguage.syntax.attributesPattern);
    setGeneralRules(m_currentHighlighter.numberColor, m_currentLanguage.syntax.numberPattern);

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

    setGeneralRules(m_currentHighlighter.quotationColor, m_currentLanguage.syntax.quotationPattern);
    setGeneralRules(m_currentHighlighter.includeColor, m_currentLanguage.syntax.includePattern);
    setGeneralRules(m_currentHighlighter.commentColor, m_currentLanguage.syntax.singlelineCommentPattern);

    // flush the highligher to take the changed style.
    rehighlight();
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

void AbstractHighlighter::initializeHighlighters()
{
    // qDebug() << "current path: " << QDir::currentPath();

    // 1. Read the language configuration file
    QFile file("config/config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "failed to open language configuration file!";
        return;
    }

    QTextStream textStream(&file);
    QString data = textStream.readAll();
    file.close();

    // 2. Get json object
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toLocal8Bit());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "failed to create json doc";
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.isEmpty()) {
        qDebug() << "json object is empty";
        return;
    }

    // 3. Parse the Languages data
    language_t lang;
    QJsonArray languages = jsonObj["Languages"].toArray();
    for (const QJsonValue& value : languages) {
        QJsonObject language = value.toObject();
        lang.name = language["name"].toString();

        QJsonObject patterns = language["patterns"].toObject();
        lang.syntax.attributesPattern = patterns["attribute"].toString();
        lang.syntax.functionPattern   = patterns["function"].toString();
        lang.syntax.includePattern    = patterns["include"].toString();
        lang.syntax.numberPattern     = patterns["number"].toString();
        lang.syntax.quotationPattern  = patterns["quotation"].toString();

        QJsonArray keywords = patterns["keywords"].toArray();
        for (const auto& keyword : keywords) {
            lang.keywords.push_back(keyword.toString());
        }

        QJsonArray operators = patterns["operators"].toArray();
        for (const auto& optor : operators) {
            lang.operators.push_back(optor.toString());
        }

        QJsonObject comment = patterns["comment"].toObject();
        lang.syntax.singlelineCommentPattern = comment["single-line"].toString();
        QJsonObject multiLineComment = comment["multi-line"].toObject();
        auto& [start, end] = lang.syntax.multilineCommentPattern;
        start = multiLineComment["start"].toString();
        end = multiLineComment["end"].toString();

        appendLanguages(lang);
    }

    // 4. Parse the Highlighters data
    highlighter_t style;
    QJsonArray highlighters = jsonObj["Highlighters"].toArray();
    for (const QJsonValue& value : highlighters) {
        QJsonObject highlighter = value.toObject();
        style.name = highlighter["name"].toString();

        QJsonObject colors = highlighter["colors"].toObject();
        style.attributesColor = colors["attributes"].toString();
        style.commentColor    = colors["comment"].toString();
        style.functionColor   = colors["function"].toString();
        style.includeColor    = colors["include"].toString();
        style.keywordsColor   = colors["keywords"].toString();
        style.numberColor     = colors["number"].toString();
        style.operatorColor   = colors["operators"].toString();
        style.quotationColor  = colors["quotation"].toString();
        style.textColor       = colors["text"].toString();

        appendHighlighters(style);
    }
}

void AbstractHighlighter::appendLanguages(const language_t &lang)
{
    m_languages[lang.name] = [this, lang]() {
        m_currentLanguage.name                            = lang.name;
        m_currentLanguage.keywords                        = lang.keywords;
        m_currentLanguage.operators                       = lang.operators;
        m_currentLanguage.syntax.attributesPattern        = lang.syntax.attributesPattern;
        m_currentLanguage.syntax.functionPattern          = lang.syntax.functionPattern;
        m_currentLanguage.syntax.includePattern           = lang.syntax.includePattern;
        m_currentLanguage.syntax.numberPattern            = lang.syntax.numberPattern;
        m_currentLanguage.syntax.quotationPattern         = lang.syntax.quotationPattern;
        m_currentLanguage.syntax.singlelineCommentPattern = lang.syntax.singlelineCommentPattern;
        m_currentLanguage.syntax.multilineCommentPattern  = lang.syntax.multilineCommentPattern;

        updateStyle();
    };
}

void AbstractHighlighter::appendHighlighters(const highlighter_t &style)
{
    m_highlighters[style.name] = [this, style]() {
        m_currentHighlighter.name            = style.name;
        m_currentHighlighter.attributesColor = style.attributesColor;
        m_currentHighlighter.commentColor    = style.commentColor;
        m_currentHighlighter.functionColor   = style.functionColor;
        m_currentHighlighter.includeColor    = style.includeColor;
        m_currentHighlighter.keywordsColor   = style.keywordsColor;
        m_currentHighlighter.numberColor     = style.numberColor;
        m_currentHighlighter.operatorColor   = style.operatorColor;
        m_currentHighlighter.quotationColor  = style.quotationColor;
        m_currentHighlighter.textColor       = style.textColor;

        updateStyle();
    };
}
