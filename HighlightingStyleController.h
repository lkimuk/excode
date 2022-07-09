#ifndef HIGHLIGHTINGSTYLECONTROLLER_H
#define HIGHLIGHTINGSTYLECONTROLLER_H

#include <QObject>

class HighlightingStyleModel;


class HighlightingStyleController : public QObject
{
    Q_OBJECT
public:
    explicit HighlightingStyleController(QObject *parent = nullptr);

    Q_INVOKABLE HighlightingStyleModel* model() const;

    void addHighlightingStyle(const QString& styleName);

private:
    HighlightingStyleModel* m_model;
};

#endif // HIGHLIGHTINGSTYLECONTROLLER_H
