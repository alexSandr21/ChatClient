#pragma once
#include "clientinfo.h"


namespace DatabaseManager {

    class DatabaseManager
    {

    public:

        ~DatabaseManager();

        void SetLogFile(std::shared_ptr<QFile> t_pLogFile);

        bool ConnectToDataBase();
        bool IsUsernameBusy(const QString& Username);
        bool WriteToDataBase(QString Username, const QByteArray& Password, ClientInfo::ClientInfo UInfo);
        bool IsCorrectLogin(QString Username, const QByteArray& Password);
        void WriteToLogFile(const QString &t_errorMsg);


        QString GetLastError() const ;

        void FillMapUsername(QMap<QString, ClientInfo::ClientInfo>& map);
        QSqlDatabase GetDB();

    private:
            QSqlDatabase m_db;
            std::shared_ptr<QFile> m_pLogFile;
    };

}
