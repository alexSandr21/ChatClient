//#include "stdafx.h"
#include "messagesdatabase.h"

namespace YAClient
{
MessagesDataBase::MessagesDataBase(QObject *parent) : QObject(parent)
{
    dBase = QSqlDatabase::addDatabase("QSQLITE");
    dBase.setDatabaseName("Messages.db");

    query = QSqlQuery(dBase);

}

int MessagesDataBase::OpenDataBase()
{
    return dBase.open();
}

bool MessagesDataBase::CreateTabel(const QString &tabelName)
{
    bool res = true;

    name='t'+tabelName;

    QString str("CREATE TABLE "+name+" (interlocutor VARCHAR, "
                                     "myAnswer INTEGER, "
                                     "message VARCHAR, "
                                     "time VARCHAR"
                                     ");");

    if(!query.exec(str))
        if(query.lastError().number()!=-1)
        {
            //write error in log file
            res = false;
        }



    return res;
}

void MessagesDataBase::Insert(const MessageStruct & mess)
{
    QString qe = "INSERT INTO "+name+" (interlocutor, myAnswer, message, time) VALUES (:interlocutor, :myAnswer, :message, :time);";

    query.prepare(qe);

    query.bindValue(0, mess.interlocutor);
    query.bindValue(1, mess.myAnswer);
    query.bindValue(2, mess.message);
    query.bindValue(3, mess.time);

    if(!query.exec())
    {
        QString str = query.lastError().text();
        //write error in log file
    }


}



QVector<MessageStruct> MessagesDataBase::GetMessages(const QString &interlocutor)
{
    QVector<MessageStruct> messages;
    if( query.exec("SELECT * FROM "+name+" WHERE interlocutor = '"+interlocutor+"';"))
    {
        record = query.record();

        while(query.next())
        {
            MessageStruct ms;
            ms.interlocutor = query.value(record.indexOf("interlocutor")).toString();
            ms.message = query.value(record.indexOf("message")).toString();
            ms.myAnswer = query.value(record.indexOf("myAnswer")).toInt();
            ms.time = query.value(record.indexOf("time")).toString();

            messages.push_back(ms);
        }


    }
    else
    {
        QString str = query.lastError().text();
        //write error in log file
    }
    return messages;
}
}

