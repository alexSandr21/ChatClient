#include "stdafx.h"
#include "clientconnection.h"
#include "databasemanager.h"
#include "serverdialog.h"


YAServer::ClientConnection::ClientConnection(ServerDialog &t_dialog,
                                               DatabaseManager::DatabaseManager &t_dbManager,
                                               QMap<QString, ClientInfo::ClientInfo> &t_mapClients,
                                               std::shared_ptr<QFile> t_pLogFile, QObject *parent):
    QObject(parent),
    m_pLogFile(t_pLogFile),
    m_nextBlockSize(0),
    m_rDbManager(t_dbManager),
    m_rMapClients(t_mapClients),
    m_rDialog(t_dialog)
{

}

YAServer::ClientConnection::~ClientConnection()
{
    m_socket.close();
}

bool YAServer::ClientConnection::SetSocketDescriptor(quintptr t_descriptor, const QSslConfiguration &t_config)
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

QSslSocket* YAServer::ClientConnection::socket()
{
    return &m_socket;
}


void YAServer::ClientConnection::SlotReadClient()
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

    if(typeMsg == TypeMsg::MESSAGE||typeMsg == TypeMsg::L_FILE)
        ReadCLientMessage(in, typeMsg);

    else if(typeMsg == TypeMsg::REG || typeMsg == TypeMsg::LOGIN)
        ReadClientRegLog(in, typeMsg);

    m_nextBlockSize = 0;
}

void YAServer::ClientConnection::SlotDissconnectClient()
{

    auto it = m_rMapClients.begin();

    for( ; it != m_rMapClients.end(); ++it)
    {
        if(it.value().pClientSocket == &m_socket)
        {
            break;
        }
    }

    if(it == m_rMapClients.end())
    {
        return;
    }

    it.value().pClientSocket->close();
    it.value().pClientSocket = nullptr;
    m_rDialog.WriteToTextBrowser("Client "+it.key()+" disconnect from the server");

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(it.key(),it.value());

    for(it = m_rMapClients.begin();it!= m_rMapClients.end();++it)
    {
        if(it.value().pClientSocket)
            SendToClient(TypeMsg::UPDATE, it.value().pClientSocket, arrBlock);
    }
}

void YAServer::ClientConnection::SlotVerifyReady()
{
    qDebug()<<"Client verified";
}


void YAServer::ClientConnection::WriteToLogFile(const QString &t_errorMsg)
{
    if(m_pLogFile->isOpen())
    {
        QByteArray arr;
        arr.append(QDateTime::currentDateTime().toString() + " " + t_errorMsg + "\r\n");
        m_pLogFile->write(arr);
    }
}

void YAServer::ClientConnection::ReadClientRegLog(QDataStream &in, const int &t_typeMsg)
{
    try
    {
    ClientInfo::ClientInfo StructUI;
    QString strMsg;
    QVector<QString> strVec;

    in>>strMsg;

    strVec = strMsg.split(ClientInfo::DELIM).toVector();

    if(strVec.size()<2 && t_typeMsg == TypeMsg::LOGIN)
    {
        WriteToLogFile("Error: Can`t correct split client message. LOG_RROR");
        SendToClient(TypeMsg::LOG_ERROR, &m_socket);
        return;
    }
    else if(strVec.size()<4 && t_typeMsg == TypeMsg::REG)
    {
        WriteToLogFile("Error: Can`t correct split client message. REG_ERROR");
        SendToClient(TypeMsg::REG_ERROR, &m_socket);
        return;
    }

    QString UserName, Password;
    UserName = strVec[0];
    Password = strVec[1];

    if(t_typeMsg == TypeMsg::REG)
    {


        StructUI.name = strVec[2];
        StructUI.surName = strVec[3];

        StructUI.pClientSocket = &m_socket;


        if(m_rDbManager.IsUsernameBusy(UserName))
        {
            SendToClient(TypeMsg::REG_ERROR, &m_socket);
            return;
        }

        if(!m_rDbManager.WriteToDataBase(UserName, QCryptographicHash::hash(QByteArray::fromStdString(Password.toStdString()), QCryptographicHash::Sha3_384), StructUI))
        {
            WriteToLogFile("Error: DB error");
            SendToClient(TypeMsg::REG_ERROR, &m_socket);
            return;
        }

        qDebug()<<"Client "<<UserName<<" registrate at server";
        m_rDialog.WriteToTextBrowser("Client " + UserName +" registrate at server");
    }
    else if(t_typeMsg == TypeMsg::LOGIN)
    {


        if(!m_rDbManager.IsCorrectLogin(UserName,QCryptographicHash::hash(QByteArray::fromStdString(Password.toStdString()), QCryptographicHash::Sha3_384)))
        {
            SendToClient(TypeMsg::LOG_ERROR, &m_socket);
            return;
        }


        StructUI = m_rMapClients.value(UserName);

        if(StructUI.name.isEmpty())
        {
            SendToClient(TypeMsg::LOG_ERROR, &m_socket);
            return;
        }

        if(StructUI.pClientSocket)
        {
            qDebug()<<"User already online";
            SendToClient(TypeMsg::LOG_ERROR, &m_socket);
            return;
        }

        StructUI.pClientSocket = &m_socket;

        qDebug()<<"Client "<<UserName<<" login to the server";
        m_rDialog.WriteToTextBrowser("Client "+ UserName + " login to the server");
    }
    else
    {
        //write label error in log file
        WriteToLogFile("Error: Received unexpected data");

        return;
    }


    m_rMapClients.insert(UserName, StructUI);


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(UserName, StructUI);


    for(auto currUsername:m_rMapClients.keys())
    {
        if(currUsername == UserName)
        {

            arrBlock.clear();
            out.device()->reset();
            out<<m_rMapClients;
            SendToClient(TypeMsg::OK, &m_socket, arrBlock);


            arrBlock.clear();
            out.device()->reset();
            out<<qMakePair(UserName, StructUI);
        }
        else if(m_rMapClients.value(currUsername).pClientSocket)
        {
            SendToClient(TypeMsg::UPDATE, m_rMapClients.value(currUsername).pClientSocket, arrBlock);
        }
    }
    }
    catch(const std::exception &ex)
    {
        QString error = ex.what();
        WriteToLogFile(error);
    }
}

void YAServer::ClientConnection::ReadCLientMessage(QDataStream &in, const int &t_typeMsg)
{
    try
    {
        QString strMsg, strSender, strReciever;
        QVector<QString> strVec;
        QByteArray file;

        in>>strMsg;

        if(t_typeMsg == TypeMsg::L_FILE)
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

        if(t_typeMsg == TypeMsg::L_FILE)
            arrBlock.append(file);


        SendToClient(t_typeMsg, m_rMapClients.value(strReciever).pClientSocket, arrBlock);
    }
    catch(const std::exception &ex)
    {
        //write error in log file
        WriteToLogFile(QString(ex.what()));
    }
}

void YAServer::ClientConnection::SendToClient(const int &t_typeMsg, QSslSocket *t_socket, const QByteArray &t_arrBlockMsg)
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
