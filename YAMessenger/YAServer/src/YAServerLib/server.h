#pragma once

#include "databasemanager.h"
#include "clientinfo.h"
#include "serverdialog.h"
#include "clientconnection.h"


namespace YAServer {


    class Server : public QTcpServer
    {
        Q_OBJECT
    public:
        Server(std::shared_ptr<QFile> t_pLogFile, QObject *parent = 0);
        ~Server();

        bool StartServer(const int& t_Port);
        QString GetIP();

    protected:
        void incomingConnection(qintptr handle) override;

    private:
        void WriteToLogFile(const QString& t_errorMsg);

    private:
        QMap<QString, ClientInfo::ClientInfo> m_mapClients;     //List of users, key is login and value is struct with user info
        DatabaseManager::DatabaseManager m_dbManager;
        ServerDialog m_dialog;
        QSslConfiguration m_sslConfig;
        std::shared_ptr<QFile> m_pLogFile;
        int m_port;


    };

}
