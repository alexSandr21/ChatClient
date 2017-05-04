#include "stdafx.h"
#include "yaserver.h"
#include <QFile>
#include <QSslError>

namespace YAServer{

QByteArray certByte("-----BEGIN CERTIFICATE-----\r\n"
                    "MIIFiDCCA3ACAQEwDQYJKoZIhvcNAQELBQAwgYYxCzAJBgNVBAYTAlVBMQ8wDQYD\r\n"
                    "VQQIDAZEbmlwcm8xDzANBgNVBAcMBkRuaXBybzEOMAwGA1UECgwFWUFvcmcxDjAM\r\n"
                    "BgNVBAsMBVlBb3JnMRIwEAYDVQQDDAlZQW9yZy5jb20xITAfBgkqhkiG9w0BCQEW\r\n"
                    "EmUucnVrc292QGdtYWlsLmNvbTAeFw0xNzA0MjgxNTIyMThaFw0xODA0MjgxNTIy\r\n"
                    "MThaMIGMMQswCQYDVQQGEwJVQTEPMA0GA1UECAwGRG5pcHJvMQ8wDQYDVQQHDAZE\r\n"
                    "bmlwcm8xDjAMBgNVBAoMBVlBb3JnMREwDwYDVQQLDAhZQW9yZ0RlbDEVMBMGA1UE\r\n"
                    "AwwMWUFvcmdEZWwuY29tMSEwHwYJKoZIhvcNAQkBFhJlLnJ1a3NvdkBnbWFpbC5j\r\n"
                    "b20wggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQClbWvR5Lc+2c9OZyv3\r\n"
                    "+8ZyXxLzE/RgjfqQvbvJZaloJB6CRQKBlMyz1RRAY5nu9tlZ0HRXXWDfBatpVtna\r\n"
                    "//Ap28rBcah60XW4Yu0eEd18ywKBkUad0yuSe34mfY3uoLyek3vTJvEmTSC6bbo8\r\n"
                    "Lz60WxpzooASZQhesVD2qM0MFcFSMoeEXBqsQxN7EN0BkWpc7Vk5VxGvWyNw6Aei\r\n"
                    "tpT3NsWwLH5fTnzzPHiVwWffnd6Q3S7CiggvVvECUL+9s4jmmPyHpYo/1ED4hRm8\r\n"
                    "27b2Vqow5tVMf/2AU5UMrlkzu/t0Re+GBOGm/pr8Nia4npx+aOb3GR4ZxjsOZSvO\r\n"
                    "rrQ3pcPdM7U0xPrfHJsvArk5/DAk9LO8is+qT+S0cHpmSw7v7hbHoJjTmY/RkzI8\r\n"
                    "5ZCYVhONl7uoQiWGvGOjiEJCo2dCYGhR1cY8u0i1h+w1R9H5Z00JQup9+2JW2AAN\r\n"
                    "mUKbt0pfssIUwNc52AwUsQjN/6S5HwUnET64JoC3O7rZCFX6K8v+nNH88E69qSjK\r\n"
                    "qeWfjqOV8wAKBjFBkeS2r0knhQp8Nkn/cWw7bJfG45NosVRmeZZr5ryh8/lPX8/r\r\n"
                    "sqRAQUARYgn4GPPI2VKYqCuFDf3EZKsZwM9rsF0C5ExxKofQ/rINP9iMsnzoHKJq\r\n"
                    "JN+Q9BnizN6Z6A191M9+q/eO4QIDAQABMA0GCSqGSIb3DQEBCwUAA4ICAQClMUaU\r\n"
                    "y7HxU67Wg5RCI8rhEY1jzpmwmwJC2mq94zLPPRYXDfQDf57Nlt51Hy3Z3BJ68KQD\r\n"
                    "OeyvBLeGDYZoEcfPLXexQJ+3Xxu0ZVikugqxa7s737Bv4B3XSgzpaAZMfURjlYM1\r\n"
                    "l8VmfTv6g+FIAdLxMsgu4B4oa0lu4a5OtftVFkHT+7JRmXwyIi0cLNrS68nMePB5\r\n"
                    "vnFMQpG04M+adrwa+u4GY9V/ky6EtIy0SAHS675Bt065RMgtug5sH31HJng51B8D\r\n"
                    "Iel+BsQH54+4ynqVQz4liYzEabRvLbBvqokvJVfsuBxYo1EMPW8YFUsaJsmuhdSz\r\n"
                    "SJIX42pvzvpF8uTmjN6If4xHRzP0LgZrfNUq9Tkochiql+K/cW5ET5zwjPvOITYI\r\n"
                    "IXbX6OWnNuKy9kaKOTWUSumqQpaUJa8lhbAkfKMIOHFnsaLtiTNbNt1uf3h7Su68\r\n"
                    "+StGcP0yDZsZZfJRwgZiMTv6S+A7WWyVSZjPz1d4muz6eOVX5GofL8HnhomN64JU\r\n"
                    "xUgk3emDiio6DrI90dkPk3+q4hflsvi+wN7M5R5mf6xLLPQ2+pC7g6f1ENjGGSYY\r\n"
                    "2g8/Kce+pOCxfYCxy1kEh6ia0cS6+4ajuI92Q5sQEYaCIkCoRZgF+iKXmEkgOmkr\r\n"
                    "UGKxnXxDYrm2zcQaWz5ATMaLFFNfb02AuSuc9g==\r\n"
                    "-----END CERTIFICATE-----");

QByteArray privKey("-----BEGIN RSA PRIVATE KEY-----\r\n"
                   "MIIJKAIBAAKCAgEApW1r0eS3PtnPTmcr9/vGcl8S8xP0YI36kL27yWWpaCQegkUC\r\n"
                   "gZTMs9UUQGOZ7vbZWdB0V11g3wWraVbZ2v/wKdvKwXGoetF1uGLtHhHdfMsCgZFG\r\n"
                   "ndMrknt+Jn2N7qC8npN70ybxJk0gum26PC8+tFsac6KAEmUIXrFQ9qjNDBXBUjKH\r\n"
                   "hFwarEMTexDdAZFqXO1ZOVcRr1sjcOgHoraU9zbFsCx+X0588zx4lcFn353ekN0u\r\n"
                   "wooIL1bxAlC/vbOI5pj8h6WKP9RA+IUZvNu29laqMObVTH/9gFOVDK5ZM7v7dEXv\r\n"
                   "hgThpv6a/DYmuJ6cfmjm9xkeGcY7DmUrzq60N6XD3TO1NMT63xybLwK5OfwwJPSz\r\n"
                   "vIrPqk/ktHB6ZksO7+4Wx6CY05mP0ZMyPOWQmFYTjZe7qEIlhrxjo4hCQqNnQmBo\r\n"
                   "UdXGPLtItYfsNUfR+WdNCULqfftiVtgADZlCm7dKX7LCFMDXOdgMFLEIzf+kuR8F\r\n"
                   "JxE+uCaAtzu62QhV+ivL/pzR/PBOvakoyqnln46jlfMACgYxQZHktq9JJ4UKfDZJ\r\n"
                   "/3FsO2yXxuOTaLFUZnmWa+a8ofP5T1/P67KkQEFAEWIJ+BjzyNlSmKgrhQ39xGSr\r\n"
                   "GcDPa7BdAuRMcSqH0P6yDT/YjLJ86ByiaiTfkPQZ4szemegNfdTPfqv3juECAwEA\r\n"
                   "AQKCAgATNYOBmedVWwFhf0VxxECk4zlWFIkZ9SKVUBLQLgff2eHZCp5h4Z9remCh\r\n"
                   "oW8seml+rl/0DRnEi+n9j2I4teNzWNxQXhxMNqpOwN370eNALGqcFPx4Mx8qIUDi\r\n"
                   "w02J2gxM9Z85Q5pY8jfMrhUkC/BiDPojV0k72ZOBO2HFYpB8yMHsA4YM7o0CV7+Z\r\n"
                   "WLnxBqutIrprmBqrnA+ferH4SO8/TZLjBCegdYK409rEuPKQ2soPVD+rw09z/8p4\r\n"
                   "lFvPcVuCO12Hesd05fatZPc6IOE79ZBJoWQ/P9V14z65fD1LbpaiR8SqcUKqm4Hv\r\n"
                   "q3IPmgtd+lxncPk30PbmkwTlORAf/jyn/KtpAAbujrQZeSgONNoD79bkcuWmjMMv\r\n"
                   "14K8ZJ+UZXY5d5AGxyx+XtmnOVXYUtOBtDifgCJsmYZO0MsGLPEfi8D/pTFX1U1V\r\n"
                   "h/+li4z2n+MbPNpSVNnL2WVnvkLAmZQqyF+zEWwSdoccAA2yBSkfFCkcGGBedxCA\r\n"
                   "3s9Kf3UdUcicCaUH5RH5z0PYZTdHHwHF1nQrB80gDw5PIRTsJpj91tLF73aUJCrL\r\n"
                   "19Ryiuu2XnJ3AfqL9jSmcoXZF96ovAM3j5nDvaqsknT2IHiWVrqNhBpiAhUMky14\r\n"
                   "BO2aH1YAtPo+vBvg7V1FvFY8v6nayFLRZ0mrimt0XSKXWLsngQKCAQEA1+v90LtA\r\n"
                   "V2+pJplhAHWmlm6/HpFp/tIqXIi8y9Rax/pxB8VRoqyu/AHQynO7ac2jFzpWova5\r\n"
                   "kAPlw2eepNJs+FjcMK7icG/9e85oBYp5DDheTIUZuhM7N0IHkuSOZ7obRgWftrDd\r\n"
                   "3GrO5cwAjb4SbepeWh9950kiPu2ULGjZk1OVx29b8f3uquldB65cvhFpaUf2k4V+\r\n"
                   "g9bhLObLt6AF8+gofmXRRJjBPA+ZqqDJAm6xN2os6/dlJArsvQSMmdK2jHf7Qv5K\r\n"
                   "EYByTeTdE9D5NGWIPC9baMbuzk+krDv0g2MZ62Tz5xeZ1rhhlpQ21U7BUiTe1UGK\r\n"
                   "vdgr2H6wWuixeQKCAQEAxCITJcWfRT7UFLtbUlfH+tcaSTjZ/QRhhrYI6R6/kxf1\r\n"
                   "v4tKVZrsu5YGUrwDkcscPsl0DjCoKTgkkuPr4/sBEJruMmWxHzKwUuHiwjDWkPYK\r\n"
                   "5SsASeG3dRFq0IlTm3t83ZA5z3HHpBDJUd6ShIWIaGEzXgUSaC13g1zG6NStwQx1\r\n"
                   "P8OUZTmYus3EKcyFs9zyUXakAfWeAx8bj6KLpv2GozGm8yTyBNgzQGDihUz311Lt\r\n"
                   "XVNr1yCJ0ojWhQv5HDDaa2F/uSOnUfuSYr1wlN4vkKId7Wo/PIwh6bh84qG1eQ1v\r\n"
                   "UnHpKZHimim+pfJx0TGYUpSLmZZe43awoOrcVWoWqQKCAQBmPjhE87Ym8Rfe0Dgv\r\n"
                   "mIZWq4BSgDb51Bj9Ej6cIiUOlteY/2WFq1F8wXtVsnGnSxmUMSq/Jid+O2o8Gj/z\r\n"
                   "+jRhcOprHxTYex9D1dxJ62UgVTzJ7SjehKbApsKhWxrXoTey84zKXHviGK16ARyu\r\n"
                   "AbgGVjlxYYIj1+ygca19TfThH25ILsISF9VBFxnXPkhqNCJ/GPV4q3jaAxFDStJi\r\n"
                   "VCMdeOxb1ei/G7o8r4JrWpF0VmgSPnOhFNfOtmGdq57MZAvO/i17bqAyI2l82weY\r\n"
                   "j5bBqeZV4XLebXy9MtdeO1GwOWgXoAkif3l4RHzpaauBXDaMqCrgPpXtCYTRkkP9\r\n"
                   "+Du5AoIBAFP7OzOrQR2HX4xZBStnBuwzer/rpfufCpuCCqK3VdSIJ+QSoouxQLi5\r\n"
                   "7o8yjly5GakQ3a7NytOLhK2ZBoKiEbyoemH+BS+Upsd2jYAVJWsAz319QixgiAuK\r\n"
                   "n7AXfUNQRxEgFoDq+U8ZGPsDUL2J5/S52iLiDRWqkkgfS9SabKQCz5m0BzCozur9\r\n"
                   "/IbhWka/zspvAJ8OKtCvWz/Pm/ivCh2Kvy/RnAfTA28HjTcC1ATRL24MsFp3Rkxt\r\n"
                   "510k2U/m4z+jiesehyCllfLNpw03biqCIEpuehFU95h6NI9eKrRgulAVEn+IRf04\r\n"
                   "7rdj7rRihhSmInhrWMoRImRk7F9ug7ECggEBANbbaczyAH//BzfLXX0NhrvMrEnH\r\n"
                   "qLl9iS9VJmOqxESEZ8mdIM67GsZY+i5Ummik8dZH8ohCTFbpfvrkXLaQZcmPvF2s\r\n"
                   "bNwW8SHCHnffa1qi9kcNu7mRfzfYzlQJUtJ0KuG28MHXG9WncsQoKUP1xZaxIoWe\r\n"
                   "EfP5A8bGKKpePS7kRaNKT+Vzra5I7lVaORvc/E/s+TI9pt1HGVL3pbb7hxlq/gx/\r\n"
                   "hvL8yJmlLMvM6LuPKpYJ/eY8TsND3cEtKYx92ZsoTQJ5qlyvgtJvYjM4qQSLriOH\r\n"
                   "vI98KTz7Lv9KhsQgud7BMYb2usswjtVy257UMY2olENSl6F4GNq/BLypkK4=\r\n"
                   "-----END RSA PRIVATE KEY-----");


YAServer::YAServer(std::shared_ptr<QFile> t_pLog, QObject *parent) : QObject(parent), m_nNextBlockSize(0)
{
    m_pLogFile=t_pLog;
        QSslError err(QSslError::SslError::CertificateUntrusted);
        qDebug()<<err.errorString();
}

bool YAServer::StartServer(int nPort)
{
    m_pTcpServer.reset(new QTcpServer(this));
    m_dialog.show();
    m_dialog.SetIpInfo(GetIP());

    if(!m_pTcpServer->listen(QHostAddress::Any, nPort))
    {
        qDebug()<<m_pTcpServer->errorString();
        m_dialog.WriteToTextBrowser(m_pTcpServer->errorString());

        if(m_pLogFile->isOpen())
        {
            m_pLogFile->write(QByteArray::fromStdString(m_pTcpServer->errorString().toStdString() + "\r\n"));
        }

        m_pTcpServer->close();
        return false;
    }
    else if(!m_dbManager.ConnectToDataBase())
    {
        qDebug()<<m_dbManager.GetLastError();
        m_dialog.WriteToTextBrowser(m_dbManager.GetLastError());

        if(m_pLogFile->isOpen())
        {

            m_pLogFile->write(QByteArray::fromStdString(m_dbManager.GetLastError().toStdString() + "\r\n"));
        }

        return false;
    }



    m_Certificate = QSslCertificate(certByte);
    if(m_Certificate.isNull())
    {
        qDebug()<<"Can`t read certificate";
        if(m_pLogFile->isOpen())
        {
            m_pLogFile->write("Error: Can`t read certificate\r\n");
        }
        return false;
    }



    m_privKey = QSslKey(privKey, QSsl::Rsa);
    if(m_privKey.isNull())
    {
        qDebug()<<"Can`t read private key";
        m_pLogFile->write("Error: Can`t read Private key\r\n");
        return false;
    }

    qDebug()<<"Server started";
    m_dbManager.FillMapUsername(m_mapClients);

    connect(m_pTcpServer.get(), &QTcpServer::newConnection, this, &YAServer::slotNewConnection);

    return true;
}

QString YAServer::GetIP()
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

void YAServer::slotNewConnection()
{
    QSslSocket* pClientSocket = new QSslSocket(this);


    if(pClientSocket->setSocketDescriptor(m_pTcpServer->nextPendingConnection()->socketDescriptor()))
    {
        connect(pClientSocket, &QSslSocket::encrypted, this, &YAServer::slotVerifyReady);
        connect(pClientSocket, &QSslSocket::disconnected,this, &YAServer::slotDissconnectClient);
        connect(pClientSocket, &QSslSocket::disconnected, pClientSocket, &QSslSocket::deleteLater);
        connect(pClientSocket, &QSslSocket::readyRead,this, &YAServer::slotReadClient);


        pClientSocket->setProtocol(QSsl::AnyProtocol);
        pClientSocket->setLocalCertificate(m_Certificate);
        pClientSocket->setPrivateKey(m_privKey);

        pClientSocket->startServerEncryption();

        qDebug()<<"Client connect to Server";
    }
    else
    {
        //write error in log file
        if(m_pLogFile->isOpen())
        {
            m_pLogFile->write(QByteArray::fromStdString(pClientSocket->errorString().toStdString() + "\r\n"));
        }
    }
}

void YAServer::slotReadClient()
{
    QSslSocket* pClientSocket = static_cast<QSslSocket*>(sender());

    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    int typeMsg = 0;
    QTime time;



    for(;;)
    {
        if(!m_nNextBlockSize)
        {
            if(pClientSocket->bytesAvailable()<sizeof(uint))
                break;
            in>>m_nNextBlockSize;
        }

        if( pClientSocket->bytesAvailable()<m_nNextBlockSize)
            break;

        in>>typeMsg>>time;

        if(typeMsg == MESSAGE||typeMsg == L_FILE)
            ReadClientMESSAGE(in, typeMsg);

        else if(typeMsg == REG || typeMsg == LOGIN)
            ReadClientREG_LOG(in, pClientSocket,typeMsg);

        m_nNextBlockSize = 0;

    }
}

void YAServer::slotDissconnectClient()
{
    QSslSocket* pClientSocket = static_cast<QSslSocket*>(sender());

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

void YAServer::slotVerifyReady()
{
    if(static_cast<QSslSocket*>(sender())->isEncrypted())
        qDebug()<<"Client succes verified";
}

void YAServer::ReadClientREG_LOG(QDataStream &in, QSslSocket *pClientSocket, const int &typeMsg)
{
    try
    {
    ClientInfo::ClientInfo StructUI;
    QString strMsg;
    QVector<QString> strVec;

    in>>strMsg;

    strVec = strMsg.split(ClientInfo::DELIM).toVector();

    if(!strVec.size())
    {
        //write error in log file
        if(m_pLogFile->isOpen())
        {
            m_pLogFile->write("Error: Can`t correct split client message\r\n");
            if(typeMsg == REG)
            {
                SendToClient(pClientSocket, REG_ERROR);
            }
            else
            {
                SendToClient(pClientSocket, LOG_ERROR);
            }
        }
        return;
    }

    if(typeMsg == REG)
    {


        StructUI.name = strVec[2];
        StructUI.surName = strVec[3];

        StructUI.pClientSocket = pClientSocket;


        if(m_dbManager.IsUsernameBusy(strVec[0]))
        {
            SendToClient(pClientSocket, REG_ERROR);
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
            SendToClient(pClientSocket, LOG_ERROR);
            return;
        }


        StructUI = m_mapClients.value(strVec[0]);

        if(StructUI.pClientSocket)
        {
            SendToClient(pClientSocket, LOG_ERROR);
            return;
        }

        StructUI.pClientSocket = pClientSocket;

        qDebug()<<"Client "<<strVec[0]<<" login to the server";
        m_dialog.WriteToTextBrowser("Client "+strVec[0]+" login to the server");
    }
    else
    {
        //write label error in log file
        if(m_pLogFile->isOpen())
        {
            m_pLogFile->write("Error: Received unexpected data\r\n");
        }
        return;
    }


    m_mapClients.insert(strVec[0], StructUI);


    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    out<<qMakePair(strVec[0], StructUI);


    for(auto currUsername:m_mapClients.keys())
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
    catch(const std::exception &ex)
    {
        QString error = ex.what();
        qDebug()<<error;
        if(m_pLogFile->isOpen())
        {
            m_pLogFile->write(QByteArray::fromStdString("Error: " + error.toStdString() + "\r\n"));
        }
    }
}

void YAServer::ReadClientMESSAGE(QDataStream &in, const int &typeMsg)
{
    try
    {
        QString strMsg, strSender, strReciever;
        QVector<QString> strVec;
        QByteArray file;

        in>>strMsg;

        if(typeMsg == L_FILE)
            file = in.device()->readAll();

        strVec = strMsg.split(ClientInfo::DELIM).toVector();
        strSender = strVec[0];
        strReciever = strVec[1];


        qDebug()<<"Client "<<strSender<<" send message to "<<strReciever;
        m_dialog.WriteToTextBrowser("Client "+strSender+" send message to "+strReciever);

        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        strMsg = strVec[0] + ClientInfo::DELIM + strVec[2];

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

void YAServer::SendToClient(QSslSocket *pClientSocket, const int &type, const QByteArray &arrBlockMsg)
{
    try
    {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
        out<<uint(0)<<type<<QTime::currentTime();


        if(!arrBlockMsg.isEmpty())
            arrBlock.append(arrBlockMsg);

        out.device()->seek(0);
        out<<uint(arrBlock.size() - sizeof(uint));

        pClientSocket->write(arrBlock);
        qDebug()<<pClientSocket->waitForBytesWritten();
    }
    catch(const std::exception &ex)
    {
        //write error in log file
    }
}


}
