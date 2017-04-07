#include "stdafx.h"
#include "serverclass.h"


ServerClass::ServerClass( QObject *parent) : QObject(parent), m_nNextBlockSize(0)
{

}


bool ServerClass::StartServer(int nPort)
{
    m_pTcpServer = new QTcpServer();
    m_dialog.show();
    m_dialog.SetIpInfo(GetIP());

    if(!m_pTcpServer->listen(QHostAddress::Any, nPort))
    {
        qDebug()<<m_pTcpServer->errorString();
        m_dialog.WriteToTextBrowser(m_pTcpServer->errorString());
        m_pTcpServer->close();
        return false;
    }
    else if(!m_dbManager.ConnectToDataBase())
    {
        qDebug()<<m_dbManager.GetLastError();
        m_dialog.WriteToTextBrowser(m_dbManager.GetLastError());
        return false;
    }
    qDebug()<<"Server started";
    m_dbManager.FillMapUsername(m_mapClients);

    connect(m_pTcpServer, &QTcpServer::newConnection, this, &ServerClass::slotNewConnection);

    return true;
}


void ServerClass::ReadClientREG_LOG(QDataStream &in, QTcpSocket* pClientSocket, const int &typeMsg)
{
    UserInfo StructUI;
    QString strMsg;
    QVector<QString> strVec;

    in>>strMsg;

    strVec = strMsg.split(DELIM).toVector();

    if(!strVec.size())
    {
        //write error in log file
        return;
    }

    if(typeMsg == REG)
    {


        StructUI.name = strVec[2];
        StructUI.surName = strVec[3];

        StructUI.pClientSocket = pClientSocket;


        if(m_dbManager.IsUsernameBusy(strVec[0]))
        {
            SendToClient(pClientSocket, REG_ERROR, NULL);
            return;
        }


        m_dbManager.WriteToDataBase(strVec[0], strVec[1], StructUI);


        qDebug()<<"Client "<<strVec[0]<<" registrate at server";
        m_dialog.WriteToTextBrowser("Client " + strVec[0] +" registrate at server");
    }
    else if(typeMsg == LOGIN)
    {

        if(!m_dbManager.IsCorrectLogin(strVec[0], strVec[1]))
        {
            SendToClient(pClientSocket, LOG_ERROR, NULL);
            return;
        }


        StructUI = m_mapClients.value(strVec[0]);
        StructUI.pClientSocket = pClientSocket;

        qDebug()<<"Client "<<strVec[0]<<" login to the server";
        m_dialog.WriteToTextBrowser("Client "+strVec[0]+" login to the server");
    }
    else
    {
        //write label error in log file
        return;
    }


    m_mapClients.insert(strVec[0], StructUI);


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(strVec[0], StructUI);


    foreach(auto currUsername,m_mapClients.keys())
    {
        if(currUsername == strVec[0])
        {

            arrBlock.clear();
            out.device()->reset();
            out<<m_mapClients;
            SendToClient(pClientSocket, OK, arrBlock);


            arrBlock.clear();
            out.device()->reset();
            out<<qMakePair(strVec[0], StructUI);
        }
        else if(m_mapClients.value(currUsername).pClientSocket)
        {
            SendToClient(m_mapClients.value(currUsername).pClientSocket, UPDATE, arrBlock);
        }
    }
}



void ServerClass::ReadClientMESSAGE(QDataStream &in, const int &typeMsg)
{
    try
    {
        QString strMsg, strSender, strReciever;
        QVector<QString> strVec;
        QByteArray file;

        in>>strMsg;

        if(typeMsg == L_FILE)
            file = in.device()->readAll();

        strVec = strMsg.split(DELIM).toVector();
        strSender = strVec[0];
        strReciever = strVec[1];


        qDebug()<<"Client "<<strSender<<" send message to "<<strReciever;
        m_dialog.WriteToTextBrowser("Client "+strSender+" send message to "+strReciever);

        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        strMsg = strVec[0] + DELIM + strVec[2];

        out<<strMsg;

        if(typeMsg == L_FILE)
            arrBlock.append(file);


        SendToClient(m_mapClients.value(strVec[1]).pClientSocket, typeMsg, arrBlock);
    }
    catch(const std::exception &ex)
    {
        //write error in log file
    }

}



void ServerClass::SendToClient(QTcpSocket *pClientSocket,const int &type, const QByteArray &arrBlockMsg)
{
    try
    {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out<<quint32(0)<<type<<QTime::currentTime();


        if(!arrBlockMsg.isEmpty())
            arrBlock.append(arrBlockMsg);

        out.device()->seek(0);
        out<<quint32(arrBlock.size() - sizeof(quint32));

        pClientSocket->write(arrBlock);
    }
    catch(const std::exception &ex)
    {
        //write error in log file
    }
}


QString ServerClass::GetIP()
{
    auto address = QNetworkInterface::allAddresses();

    for(int i = 0; i<address.size();++i)
    {
        if(address.at(i)!=QHostAddress::LocalHost&&address.at(i).toIPv4Address())
        {
            return address.at(i).toString() + " : 3004";
        }
    }
    return QHostAddress(QHostAddress::LocalHost).toString() + " : 3004";
}


void ServerClass::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_pTcpServer->nextPendingConnection();

    if(pClientSocket)
    {
        connect(pClientSocket, &QTcpSocket::disconnected,this, &ServerClass::slotDissconnectClient);
        connect(pClientSocket, &QTcpSocket::disconnected, pClientSocket, &QTcpSocket::deleteLater);
        connect(pClientSocket, &QTcpSocket::readyRead,this, &ServerClass::slotReadClient);
        qDebug()<<"Client connect to Server";
    }
    else
    {
        //write error in log file
    }

}


void ServerClass::slotReadClient()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());

    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    int typeMsg = 0;
    QTime time;



    for(;;)
    {
        if(!m_nNextBlockSize)
        {
            if(pClientSocket->bytesAvailable()<sizeof(quint32))
                break;
            in>>m_nNextBlockSize;
        }

        if(pClientSocket->bytesAvailable()<m_nNextBlockSize)
            break;

        in>>typeMsg>>time;

        if(typeMsg == MESSAGE||typeMsg == L_FILE)
            ReadClientMESSAGE(in, typeMsg);
        else
            ReadClientREG_LOG(in, pClientSocket,typeMsg);

         m_nNextBlockSize = 0;
    }
}


void ServerClass::slotDissconnectClient()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());

    auto it = m_mapClients.begin();

    for( ; it != m_mapClients.end(); ++it)
    {
        if(it.value().pClientSocket == pClientSocket)
        {
            break;
        }
    }

    if(it == m_mapClients.end())
    {
        return;
    }

    it.value().pClientSocket->close();
    it.value().pClientSocket = nullptr;
    m_dialog.WriteToTextBrowser("Client "+it.key()+" disconnect from the server");

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(it.key(),it.value());

    for(it = m_mapClients.begin();it!= m_mapClients.end();++it)
    {
        if(it.value().pClientSocket)
            SendToClient(it.value().pClientSocket, UPDATE, arrBlock);
    }
}
