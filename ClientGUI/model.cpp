#include "model.h"

Model::Model(QObject *parent) : QObject(parent)
{
    socket.reset(new QTcpSocket());
    port = 3004;
    blockSize =0;

    connect(socket.data(), SIGNAL(readyRead()), SLOT(slotRead()));
    connect(socket.data(), SIGNAL(connected()), SLOT(slotConnected()));
    connect(socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotConnectError(QAbstractSocket::SocketError)));
}

Model::~Model()
{
    socket->close();
    socket.reset();
}

void Model::connectToHost(const QString &hostIP)
{
    socket->connectToHost(hostIP, port);
}

void Model::SendMessage(const int &label, const QString &message,const QByteArray &file)
{
    try
    {
        QByteArray arrBlock;
        QDataStream send(&arrBlock, QIODevice::WriteOnly);
        send.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        send<<quint32(0)<<label<<QTime::currentTime()<<message;

        if(!file.isEmpty())
            arrBlock.append(file);

        send.device()->seek(0);
        send<<(arrBlock.size() - sizeof(quint32));

        socket->write(arrBlock);
    }
    catch(const std::exception&ex)
    {
        //write error in log file
        emit signalMessageError();
    }
}

QMap<QString, clientInfo> *Model::GetClients()
{
    return &clientsList;
}

void Model::Parser(QDataStream &mess)
{
    int label= 0;

    QTime time;
    QString message;
    QString sender;
    QPair<QString, clientInfo> newClient;
    QByteArray file;

    mess>>label;
    mess>>time;

    switch (label)
    {
    case L_OK:
        mess>>clientsList;
        emit signalOK();
        break;

    case L_MESS:
        mess>>message;
        sender = message.left(message.indexOf(DELIM));
        message.remove(0, sender.size()+1);
        emit signalNewMessage(sender, time, message);
        break;

    case L_FILE:
        mess>>message;
        sender = message.left(message.indexOf(DELIM));
        message.remove(0, sender.size()+1);

        mess.device()->seek(mess.device()->pos()+4);

        file = mess.device()->readAll();
        emit signalNewFile(sender, time, message, file);
        break;

    case L_ERROR:
        emit signalWrongLogin();
        break;

    case L_LOGINEXIST:
        emit signalLoginExist();
        break;

    case L_NEWCLIENT:
        mess>>newClient;
        clientsList.insert(newClient.first, newClient.second);
        emit signalNewClient(newClient);
        break;

    default:
        //write label error in log file
        break;
    }
}

void Model::slotRead()
{
    QDataStream in(socket.data());
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    for(;;)
    {
        if(!blockSize)
        {
            if(socket->bytesAvailable()<sizeof(quint32))
                break;

            in>>blockSize;
        }

        if(socket->bytesAvailable()<blockSize)
            break;

        blockSize = 0;
        Parser(in);
    }
}

void Model::slotConnectError(QAbstractSocket::SocketError err)
{

    QString strError = "Error: "+(err == QAbstractSocket::HostNotFoundError?
                                      "The host was not found.":
                                      err == QAbstractSocket::RemoteHostClosedError?
                                          "The remote host is close":
                                          err == QAbstractSocket::ConnectionRefusedError?
                                              "The connection was refused.":
                                              QString(socket->errorString()));

    emit signalConnect(strError);
}

void Model::slotConnected()
{
    emit signalConnect("");
}



