#pragma once

#include "userinfo.h"
#include "emessagetype.h"




class DataBaseClass
{
private:
    QSqlDatabase m_db;

public:
    DataBaseClass();

    bool ConnectToDataBase();
    bool IsUsernameBusy(QString Username);
    bool WriteToDataBase(QString Username, QString Password, UserInfo UInfo);
    bool IsCorrectLogin(QString Username, QString Password);

    QString GetLastError() const ;

    void FillMapUsername(QMap<QString, UserInfo>& map);
};


