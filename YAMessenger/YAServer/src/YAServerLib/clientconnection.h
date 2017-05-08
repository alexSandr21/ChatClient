#pragma once

#include "databasemanager.h"
#include "clientinfo.h"
#include "serverdialog.h"
#include <QObject>
#include <QSslConfiguration>
#include <memory>

namespace Connection{

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

    class ClientConnection : public QObject
    {
        Q_OBJECT
    public:
        explicit ClientConnection(ServerDialog &t_dialog, DatabaseManager::DatabaseManager &t_dbManager, QMap<QString, ClientInfo::ClientInfo> &t_mapClients, std::shared_ptr<QFile> t_pLogFile, QObject *parent = 0);

        bool SetSocketDescriptor(quintptr t_descriptor, const QSslConfiguration& t_config);

        QSslSocket* socket();


    private slots:
        void SlotReadClient();
        void SlotDissconnectClient();
        void SlotVerifyReady();

    private:
        QSslSocket m_socket;
        DatabaseManager::DatabaseManager* m_pDbManager;
        QMap<QString,ClientInfo::ClientInfo>* m_pMapClients;
        ServerDialog* m_pDialog;
        std::shared_ptr<QFile> m_pLogFile;
        uint m_nextBlockSize;

        void WriteToLogFile(const QString& t_errorMsg);
        void ReadClientRegLog(QDataStream& in, const int& t_typeMsg);
        void ReadCLientMessage(QDataStream& in, const int& t_typeMsg);
        void SendToClient(const int& t_typeMsg, QSslSocket* t_socket, const QByteArray& t_arrBlockMsg = QByteArray());
    };


}
