#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QRegularExpression>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>
#include <QVector>
#include "database.h"
#include "model.h"
#include <algorithm>
#include <QFile>
#include <QDir>
#include "customdeleter.h"


const std::vector<char> WRONGCH{'.',',','*',';',' ',':','\'','\"','|','?'};
const int MAX_FILE_SIZE = 30000000;

class Presenter : public QObject
{
    Q_OBJECT

    QMap<QString, clientInfo> * clientsMap;
    QString myReceiver;
    QString receiverName;
    QString myLogin;
    MessagesDataBase dBase;
    Model model;

public:
    explicit Presenter(QObject *parent = 0);

    Q_INVOKABLE bool methCheckString(QString str);
    Q_INVOKABLE bool methCheckStringAll(QString str);
    Q_INVOKABLE QString methGetReceiver();
    Q_INVOKABLE QString methGetLogin();
    Q_INVOKABLE bool methIsOnline(QString receiver);

signals:

    //signals for VIEW
    void signalSetUsers(QString name, QString status, QString login);
    void signalNewUser(QString name, QString status, QString login);
    void signalConnectResult(QString result);
    void signalLoginResult(bool result);
    void signalRegistrationResult(bool result);
    void signalNewMessage(QString sender);
    void signalWriteMessage(QString interlocutor, bool myanswer, QString message, QString time);
    void signalTooBigFile();
    void signalMessageError();
    void signalSetFriendStatus(QString login);
    void signalSetTitle(QString);

public slots:

    //slots for VIEW
    void slotGetUsers();
    void slotSendMessage(QString mess);
    void slotConnect(QString serverIP);
    void slotLogin(QString login, QString pass);
    void slotRegistration(QString name, QString surname, QString login, QString pass);
    void slotSetReceiver(QString receiver);
    void slotSendFile(QString path);

    //slots for MODEL
    void slotConnectResult(QString result);
    void slotNewClient(QPair<QString, clientInfo> newClient);
    void slotNewMessage(QString sender, QTime time, QString message);
    void slotNewFile(QString sender, QTime time, QString fileName, QByteArray file);
    void slotWrongLogin();
    void slotLoginExist();
    void slotOK();
};

#endif // PRESENTER_H
