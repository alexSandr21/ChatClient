#include "stdafx.h"
#include "clientconnection.h"


Connection::ClientConnection::ClientConnection(ServerDialog &t_dialog,
                                               DatabaseManager::DatabaseManager &t_dbManager,
                                               QMap<QString, ClientInfo::ClientInfo> &t_mapClients,
                                               std::shared_ptr<QFile> t_pLogFile, QObject *parent): QObject(parent),
                                                                                                    m_pLogFile(t_pLogFile),
                                                                                                    m_nextBlockSize(0)
{
    m_pDbManager = &t_dbManager;
    m_pMapClients = &t_mapClients;
    m_pDialog = &t_dialog;
}

bool Connection::ClientConnection::SetSocketDescriptor(quintptr t_descriptor, const QSslConfiguration &t_config)
{
    if(m_socket.setSocketDescriptor(t_descriptor))
    {
        connect(&m_socket, &QSslSocket::encrypted, this, &ClientConnection::SlotVerifyReady);
        connect(&m_socket, &QSslSocket::disconnected, this, &ClientConnection::SlotDissconnectClient);
        connect(&m_socket, &QSslSocket::disconnected, this, &ClientConnection::deleteLater);
        connect(&m_socket, &QSslSocket::readyRead, this, &ClientConnection::SlotReadClient);

        m_socket.setSslConfiguration(t_config);

        m_socket.startServerEncryption();
        if(!m_socket.waitForEncrypted(1000))
        {
            qDebug()<<"Client didn't verify";
            return false;
        }
        return true;
    }
    else
    {
        WriteToLogFile(m_socket.errorString());
        return false;
    }
}

QSslSocket* Connection::ClientConnection::socket()
{
    return &m_socket;
}


