#pragma once
#include "stdafx.h"


class DataBaseClass
{
private:
    QSqlDatabase m_db;

public:
    DataBaseClass();

    bool ConnectToDataBase();
    bool isUsernameBusy(QString Username);
    bool WriteToDataBase(QString Username, QString Password, UserInfo UInfo);
    bool isCorrectLogin(QString Username, QString Password);

    QString GetLastError() const ;

    void FillMapUsername(QMap<QString, UserInfo>& map);
};


