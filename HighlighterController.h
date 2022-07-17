#ifndef HIGHLIGHTERCONTROLLER_H
#define HIGHLIGHTERCONTROLLER_H

#include <QObject>

class Highlighter;
class QQuickTextDocument;


class HighlighterController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* textDocument READ getTextDocument WRITE setTextDocument NOTIFY textDocumentChanged)

public:
    explicit HighlighterController(QObject *parent = nullptr);

    Q_INVOKABLE void setLanguage(const QString& lang);
    Q_INVOKABLE void setHighlighter(const QString& highlighter);

    std::vector<QString> getLanguages() const;
    std::vector<QString> getHighlighters() const;

private:
    QQuickTextDocument* getTextDocument() const;
    void setTextDocument(QQuickTextDocument* textDocument);

signals:
    void textDocumentChanged();

private:
    Highlighter* m_highlighter;
};

#endif // HIGHLIGHTERCONTROLLER_H
