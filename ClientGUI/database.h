#pragma once

#include "messagestruct.h"


class MessagesDataBase : public QObject
{
    Q_OBJECT

    QSqlDatabase dBase;
    QSqlQuery query;
    QSqlRecord record;
    QString name;

public:
    explicit MessagesDataBase(QObject *parent = 0);

    int OpenDataBase();

    bool CreateTabel(const QString & tabelName);
    void Insert(const MessageStruct & mess);
    QVector<MessageStruct> GetMessages( const QString &interlocutor);

};

