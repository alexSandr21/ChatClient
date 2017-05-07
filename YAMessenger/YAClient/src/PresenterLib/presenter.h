#pragma once
#include <QObject>
#include "messagesdatabase.h"
#include "model.h"
#include <QDateTime>
#include <vector>

namespace YAClient
{
    const std::vector<char> WRONGCH{'.',',','*',';',' ',':','\'','\"','|','?', '&', '$', '@', '#'};
    const int MAX_FILE_SIZE = 30000000;


    class Presenter : public QObject
    {
        Q_OBJECT

    public:
        explicit Presenter(std::shared_ptr<QFile> log, QObject *parent = 0);

        Q_INVOKABLE bool methCheckString(QString str);
        Q_INVOKABLE bool methCheckStringAll(QString str);
        Q_INVOKABLE QString methGetReceiver();
        Q_INVOKABLE QString methGetLogin();
        Q_INVOKABLE bool methIsOnline(QString receiver);
        Q_INVOKABLE bool methIsContact();

    signals:

        //signals for VIEW
        void signalSetUsers(QString name, QString status, QString login);
        void signalNewUser(QString name, QString status, QString login);
        void signalConnectResult(QString result);
        void signalLoginResult(bool result);
        void signalRegistrationResult(bool result);
        void signalNewMessage(QString sender);
        void signalWriteMessage(QString interlocutor, bool myanswer, QString message, QString time);
        void signalMessageError(QString mess);
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
        void slotNewClient(const QPair<QString, clientInfo> & newClient);
        void slotNewMessage(const QString & sender, const QTime & time, const QString & message);
        void slotNewFile(const QString & sender, const QTime & time, const QString & fileName, const QByteArray & file);
        void slotWrongLogin();
        void slotLoginExist();
        void slotOK();
        void slotWriteLog(const QString & errStr);

    private:

        QMap<QString, clientInfo> * clientsMap;
        std::shared_ptr<QFile> logFile;
        QString myReceiver;
        QString receiverName;
        QString myLogin;
        MessagesDataBase dBase;
        Model model;
        bool contact;
    };

}
