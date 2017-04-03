#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "clientinfo.h"
#include <QDataStream>
#include <QTime>


enum {L_REG, L_LOG, L_MESS, L_OK, L_ERROR, L_NEWCLIENT, L_LOGINEXIST};

class Model : public QObject
{
    Q_OBJECT

    QScopedPointer<QTcpSocket> socket;
    quint32 blockSize = 0;
    QMap<QString, clientInfo> clientsList;
    int port;


    void Parser(QDataStream & mess);

public:

    explicit Model(QObject *parent = 0);
    ~Model();

    void connectToHost(const QString &hostIP);
    void SendMessage(const int & label, const QString & message);
    QMap<QString, clientInfo>*GetClients();

signals:

    //signals for GUI
    void signalOK();
    void signalNewClient(const QPair<QString, clientInfo> & newClient);
    void signalNewMessage(const QString & sender, const QTime &time, const QString & message);
    void signalConnect(const QString & err);
    void signalWrongLogin();
    void signalLoginExist();
    void signalDisconnect();

public slots:

    void slotRead();
    void slotConnectError(QAbstractSocket::SocketError);
    void slotConnected();
    void slotDisconnected();
};


#endif // MODEL_H