void Connection::ClientConnection::SlotReadClient()
{
    QDataStream in(&m_socket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    int typeMsg = 0;
    QTime time;

    if(!m_nextBlockSize)
    {
        if(m_socket.bytesAvailable()<sizeof(uint))
            return;
        in>>m_nextBlockSize;
    }

    if( m_socket.bytesAvailable()<m_nextBlockSize)
        return;

    in>>typeMsg>>time;

    if(typeMsg == MESSAGE||typeMsg == L_FILE)
        ReadCLientMessage(in, typeMsg);

    else if(typeMsg == REG || typeMsg == LOGIN)
        ReadClientRegLog(in, typeMsg);

    m_nextBlockSize = 0;
}

void Connection::ClientConnection::SlotDissconnectClient()
{

    auto it = m_pMapClients->begin();

    for( ; it != m_pMapClients->end(); ++it)
    {
        if(it.value().pClientSocket == &m_socket)
        {
            break;
        }
    }

    if(it == m_pMapClients->end())
    {
        return;
    }

    it.value().pClientSocket->close();
    it.value().pClientSocket = nullptr;
    m_pDialog->WriteToTextBrowser("Client "+it.key()+" disconnect from the server");

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(it.key(),it.value());

    for(it = m_pMapClients->begin();it!= m_pMapClients->end();++it)
    {
        if(it.value().pClientSocket)
            SendToClient(UPDATE, it.value().pClientSocket, arrBlock);
    }
}

void Connection::ClientConnection::SlotVerifyReady()
{
    qDebug()<<"Client verified";
}


void Connection::ClientConnection::WriteToLogFile(const QString &t_errorMsg)
{
    if(m_pLogFile->isOpen())
    {
        m_pLogFile->write(QByteArray::fromStdString(t_errorMsg.toStdString() + "\r\n"));
    }
}

void Connection::ClientConnection::ReadClientRegLog(QDataStream &in, const int &t_typeMsg)
{
    try
    {
    ClientInfo::ClientInfo StructUI;
    QString strMsg;
    QVector<QString> strVec;

    in>>strMsg;

    strVec = strMsg.split(ClientInfo::DELIM).toVector();

    if(strVec.size()<2 && t_typeMsg == LOGIN)
    {
        WriteToLogFile("Error: Can`t correct split client message. LOG_RROR");
        SendToClient(LOG_ERROR, &m_socket);
        return;
    }
    else if(strVec.size()<4 && t_typeMsg == REG)
    {
        WriteToLogFile("Error: Can`t correct split client message. REG_ERROR");
        SendToClient(REG_ERROR, &m_socket);
        return;
    }

    QString UserName, Password;
    UserName = strVec[0];
    Password = strVec[1];

    if(t_typeMsg == REG)
    {


        StructUI.name = strVec[2];
        StructUI.surName = strVec[3];

        StructUI.pClientSocket = &m_socket;


        if(m_pDbManager->IsUsernameBusy(UserName))
        {
            SendToClient(REG_ERROR, &m_socket);
            return;
        }

        if(!m_pDbManager->WriteToDataBase(UserName, QCryptographicHash::hash(QByteArray::fromStdString(Password.toStdString()), QCryptographicHash::Sha3_384), StructUI))
        {
            WriteToLogFile("Error: DB error");
            SendToClient(REG_ERROR, &m_socket);
            return;
        }

        qDebug()<<"Client "<<UserName<<" registrate at server";
        m_pDialog->WriteToTextBrowser("Client " + UserName +" registrate at server");
    }
    else if(t_typeMsg == LOGIN)
    {


        if(!m_pDbManager->IsCorrectLogin(UserName,QCryptographicHash::hash(QByteArray::fromStdString(Password.toStdString()), QCryptographicHash::Sha3_384)))
        {
            SendToClient(LOG_ERROR, &m_socket);
            return;
        }


        StructUI = m_pMapClients->value(UserName);

        if(StructUI.name.isEmpty())
        {
            SendToClient(LOG_ERROR, &m_socket);
            return;
        }

        if(StructUI.pClientSocket)
        {
            qDebug()<<"User already online";
            SendToClient(LOG_ERROR, &m_socket);
            return;
        }

        StructUI.pClientSocket = &m_socket;

        qDebug()<<"Client "<<UserName<<" login to the server";
        m_pDialog->WriteToTextBrowser("Client "+ UserName + " login to the server");
    }
    else
    {
        //write label error in log file

        WriteToLogFile("Error: Received unexpected data");

        return;
    }


    m_pMapClients->insert(UserName, StructUI);


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(UserName, StructUI);


    for(auto currUsername:m_pMapClients->keys())
    {
        if(currUsername == UserName)
        {

            arrBlock.clear();
            out.device()->reset();
            out<<*m_pMapClients;
            SendToClient(OK, &m_socket, arrBlock);


            arrBlock.clear();
            out.device()->reset();
            out<<qMakePair(UserName, StructUI);
        }
        else if(m_pMapClients->value(currUsername).pClientSocket)
        {
            SendToClient(UPDATE, m_pMapClients->value(currUsername).pClientSocket, arrBlock);
        }
    }
    }
    catch(const std::exception &ex)
    {
        QString error = ex.what();
        qDebug()<<error;
        WriteToLogFile(error);
    }
}

void Connection::ClientConnection::ReadCLientMessage(QDataStream &in, const int &t_typeMsg)
{
    try
    {
        QString strMsg, strSender, strReciever;
        QVector<QString> strVec;
        QByteArray file;

        in>>strMsg;

        if(t_typeMsg == L_FILE)
            file = in.device()->readAll();

        strVec = strMsg.split(ClientInfo::DELIM).toVector();
        strSender = strVec[0];
        strReciever = strVec[1];



        qDebug()<<"Client "<<strSender<<" send message to "<<strReciever;
//        m_dialog.WriteToTextBrowser("Client "+strSender+" send message to "+strReciever);

        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        strMsg = strSender + ClientInfo::DELIM;
        for(int i = 2; i<strVec.size(); i++)
        {
            strMsg+=strVec[i];
        }

        out<<strMsg;

        if(t_typeMsg == L_FILE)
            arrBlock.append(file);


        SendToClient(t_typeMsg, m_pMapClients->value(strReciever).pClientSocket, arrBlock);
    }
    catch(const std::exception &ex)
    {
        //write error in log file
        WriteToLogFile(QString(ex.what()));
    }
}

void Connection::ClientConnection::SendToClient(const int &t_typeMsg, QSslSocket *t_socket, const QByteArray &t_arrBlockMsg)
{
    try
    {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out<<uint(0)<<t_typeMsg<<QTime::currentTime();


        if(!t_arrBlockMsg.isEmpty())
            arrBlock.append(t_arrBlockMsg);

        out.device()->seek(0);
        out<<uint(arrBlock.size() - sizeof(uint));

        if(t_socket)
            t_socket->write(arrBlock);
        else
            WriteToLogFile("Pointer to socket is nullptr");

    }
    catch(const std::exception &ex)
    {
        //write error in log file
        WriteToLogFile(ex.what());
    }
}
