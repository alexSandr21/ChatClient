#include "stdafx.h"
#include "serverclass.h"

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);
        ServerClass server;
        server.StartServer(3004);
        return a.exec();
    }
    catch(const std::exception &ex)
    {
        //write error in log file
        return -1;
    }
}
