#pragma once

#include <QtSql>
#include <memory>

#include "clientinfo.h"




namespace DatabaseManager {

    class DatabaseManager
    {

    public:
        void SetLogFile(std::shared_ptr<QFile> t_pLogFile);

        bool ConnectToDataBase();
        bool IsUsernameBusy(const QString& Username);
        bool WriteToDataBase(QString Username, const QByteArray& Password, ClientInfo::ClientInfo UInfo);
        bool IsCorrectLogin(QString Username, const QByteArray& Password);

        QString GetLastError() const ;

        void FillMapUsername(QMap<QString, ClientInfo::ClientInfo>& map);

    private:
            QSqlDatabase m_db;
            std::shared_ptr<QFile> m_pLogFile;
};

}
