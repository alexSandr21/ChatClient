#include "message.h"

Message::Message()
{

}

void Message::ParseUserInfo(QString strMsg)
{
    int i = 0;
    while(strMsg[i] != '.')
    {
        this->m_UserName+=strMsg[i];
        ++i;
    }
    ++i;
    this->m_Password.append(strMsg.mid(i));
}

void Message::ParseMessageInfo(QString strMsg)
{
    int i = 0;
    while(strMsg[i] != '.')
    {
        this->m_Sender+=strMsg[i];
        ++i;
    }
    ++i;
    while(strMsg[i]!= '.')
    {
        this->m_Receiver+=strMsg[i];
    }
    ++i;
    this->m_Message.append(strMsg.mid(i));
}

 QDataStream& operator>>(QDataStream& in, Message& msg)
 {
     int k;
     in>>k;
     QString tempStr;
     switch (k) {
     case REG:
         in>>msg.m_sUserInfo;
         in>>tempStr;
         msg.ParseUserInfo(tempStr);
         break;
     case LOGIN:
         in>>tempStr;
         msg.ParseUserInfo(tempStr);
         break;
     case MESSAGE:
         in>>tempStr;
         msg.ParseMessageInfo(tempStr);
         break;
     }
 }
