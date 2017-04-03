#pragma once

#include "stdafx.h"





class ServerClass : public QObject
{
    Q_OBJECT

private:
    QTcpServer* m_pTcpServer;
    DataBaseClass m_dbManager;
    QMap<QString, UserInfo> m_mapClients;
    QTextBrowser* m_ptxtInfo;
    quint32 m_nNextBlockSize;


    void ReadClientREG_LOG(QDataStream& in, QTcpSocket* pClientSocket,const MsgType& typeMsg);
    void ReadClientMESSAGE(QDataStream& in);


    void SendToClient(QTcpSocket* pClientSocket, const MsgType& type, const QByteArray& arrBlockMsg);

public:
    explicit ServerClass(QObject *parent = 0 );


    bool StartServer(int nPort, QTextBrowser* txtInfo);
    QString GetIP();
    void setTxtBrowser(QTextBrowser* txtInfo);

public slots:
    void slotNewConnection();
    void slotReadClient();
    void slotDissconnectClient();
};


