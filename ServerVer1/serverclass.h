#pragma once

#include "databaseclass.h"
#include "dialog.h"
#include "ui_dialog.h"

class ServerClass : public QObject
{
    Q_OBJECT

private:
    QTcpServer* m_pTcpServer;
    DataBaseClass m_dbManager;
    QMap<QString, UserInfo> m_mapClients;
    Dialog m_dialog;
    quint32 m_nNextBlockSize;


    void ReadClientREG_LOG(QDataStream& in, QTcpSocket* pClientSocket,const int& typeMsg);
    void ReadClientMESSAGE(QDataStream& in, const int &typeMsg);


    void SendToClient(QTcpSocket* pClientSocket, const int& type, const QByteArray& arrBlockMsg);

public:
    explicit ServerClass(QObject *parent = 0 );


    bool StartServer(int nPort);
    QString GetIP();

public slots:
    void slotNewConnection();
    void slotReadClient();
    void slotDissconnectClient();
};


