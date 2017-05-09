#include "stdafx.h"

#include "server.h"

const int MAX_FILE_SIZE = 30000000;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<QFile> file(new QFile("server.log"));
    if(file->size()<MAX_FILE_SIZE)
    {
       file->open(QIODevice::WriteOnly | QIODevice::Append);
    }
    else
    {
        file->open(QIODevice::WriteOnly);
    }

//    YAServer::YAServer server(file);
//    server.StartServer(3004);
    try
    {
        YAServer::Server server(file);
        server.StartServer(3004);
        return a.exec();
    }
    catch(const std::exception &ex)
    {
       if(file->isOpen())
       {
           QByteArray arr;
           arr.append(QDateTime::currentDateTime().toString());
           arr.append(" " + QString(ex.what()) + "\r\n");

           file->write(arr);
       }
       return -1;
    }


}
