#include "stdafx.h"
#include "server.h"

#include <QFile>


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



Connection::Server::Server(std::shared_ptr<QFile> t_pLogFile, QObject *parent) : QTcpServer(parent), m_port(1234)
{
    m_pLogFile = t_pLogFile;
}

bool Connection::Server::StartServer(const int &t_Port)
{
    m_port = t_Port;
    m_dialog.show();
    m_dialog.SetIpInfo(GetIP());
    m_dbManager.SetLogFile(m_pLogFile);

    if(!listen(QHostAddress::Any, m_port))
    {
        qDebug()<<errorString();
        m_dialog.WriteToTextBrowser(errorString());

        WriteToLogFile(errorString());
        close();
        return false;
    }
    else if(!m_dbManager.ConnectToDataBase())
    {
        qDebug()<<m_dbManager.GetLastError();
        m_dialog.WriteToTextBrowser(m_dbManager.GetLastError());

        WriteToLogFile(m_dbManager.GetLastError());
        return false;
    }

    QSslCertificate cert(certByte);

//    QFile file("test.cert");
//    if(file.open(QIODevice::ReadOnly))
//    {
//       cert = QSslCertificate(file.readAll());
//    }


    if(cert.isNull())
    {
        qDebug()<<"Can`t read certificate";
        WriteToLogFile("Error: Can`t read certificate");
        return false;
    }

    QSslKey key(privKey, QSsl::Rsa);

//    QFile file2("test.key");
//    if(file2.open(QIODevice::ReadOnly))
//    {
//        key = QSslKey(file2.readAll(), QSsl::Rsa);
//    }
    if(key.isNull())
    {
        qDebug()<<"Can`t read private key";
        WriteToLogFile("Error: Can`t read Private key");
        return false;
    }

    m_sslConfig.setProtocol(QSsl::AnyProtocol);
    m_sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
    m_sslConfig.setLocalCertificate(cert);
    m_sslConfig.setPrivateKey(key);

    qDebug()<<"Server started";
    m_dialog.WriteToTextBrowser("Server started");
    m_dbManager.FillMapUsername(m_mapClients);

    return true;
}

QString Connection::Server::GetIP()
{
    auto address = QNetworkInterface::allAddresses();

    for(int i = 0; i<address.size();++i)
    {
        if(address.at(i)!=QHostAddress::LocalHost&&address.at(i).toIPv4Address())
        {
            return address.at(i).toString() + " : " + QString(std::to_string(m_port).c_str());
        }
    }
    return QHostAddress(QHostAddress::LocalHost).toString() + " : " + QString(std::to_string(m_port).c_str());
}

void Connection::Server::incomingConnection(qintptr handle)
{
    ClientConnection* client = new ClientConnection(m_dialog, m_dbManager, m_mapClients, m_pLogFile, this);

    if(!client->SetSocketDescriptor(handle, m_sslConfig))
    {
        WriteToLogFile("Can't create client socket");
        delete client;
    }
    else
    {
        addPendingConnection(client->socket());
    }

}

void Connection::Server::WriteToLogFile(const QString &t_errorMsg)
{
    if(m_pLogFile->isOpen())
    {
        m_pLogFile->write(QByteArray::fromStdString(t_errorMsg.toStdString() + "\r\n"));
    }
}
