#pragma once
#include <QObject>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QString>


namespace YAClient
{

    struct MessageStruct
    {
        QString interlocutor;
        int myAnswer;
        QString message;
        QString time;
    };


    class MessagesDataBase : public QObject
    {
        Q_OBJECT

    public:
        explicit MessagesDataBase(QObject *parent = 0);

        int OpenDataBase();

        bool CreateTabel(const QString & tabelName);
        void Insert(const MessageStruct & mess);
        QVector<MessageStruct> GetMessages( const QString &interlocutor);

    private:

        QSqlDatabase dBase;
        QSqlQuery query;
        QSqlRecord record;
        QString name;
    };
}

