#ifndef CUSTOMDELETER_H
#define CUSTOMDELETER_H

#include <QFile>

struct CustomDeleter
{
    static inline void cleanup(QFile *file)
    {
        if(file->isOpen())
            file->close();

        delete file;
    }
};

#endif // CUSTOMDELETER_H