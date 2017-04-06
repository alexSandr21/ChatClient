#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql>
#include <QString>
#include "messagestruct.h"
#include <QDebug>
#include <exception>

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

#endif // DATABASE_H
