#ifndef ABSTRACTHIGHLIGHTER_H
#define ABSTRACTHIGHLIGHTER_H

#include <QQuickTextDocument>
#include <QRegularExpression>
#include <QSyntaxHighlighter>


class AbstractHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit AbstractHighlighter(QObject *parent = nullptr);

    // set current highter
    virtual void setHighlighter(const QString& highlighter) = 0;

    QQuickTextDocument* getTextDocument() const;
    void setTextDocument(QQuickTextDocument* textDocument);

protected:
    void highlightBlock(const QString &text) override;

    // clear all highter rules
    void clearHighlightingRules();

    // register highter rules
    void registerHighlighterRules(const QColor& color, const char* pattern);

    // setMultiline
    void setMultiLineCommentRules(const QColor& color, const char* startPattern, const char* endPattern);

    // clear keywords
    void clearKeywords();

    // append keyword
    void registerKeywords(std::initializer_list<QString> keywords);

    const std::vector<QString>& keywords() const;

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> m_highlightingRules;
    QQuickTextDocument* m_textDocument{ nullptr };
    std::vector<QString> m_keywords;
    QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;
    QTextCharFormat m_multiLineCommentFormat;
};

#endif // ABSTRACTHIGHLIGHTER_H
