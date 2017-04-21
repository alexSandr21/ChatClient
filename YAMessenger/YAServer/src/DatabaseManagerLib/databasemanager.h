#pragma once

#include <QtSql>


#include "clientinfo.h"




namespace DatabaseManager {

class DatabaseManager
{
    QSqlDatabase m_db;
public:
    DatabaseManager();

    bool ConnectToDataBase();
    bool IsUsernameBusy(QString Username);
    bool WriteToDataBase(QString Username, QString Password, ClientInfo::ClientInfo UInfo);
    bool IsCorrectLogin(QString Username, QString Password);

    QString GetLastError() const ;

    void FillMapUsername(QMap<QString, ClientInfo::ClientInfo>& map);
};

}
