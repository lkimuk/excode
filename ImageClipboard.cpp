#include "ImageClipboard.h"
#include <QtWidgets/QApplication>
#include <QClipboard>
#include <QQuickItem>
#include <QQuickItemGrabResult>


ImageClipboard::ImageClipboard(QObject *parent)
    : QObject{parent}, m_grabbedImage{nullptr}
{
}

void ImageClipboard::copyToClipboard(QObject *item)
{
    if (item) {
        auto codeArea = qobject_cast<QQuickItem*>(item);
        m_grabbedImage = codeArea->grabToImage(QSize(codeArea->width(), codeArea->height()));
        connect(m_grabbedImage.data(), &QQuickItemGrabResult::ready, this, &ImageClipboard::doCopy);
    }
}

void ImageClipboard::doCopy()
{
    m_grabbedImage->saveToFile("code.png");
    if (m_grabbedImage.data()) {
        auto img = m_grabbedImage->image();
        QApplication::clipboard()->setImage(img, QClipboard::Clipboard);
        qDebug() << "copy succeed";
    }
}
