#ifndef HIGHLIGHTERCPP_H
#define HIGHLIGHTERCPP_H

#include "AbstractHighlighter.h"


class HighlighterCpp : public AbstractHighlighter
{
    Q_OBJECT

public:
    explicit HighlighterCpp(QObject *parent = nullptr);

    void setHighlighter(const QString& highlighter) override;
};

#endif // HIGHLIGHTERCPP_H
