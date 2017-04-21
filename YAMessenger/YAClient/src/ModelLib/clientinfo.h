#pragma once

#include <QDataStream>
#include <QString>

namespace YAClient
{
enum {L_REG, L_LOG, L_MESS, L_OK, L_ERROR, L_NEWCLIENT, L_LOGINEXIST, L_FILE};
const char DELIM = '|';

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

}
