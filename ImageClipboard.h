#ifndef IMAGECLIPBOARD_H
#define IMAGECLIPBOARD_H

#include <QObject>
#include <QSharedPointer>


class QQuickItemGrabResult;


class ImageClipboard : public QObject
{
    Q_OBJECT
public:
    explicit ImageClipboard(QObject *parent = nullptr);

    Q_INVOKABLE void copyToClipboard(QObject* item);

private:
    void doCopy();

private:
    QSharedPointer<QQuickItemGrabResult> m_grabbedImage;
};

#endif // IMAGECLIPBOARD_H
