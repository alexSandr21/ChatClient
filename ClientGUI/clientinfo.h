#pragma once

#include "stdafx.h"

const char DELIM = '|';

//privet

//privet

struct clientInfo
{
    QString name;
    QString secName;
    bool Online;

    friend QDataStream & operator>>(QDataStream &in, clientInfo&obj)
    {
        in>>obj.Online;
        in>>obj.secName;

        obj.name = obj.secName.left(obj.secName.indexOf(DELIM));
        obj.secName.remove(0, obj.name.size()+1);

        return in;
    }
};


