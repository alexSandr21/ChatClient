#ifndef MESSAGE_H
#define MESSAGE_H

#include "emessagetype.h"
#include "userinfo.h"

class Message
{
private:
    MsgType m_MsgType;
    UserInfo m_sUserInfo;
    QString m_UserName;
    QString m_Password;
    QString m_Message;
    QString m_Sender;
    QString m_Receiver;

    void ParseUserInfo(QString strMsg);
    void ParseMessageInfo(QString strMsg);


public:
    Message();

    QString GetUsername();
    QString GetPassword();
    QString GetStringMessage();
    QString GetSender();
    QString GetReceiver();

    friend QDataStream& operator>>(QDataStream& in, Message& msg);

};

#endif // MESSAGE_H
