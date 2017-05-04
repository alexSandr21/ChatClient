#include <QApplication>
#include "yaserver.h"
#include <QFile>
#include <memory>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<QFile> file(new QFile("server.log"));
    file->open(QIODevice::WriteOnly);
    YAServer::YAServer server(file);
    server.StartServer(3004);
    return a.exec();
}
