#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QList>
#include <QQuickTextDocument>
#include <QRegularExpression>
#include <QSyntaxHighlighter>


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* textDocument READ getTextDocument WRITE setTextDocument NOTIFY textDocumentChanged)

public:
    Highlighter(QObject* parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    QQuickTextDocument* getTextDocument() const;
    void setTextDocument(QQuickTextDocument* textDocument);

signals:
    void textDocumentChanged();

private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QQuickTextDocument* m_TextDocument{ nullptr };
};

#endif // HIGHLIGHTER_H
