#ifndef ABSTRACTHIGHLIGHTER_H
#define ABSTRACTHIGHLIGHTER_H

#include <QQuickTextDocument>
#include <QRegularExpression>
#include <QSyntaxHighlighter>



struct syntax_patterns_t {
    QString quotationPattern;
    QString functionPattern;
    QString singlelineCommentPattern;
    std::pair<QString, QString> multilineCommentPattern;

    QString attributesPattern; // new
    QString includePattern;   // new
    QString numberPattern; // new
};

struct language_t {
    QString name;
    std::vector<QString> keywords;
    std::vector<QString> operators;
    syntax_patterns_t syntax;
};

struct highlighter_t {
    QString name;
    QString attributesColor; // new
    QString commentColor;
    QString functionColor;
    QString includeColor; // new
    QString keywordsColor;
    QString numberColor; // new
    QString operatorColor; // new
    QString quotationColor;
    QString textColor; // new

};


class AbstractHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    using assoc_type = std::unordered_map<QString, std::function<void()>>;

public:
    explicit AbstractHighlighter(QObject *parent = nullptr);

    void setCurrentLanguage(const QString& lang);
    void setCurrentHighlighter(const QString& style);

    QQuickTextDocument* getTextDocument() const;
    void setTextDocument(QQuickTextDocument* textDocument);

private:
    void updateStyle();
    void setGeneralRules(const QString& color, const QString& pattern);
    void setMultiLineCommentRules(const QColor& color, const QString& startPattern, const QString& endPattern);

protected:
    void highlightBlock(const QString &text) override;

    void appendLanguages(const language_t& lang);
    void appendHighlighters(const highlighter_t& style);

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> m_highlightingRules;
    QQuickTextDocument* m_textDocument{ nullptr };
    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;
    QTextCharFormat m_multiLineCommentFormat;

    assoc_type m_languages;
    assoc_type m_highlighters;
    language_t m_currentLanguage;
    highlighter_t m_currentHighlighter;
};

#endif // ABSTRACTHIGHLIGHTER_H
