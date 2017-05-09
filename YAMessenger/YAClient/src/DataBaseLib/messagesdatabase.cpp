#include "stdafx.h"
#include "messagesdatabase.h"


YAClient::MessagesDataBase::MessagesDataBase(QObject *parent) : QObject(parent)
{
    m_dBase = QSqlDatabase::addDatabase("QSQLITE");
    m_dBase.setDatabaseName("Messages.db");

    m_query = QSqlQuery(m_dBase);

}

bool YAClient::MessagesDataBase::OpenDataBase()
{
    return m_dBase.open();
}

bool YAClient::MessagesDataBase::CreateTabel(const QString &tabelName)
{
    bool res = true;

    m_name='t'+tabelName;

    QString str("CREATE TABLE "+m_name+" (interlocutor VARCHAR, "
                                     "myAnswer INTEGER, "
                                     "message VARCHAR, "
                                     "time VARCHAR"
                                     ");");

    if(!m_query.exec(str))
    {
        emit signalError(m_query.lastError().text());
        res = false;
    }

    return res;
}

void YAClient::MessagesDataBase::Insert(const MessageStruct & mess)
{
    QString qe = "INSERT INTO "+m_name+" (interlocutor, myAnswer, message, time) VALUES (:interlocutor, :myAnswer, :message, :time);";

    m_query.prepare(qe);

    m_query.bindValue(0, mess.interlocutor);
    m_query.bindValue(1, mess.myAnswer);
    m_query.bindValue(2, mess.message);
    m_query.bindValue(3, mess.time);

    if(!m_query.exec())
        emit signalError(m_query.lastError().text());

}



QVector<YAClient::MessageStruct> YAClient::MessagesDataBase::GetMessages(const QString &interlocutor)
{
    QVector<MessageStruct> messages;

    if( m_query.exec("SELECT * FROM "+m_name+" WHERE interlocutor = '"+interlocutor+"';"))
    {
        m_record = m_query.record();

        while(m_query.next())
        {
            MessageStruct ms;
            ms.interlocutor = m_query.value(m_record.indexOf("interlocutor")).toString();
            ms.message = m_query.value(m_record.indexOf("message")).toString();
            ms.myAnswer = m_query.value(m_record.indexOf("myAnswer")).toInt();
            ms.time = m_query.value(m_record.indexOf("time")).toString();

            messages.push_back(ms);
        }


    }
    else
        emit signalError(m_query.lastError().text());

    return messages;
}

