#include "model.h"




YAClient::Model::Model(QObject *parent) : QObject(parent)
{
    port = 3004;
    blockSize =0;

}

YAClient::Model::~Model()
{
    socket->close();
    socket.reset();
}

void YAClient::Model::connectToHost(const QString &hostIP)
{
    //
     emit signalError("model test");


    socket.reset(new QSslSocket());

    connect(socket.get(), SIGNAL(readyRead()), SLOT(slotRead()));
    connect(socket.get(), SIGNAL(encrypted()), SLOT(slotConnected()));
    connect(socket.get(), SIGNAL(sslErrors(const QList<QSslError> &)),SLOT(slotConnectError(const QList<QSslError> &)));
    connect(socket.get(), SIGNAL(disconnected()), SLOT(slotDisconnected()));

    QSslCertificate cert(certByte);
    if(cert.isNull())
    {
        emit signalConnectResult("Connection error");
        emit signalError("Error sertificate read");
        return;
    }
    socket->addCaCertificate(cert);

    socket->setProtocol(QSsl::TlsV1SslV3);
    socket->setPeerVerifyDepth(2);


    socket->connectToHostEncrypted(hostIP, port);

    if(!socket->waitForEncrypted(1000))
        emit signalConnectResult("Connection error");


}

void YAClient::Model::SendMessage(const int &label, const QString & message, const QByteArray & file)
{
    try
    {
        QByteArray arrBlock;
        QDataStream send(&arrBlock, QIODevice::WriteOnly);
        send.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        send<<uint(0)<<label<<QTime::currentTime()<<message;

        if(!file.isEmpty())
            arrBlock.append(file);

        send.device()->seek(0);

        uint size(arrBlock.size() - sizeof(uint));
        send<<(size);

        qDebug()<<socket->write(arrBlock);
        qDebug()<<socket->waitForBytesWritten(1);
    }
    catch(const std::exception & ex)
    {
        emit signalError(ex.what());
        emit signalMessageError("Error send file");
    }

}

QMap<QString, YAClient::clientInfo>* YAClient::Model::GetClients()
{
    return &clientsList;
}


void YAClient::Model::Parser(QDataStream &mess)
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
        emit signalError("Protocol error");
        break;
    }
}

void YAClient::Model::slotRead()
{
    try
    {
        QDataStream in(socket.get());
        in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        for(;;)
        {
            if(!blockSize)
            {
                if(socket->bytesAvailable()<sizeof(uint))
                    break;

                in>>blockSize;
            }

            if(socket->bytesAvailable()<blockSize)
                break;

            blockSize = 0;
            Parser(in);
        }
    }
    catch(const std::exception &ex)
    {
        emit signalError(ex.what());
    }
}

void YAClient::Model::slotConnectError(const QList<QSslError> &err)
{

    for(int i=0; i<err.size();i++)
    {
        if(err.at(i).error()==QSslError::HostNameMismatch)
        {
            qDebug()<<"true";

            QList<QSslError> ign;
            ign.append(err.at(i));

            socket->ignoreSslErrors(ign);
            break;
        }
    }
}

void YAClient::Model::slotConnected()
{
    emit signalConnectResult("");
}

void YAClient::Model::slotDisconnected()
{
    emit signalConnectResult("Connection lost");
}



