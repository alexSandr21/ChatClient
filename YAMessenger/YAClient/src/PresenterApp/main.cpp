#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "presenter.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<QFile> logFile(new QFile("YAClient.log"));

    if(logFile->size()>YAClient::MAX_FILE_SIZE)
        logFile->open(QIODevice::WriteOnly);

    else
        logFile->open(QIODevice::WriteOnly|QIODevice::Append);

    try
    {
        QGuiApplication app(argc, argv);

        YAClient::Presenter presenter(logFile);

        QQmlApplicationEngine engine;

        QQmlContext *context = engine.rootContext();

        context->setContextProperty("presenter", &presenter);

        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        return app.exec();
    }
    catch(const std::exception &ex)
    {
       if(logFile->isOpen())
       {
           QByteArray str;
           str.append(QDateTime::currentDateTime().toString());
           str.append(" ");
           str.append(ex.what());
           str.append("\r\n");

           logFile->write(str);
       }

        return -1;
    }
}
