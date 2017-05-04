#pragma once

#include <QtSql>
#include <memory>

#include "clientinfo.h"




namespace DatabaseManager {

class DatabaseManager
{

public:
    DatabaseManager();

    void SetLogFile(std::shared_ptr<QFile> t_pLogFile);

    bool ConnectToDataBase();
    bool IsUsernameBusy(const QString& Username);
    bool WriteToDataBase(QString Username, QString Password, ClientInfo::ClientInfo UInfo);
    bool WriteToDataBase(const QByteArray& t_UserHash, const ClientInfo::ClientInfo& t_UInfo);
    bool IsCorrectLogin(QString Username, QString Password);
    bool IsCorrectLogin(const QByteArray& t_UserHash);

    QString GetLastError() const ;

    void FillMapUsername(QMap<QString, ClientInfo::ClientInfo>& map);

private:
        QSqlDatabase m_db;
        std::shared_ptr<QFile> m_pLogFile;
};

}
