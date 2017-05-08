#include "model.h"




YAClient::Model::Model(QObject *parent) : QObject(parent),m_port(3004), m_blockSize(0)
{
}

YAClient::Model::~Model()
{
    m_socket->close();
    m_socket.reset();
}

void YAClient::Model::connectToHost(const QString &hostIP)
{
    m_socket.reset(new QSslSocket());

    connect(m_socket.get(), SIGNAL(readyRead()), SLOT(slotRead()));
    connect(m_socket.get(), SIGNAL(encrypted()), SLOT(slotConnected()));
    connect(m_socket.get(), SIGNAL(sslErrors(const QList<QSslError> &)),SLOT(slotConnectError(const QList<QSslError> &)));
    connect(m_socket.get(), SIGNAL(disconnected()), SLOT(slotDisconnected()));

    QSslCertificate cert(certByte);
    if(cert.isNull())
    {
        emit signalConnectResult("Connection error");
        emit signalError("Error sertificate read");
        return;
    }
    m_socket->addCaCertificate(cert);

    m_socket->setProtocol(QSsl::TlsV1SslV3);
    m_socket->setPeerVerifyDepth(2);


    m_socket->connectToHostEncrypted(hostIP, m_port);

    if(!m_socket->waitForEncrypted(1000))
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

        qDebug()<<m_socket->write(arrBlock);
        qDebug()<<m_socket->waitForBytesWritten(1);
    }
    catch(const std::exception & ex)
    {
        emit signalError(ex.what());
        emit signalMessageError("Error send file");
    }

}

QMap<QString, YAClient::clientInfo>* YAClient::Model::GetClients()
{
    return &m_clientsList;
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
        mess>>m_clientsList;
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
        m_clientsList.insert(newClient.first, newClient.second);
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
        QDataStream in(m_socket.get());
        in.setVersion(QDataStream::Qt_DefaultCompiledVersion);

        for(;;)
        {
            if(!m_blockSize)
            {
                if(m_socket->bytesAvailable()<sizeof(uint))
                    break;

                in>>m_blockSize;
            }

            if(m_socket->bytesAvailable()<m_blockSize)
                break;

            m_blockSize = 0;
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
            QList<QSslError> ign;
            ign.append(err.at(i));

            m_socket->ignoreSslErrors(ign);
        }
        else
            emit signalError(err.at(i).errorString());
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



