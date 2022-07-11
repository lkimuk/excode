#ifndef HIGHLIGHTERCONTROLLER_H
#define HIGHLIGHTERCONTROLLER_H

#include <QObject>

class AbstractHighlighter;
class QQuickTextDocument;


class HighlighterController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* textDocument READ getTextDocument WRITE setTextDocument NOTIFY textDocumentChanged)

public:
    explicit HighlighterController(QObject *parent = nullptr);

    Q_INVOKABLE void setLanguage(const QString& lang);
    Q_INVOKABLE void setHighlighter(const QString& highlighter);

private:
    QQuickTextDocument* getTextDocument() const;
    void setTextDocument(QQuickTextDocument* textDocument);

signals:
    void textDocumentChanged();

private:
    AbstractHighlighter* m_highlighter;
};

#endif // HIGHLIGHTERCONTROLLER_H
