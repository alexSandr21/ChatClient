#pragma once

#include <QTcpServer>
#include <QSslKey>
#include <memory>

#include "databasemanager.h"
#include "serverdialog.h"

namespace YAServer{

enum
{
    REG,
    LOGIN,
    MESSAGE,
    OK,
    LOG_ERROR,
    UPDATE,
    REG_ERROR,
    L_FILE
};

class YAServer : public QObject
{
    Q_OBJECT
public:
    explicit YAServer(std::shared_ptr<QFile> t_pLog = nullptr, QObject *parent = 0);

    bool StartServer(int nPort);
    QString GetIP();

public slots:
    void slotNewConnection();
    void slotReadClient();
    void slotDissconnectClient();
    void slotVerifyReady();

private:
    std::unique_ptr<QTcpServer> m_pTcpServer;
    std::shared_ptr<QFile> m_pLogFile;
    DatabaseManager::DatabaseManager m_dbManager;
    QMap<QString, ClientInfo::ClientInfo> m_mapClients;
    ServerDialog m_dialog;
    uint m_nNextBlockSize;
    QSslCertificate m_Certificate;
    QSslKey m_privKey;


    void ReadClientREG_LOG(QDataStream& in, QSslSocket* pClientSocket,const int& typeMsg);
    void ReadClientMESSAGE(QDataStream& in, const int &typeMsg);


    void SendToClient(QSslSocket* pClientSocket, const int& type, const QByteArray& arrBlockMsg = QByteArray());
};

}
