#include "dialog.h"
#include <QApplication>
#include <databaseclass.h>
#include "serverclass.h"

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);
        Dialog w;
        w.show();
        return a.exec();
    }
    catch(QException &ex)
    {
        //write error in log file
        return -1;
    }
}
