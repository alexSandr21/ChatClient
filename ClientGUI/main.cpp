#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "presenter.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);


    Presenter presenter;

    QQmlApplicationEngine engine;

    QQmlContext *context = engine.rootContext();

    context->setContextProperty("presenter", &presenter);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}
