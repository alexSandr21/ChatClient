#pragma once

#include "clientinfo.h"

namespace DatabaseManager {
    class DatabaseManager;
}

namespace TypeMsg{
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

}
namespace YAServer{

    class ServerDialog;


    class ClientConnection : public QObject
    {
        Q_OBJECT
    public:
        ClientConnection(ServerDialog &t_dialog, DatabaseManager::DatabaseManager &t_dbManager, QMap<QString, ClientInfo::ClientInfo> &t_mapClients, std::shared_ptr<QFile> t_pLogFile, QObject *parent = 0);
        ~ClientConnection();

        bool SetSocketDescriptor(quintptr t_descriptor, const QSslConfiguration& t_config);

        QSslSocket* socket();


    private slots:
        void SlotReadClient();
        void SlotDissconnectClient();
        void SlotVerifyReady();

    private:
        void WriteToLogFile(const QString& t_errorMsg);
        void ReadClientRegLog(QDataStream& in, const int& t_typeMsg);
        void ReadCLientMessage(QDataStream& in, const int& t_typeMsg);
        void SendToClient(const int& t_typeMsg, QSslSocket* t_socket, const QByteArray& t_arrBlockMsg = QByteArray());

    private:
        QSslSocket m_socket;
        DatabaseManager::DatabaseManager &m_rDbManager;
        QMap<QString,ClientInfo::ClientInfo> &m_rMapClients;
        ServerDialog &m_rDialog;
        std::shared_ptr<QFile> m_pLogFile;
        uint m_nextBlockSize;


    };


}
