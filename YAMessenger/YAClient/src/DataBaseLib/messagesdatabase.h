#pragma once

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

    signals:
        void signalError(const QString & errStr);

    private:

        QSqlDatabase m_dBase;
        QSqlQuery m_query;
        QSqlRecord m_record;
        QString m_name;
    };
}

