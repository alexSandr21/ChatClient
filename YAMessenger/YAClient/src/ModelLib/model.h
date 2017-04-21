#pragma once

#include "stdafx.h"
#include <QMutex>
#include <memory>
#include <QTcpSocket>
#include <QDebug>
#include "clientinfo.h"


namespace YAClient
{

class Model : public QObject
{
    Q_OBJECT

    std::unique_ptr<QTcpSocket> socket;
    uint blockSize = 0;
    QMap<QString, clientInfo> clientsList;
    int port;

    void Parser(QDataStream & mess);

public:

    explicit Model(QObject *parent = 0);
    ~Model();

    void connectToHost(const QString &hostIP);
    QMap<QString, clientInfo>*GetClients();

signals:

    //signals for GUI
    void signalOK();
    void signalNewClient(const QPair<QString, clientInfo> & newClient);
    void signalNewMessage(const QString & sender, const QTime &time, const QString & message);
    void signalNewFile(const QString &sender, const QTime &time,const QString & fileName, const QByteArray &file);
    void signalConnect(const QString & err);
    void signalWrongLogin();
    void signalLoginExist();
    void signalMessageError(QString mess);

public slots:

    void SendMessage(const int & label, QString message, QByteArray file = QByteArray());
    void slotRead();
    void slotConnectError(QAbstractSocket::SocketError err);
    void slotConnected();
};
}
