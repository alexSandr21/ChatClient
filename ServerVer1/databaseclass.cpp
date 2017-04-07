#include "stdafx.h"
#include "databaseclass.h"


DataBaseClass::DataBaseClass()
{
}


bool DataBaseClass::ConnectToDataBase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("ServerDB.db");

    if(m_db.open())
    {
        qDebug()<<"DB open";
    }
    else
    {
        //write error in log file
        qDebug()<<m_db.lastError().text();
        return false;
    }

    QSqlQuery qry(m_db);


    if(!m_db.tables().contains(QLatin1String("Users")))
    {
        qry.prepare("CREATE TABLE Users (id INTEGER PRIMARY KEY, Username VARCHAR, Password VARCHAR, Name VARCHAR, Surname VARCHAR);");
        if(!qry.exec())
        {
            //write error in log file
            qDebug()<<qry.lastError().text();
            return false;
        }
    }

    return true;
}


bool DataBaseClass::IsUsernameBusy(QString Username)
{
    QSqlQuery qry(m_db);

    qry.prepare("SELECT Username FROM Users WHERE Username = :name");
    qry.bindValue(":name", Username);
    if(qry.exec())
    {
        if(qry.next())
        {
            qDebug()<<"Username is busy!";
            return true;
        }
        else
            return false;
    }

    //write error in log file
    qDebug()<<m_db.lastError().text();
    return true;

}


bool DataBaseClass::WriteToDataBase(QString Username, QString Password, UserInfo UInfo)
{
    if(IsUsernameBusy(Username))
    {
        return false;
    }

    QSqlQuery qry(m_db);

    qry.prepare("INSERT INTO Users (Username, Password, Name, Surname) Values (:username,:pass, :name, :surname)");
    qry.bindValue(":username", Username);
    qry.bindValue(":pass", Password);
    qry.bindValue(":name", UInfo.name);
    qry.bindValue(":surname", UInfo.surName);

    if(qry.exec())
    {
        qDebug()<<"Refistration succesed";
        return true;
    }
    else
    {
        //write error in log file
        QString str = m_db.lastError().text();
        return false;
    }

}


bool DataBaseClass::IsCorrectLogin(QString Username, QString Password)
{
    QSqlQuery qry(m_db);

    qry.prepare("SELECT Username, Password From Users WHERE Username= :name AND Password= :pass");
    qry.bindValue(":name", Username);
    qry.bindValue(":pass", Password);
    if(qry.exec())
    {
        if(qry.next())
        {
            qDebug()<<"Login succseed";
            return true;
        }
        qDebug()<<"Wrong Username or Password";
        return false;
    }

    //write error in log file
    qDebug()<<m_db.lastError().databaseText();
    return false;
}


QString DataBaseClass::GetLastError() const
{
    return this->m_db.lastError().text();
}


void DataBaseClass::FillMapUsername(QMap<QString, UserInfo>& map)
{
    QSqlQuery qry(this->m_db);
    UserInfo tempUIS;
    qry.prepare("SELECT Username, Name, Surname FROM Users");
    if(qry.exec())
    {
        while(qry.next())
        {
            tempUIS.name = qry.value(1).toString();
            tempUIS.surName = qry.value(2).toString();
            map.insert(qry.value(0).toString(), tempUIS);
        }
    }
    else
    {
        //write errror in log file
    }
}
