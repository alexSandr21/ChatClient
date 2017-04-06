#pragma once

#include "stdafx.h"

struct CustomDeleter
{
    static inline void cleanup(QFile *file)
    {
        if(file->isOpen())
            file->close();

        delete file;
    }
};

