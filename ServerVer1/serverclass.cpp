#include "serverclass.h"

ServerClass::ServerClass( QObject *parent) : QObject(parent), m_ptxtInfo(nullptr), m_nNextBlockSize(0)
{

}


bool ServerClass::StartServer(int nPort, QTextBrowser* txtInfo)
{
    m_pTcpServer = new QTcpServer();
    this->setTxtBrowser(txtInfo);

    if(!m_pTcpServer->listen(QHostAddress::Any, nPort))
    {
        qDebug()<<m_pTcpServer->errorString();
        this->m_ptxtInfo->append(m_pTcpServer->errorString());
        m_pTcpServer->close();
        return false;
    }
    else if(!this->m_dbManager.ConnectToDataBase())
    {
        qDebug()<<this->m_dbManager.GetLastError();
        this->m_ptxtInfo->append(this->m_dbManager.GetLastError());
        return false;
    }
    qDebug()<<"Server started";
    this->m_dbManager.FillMapUsername(this->m_mapClients);

    connect(this->m_pTcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    return true;
}


void ServerClass::ReadClientREG_LOG(QDataStream &in, QTcpSocket* pClientSocket, const MsgType &typeMsg)
{
    UserInfo StructUI;
    QString strMsg;
    QVector<QString> strVec;

    in>>strMsg;

    strVec = strMsg.split('.').toVector();

    if(!strVec.size())
    {
        return;
    }

    if(typeMsg == REG)
    {


        StructUI.name = strVec[2];
        StructUI.surName = strVec[3];

        StructUI.pClientSocket = pClientSocket;


        if(this->m_dbManager.isUsernameBusy(strVec[0]))
        {
            this->SendToClient(pClientSocket, REG_ERROR, NULL);
            return;
        }


        this->m_dbManager.WriteToDataBase(strVec[0], strVec[1], StructUI);


        qDebug()<<"Client "<<strVec[0]<<" registrate at server";
        this->m_ptxtInfo->append("Client " + strVec[0] +" registrate at server");
    }
    else if(typeMsg == LOGIN)
    {

        if(!this->m_dbManager.isCorrectLogin(strVec[0], strVec[1]))
        {
            this->SendToClient(pClientSocket, LOG_ERROR, NULL);
            return;
        }


        StructUI = this->m_mapClients.value(strVec[0]);
        StructUI.pClientSocket = pClientSocket;

        qDebug()<<"Client "<<strVec[0]<<" login to the server";
        this->m_ptxtInfo->append("Client "+strVec[0]+" login to the server");
    }


    this->m_mapClients.insert(strVec[0], StructUI);


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(strVec[0], StructUI);


    foreach(auto currUsername,this->m_mapClients.keys())
    {
        if(currUsername == strVec[0])
        {

            arrBlock.clear();
            out.device()->reset();
            out<<this->m_mapClients;
            this->SendToClient(pClientSocket, OK, arrBlock);


            arrBlock.clear();
            out.device()->reset();
            out<<qMakePair(strVec[0], StructUI);
        }
        else if(this->m_mapClients.value(currUsername).pClientSocket)
        {
            this->SendToClient(this->m_mapClients.value(currUsername).pClientSocket, UPDATE, arrBlock);
        }
    }
}



void ServerClass::ReadClientMESSAGE(QDataStream &in)
{
    QString strMsg, strSender, strReciever;
    QVector<QString> strVec;
    in>>strMsg;

    strVec = strMsg.split('.').toVector();
    strSender = strVec[0];
    strReciever = strVec[1];


    qDebug()<<"Client "<<strSender<<" send message to "<<strReciever;
    this->m_ptxtInfo->append("Client "+strSender+" send message to "+strReciever);

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    strMsg = strVec[0] + '.' + strVec[2];

    out<<strMsg;


    SendToClient(this->m_mapClients.value(strVec[1]).pClientSocket, MESSAGE, arrBlock);

}



void ServerClass::SendToClient(QTcpSocket *pClientSocket,const MsgType &type, const QByteArray &arrBlockMsg)
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


QString ServerClass::GetIP()
{
    auto address = QNetworkInterface::allAddresses();

    for(int i = 0; i<address.size();++i)
    {
        if(address.at(i)!=QHostAddress::LocalHost&&address.at(i).toIPv4Address())
        {
            return address.at(i).toString();
        }
    }
    return QHostAddress(QHostAddress::LocalHost).toString();
}


void ServerClass::setTxtBrowser(QTextBrowser *txtInfo)
{
    m_ptxtInfo = txtInfo;
}


void ServerClass::slotNewConnection()
{
    QTcpSocket* pClientSocket = this->m_pTcpServer->nextPendingConnection();

    connect(pClientSocket, SIGNAL(disconnected()), SLOT(slotDissconnectClient()));
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), SLOT(slotReadClient()));
    qDebug()<<"Client connect to Server";
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
        if(!this->m_nNextBlockSize)
        {
            if(pClientSocket->bytesAvailable()<sizeof(quint32))
                break;
            in>>m_nNextBlockSize;
        }

        if(pClientSocket->bytesAvailable()<m_nNextBlockSize)
            break;

        in>>typeMsg>>time;

        if(typeMsg == MESSAGE)
            ReadClientMESSAGE(in);
        else
            ReadClientREG_LOG(in, pClientSocket, static_cast<MsgType>(typeMsg));
    }

    m_nNextBlockSize = 0;
}


void ServerClass::slotDissconnectClient()
{
    QTcpSocket* pClientSocket = static_cast<QTcpSocket*>(sender());

    QMap<QString,UserInfo>::iterator it;

    for(it = m_mapClients.begin(); it != m_mapClients.end(); ++it)
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
    it.value().pClientSocket = nullptr;
    m_ptxtInfo->append("Client "+it.key()+" dissconnect from the server");

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

