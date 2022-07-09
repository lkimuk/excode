#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Highlighter.h"

#include <HighlighterController.h>
#include <HighlightingStyleController.h>
#include <HighlightingStyleModel.h>
#include <LanguageController.h>
#include <LanguageModel.h>
#include <QQmlContext>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<Highlighter>("lkimuk", 1, 0, "Highlighter");
    qmlRegisterType<HighlightingStyleModel>("com.highlighting.style", 1, 0, "HighlightingStyleModel");
    qmlRegisterType<LanguageModel>("com.language.model", 1, 0, "LanguageModel");

    HighlightingStyleController* highlightingStyleController = new HighlightingStyleController(&engine);
    highlightingStyleController->addHighlightingStyle("VSCode");
    highlightingStyleController->addHighlightingStyle("Decco");

    LanguageController* languageController = new LanguageController(&engine);
    languageController->addLanguage("C");
    languageController->addLanguage("C++");

    HighlighterController* highlighterController = new HighlighterController(&engine);

    QQmlContext* context = engine.rootContext();
    context->setContextProperty("highlightingStyleController", highlightingStyleController);
    context->setContextProperty("languageController", languageController);
    context->setContextProperty("highlighterController", highlighterController);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Show all supprted fonts
//    foreach(const QString& family, QFontDatabase::families()) {
//        qDebug() << family;
//    }

    return app.exec();
}
