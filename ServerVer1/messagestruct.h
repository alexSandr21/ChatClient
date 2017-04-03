#ifndef MESSAGESTRUCT_H
#define MESSAGESTRUCT_H

#include "emessagetype.h"
#include "userinfo.h"

struct Message
{
    MsgType m_MsgType;
    UserInfo m_sUserInfo;
    QString m_UserName;
    QString m_Password;
    QString m_Message;
    QString m_Sender;
    QString m_Receiver;

    Message()
    {

    }
};

#endif // MESSAGESTRUCT_H
