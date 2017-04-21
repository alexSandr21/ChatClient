#pragma once

#include <QTcpSocket>

namespace ClientInfo{

const char DELIM = '|';

struct ClientInfo {

    QString name;
    QString surName;
    QTcpSocket* pClientSocket;


    friend QDataStream& operator>>(QDataStream& in, ClientInfo& u)
    {
        in>>u.name>>u.surName;
        return in;
    }
    friend QDataStream& operator<<(QDataStream& out, const ClientInfo& u)
    {
        if(u.pClientSocket)
            out<<true;
        else
            out<<false;
        QString str(u.name + DELIM + u.surName);
        out<<str;
        return out;
    }
};

}

