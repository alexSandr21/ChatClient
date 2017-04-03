#pragma once
#include <QtNetwork>



struct UserInfo {

    bool isNotFirstContact;
    QString name;
    QString surName;
    QTcpSocket* pClientSocket;


    UserInfo():name(""), surName(""), pClientSocket(nullptr)
    {
        isNotFirstContact = false;
    }

    friend QDataStream& operator>>(QDataStream& in, UserInfo& u)
    {
        in>>u.name>>u.surName;
        return in;
    }
    friend QDataStream& operator<<(QDataStream& out, const UserInfo& u)
    {
        if(u.pClientSocket)
            out<<true;
        else
            out<<false;
        QString str(u.name + '.' + u.surName);
        out<<str;
        return out;
    }
};


