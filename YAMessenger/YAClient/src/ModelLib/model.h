#pragma once

#include <QTime>
#include <QMap>
#include <QObject>
#include <QPair>
#include <memory>
#include <QFile>
#include <QVector>
#include <QList>
#include <QTcpSocket>
#include <QSslSocket>
#include <QSsl>
#include <QDebug>
#include "clientinfo.h"


namespace YAClient
{
    const QByteArray certByte("-----BEGIN CERTIFICATE-----\r\n"
                          "MIIF5DCCA8ygAwIBAgIJAL0Nmj/CxfHyMA0GCSqGSIb3DQEBCwUAMIGGMQswCQYD\r\n"
                          "VQQGEwJVQTEPMA0GA1UECAwGRG5pcHJvMQ8wDQYDVQQHDAZEbmlwcm8xDjAMBgNV\r\n"
                          "BAoMBVlBb3JnMQ4wDAYDVQQLDAVZQW9yZzESMBAGA1UEAwwJWUFvcmcuY29tMSEw\r\n"
                          "HwYJKoZIhvcNAQkBFhJlLnJ1a3NvdkBnbWFpbC5jb20wHhcNMTcwNDI4MTUwNzQz\r\n"
                          "WhcNMTgwNDI4MTUwNzQzWjCBhjELMAkGA1UEBhMCVUExDzANBgNVBAgMBkRuaXBy\r\n"
                          "bzEPMA0GA1UEBwwGRG5pcHJvMQ4wDAYDVQQKDAVZQW9yZzEOMAwGA1UECwwFWUFv\r\n"
                          "cmcxEjAQBgNVBAMMCVlBb3JnLmNvbTEhMB8GCSqGSIb3DQEJARYSZS5ydWtzb3ZA\r\n"
                          "Z21haWwuY29tMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAqSnDF71H\r\n"
                          "ypgJRueQIn41TwkJupNjvMfncPTIGtBg/PMNnvD0i651bZg8EMNkk9PXOywEcMiO\r\n"
                          "fhw8E+bjn2L/sslxixYYKIVF+kYALkgGoHx3xZ+VGIa5DG96hHc18Isfxa5A8dU8\r\n"
                          "mybDAs+dZC8eyjBEL5nSePa+1z4bq5qBUruOd1PPjBe2oyosjEhHiBDHLQcjVB1o\r\n"
                          "XTwZ+ZUOAQEsRU/nri7mZ+hy7irL2eZ4uBwneJkYaK6p2Nu55voZGaYnpoHQD718\r\n"
                          "sxeMUlOsop+tuKNrIyGuhZ3MSBaF505WAlS1ziraFzr+h1XwguDS6+sI1+CKMw5I\r\n"
                          "qqOn+WI+0L3gE5hlUcjsTn8wTDbxY3ie/3ns5aecHN7pdt7R0BX+QWp2gYSYKEKc\r\n"
                          "WJ+ezAsnsDutXwJtjFNIXmhDgX84NtOGxWECaOoA8oLOHUhpJYxX9jbRIHgnoETc\r\n"
                          "rRtSmPtNDTxFtlGCX7F4W1L1z+SaCAomnlYjlsClJ8Gu+CEzITONh7gM4V3zM0r2\r\n"
                          "Yjd/sLwe1yJRMsN/AxTEKPlKVJvIvy/zCrFS5r6ICXPCDuaZlL8X6BmbqPgt8Mbl\r\n"
                          "/X22Mrc84nIImTge7Dybztx06bkqTdxZT/pdE2nPpUPnhpATcKYsehz+tbenSkM8\r\n"
                          "KWbQNaV3PUh7woW/+fj5lGgXGkKVeAiuGT8CAwEAAaNTMFEwHQYDVR0OBBYEFPIP\r\n"
                          "cgM5u7zZ7mlqWnNdzZU6v7PcMB8GA1UdIwQYMBaAFPIPcgM5u7zZ7mlqWnNdzZU6\r\n"
                          "v7PcMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggIBAKT0FxxhWrZr\r\n"
                          "zMHbAx5rA3IITJGI03XUs9kg1Yje6FoEE+rQ7PIq6R81JFy9v7biJssbAcuyUAlS\r\n"
                          "Sd51GHXi3K3f27QqsUnU4edJ/bKEBIyh+ito21fvPwUflhCGoinXX6Gtq3EdMwnB\r\n"
                          "jVaEoY96z+86mO/jDPlRLqsqRVecickucvxo4H5XapFibSqHZIdRMlvrfXls2eb9\r\n"
                          "rDBNYtEX7gP7IVRKyHJV0UksxxK1/jDgzwEVXMI5wvngyULMF94c0u2scwFkyxza\r\n"
                          "vxCeSN8CcQFYhbhtJerxPAR/iWndEfVE3mbUCJ+99MP0ZMIkLp1T1qfyVLw5Jwww\r\n"
                          "bZiWeBYaWE7aqjqWbz8YcuwQGB1V5//fzd2EBJ3/Ny9nHQP8qPodMO+FOifHiQJi\r\n"
                          "9jggx0spOgsLGV9GIyrSNjTL3a9LdeI0Pele3odBVYLMrWubJUNfaVxoiaFefzJE\r\n"
                          "AnqLpsY5ei6X/eKUGs9dubtdn0wMgWr28awRowsbKBNByaplKXjzppLk7b0NP+D0\r\n"
                          "e8LJNZCXMJzXoJq9wzJJsEXT2bjNdqS7zfJQEDDprvb/SrZbLG4tSE47hPxfQLig\r\n"
                          "GMPxKvyjBTKLb7ThdpQfDy60u6HqwPNHoUYkG5/qJFIXS1gr7jijk3pGJl86pS3q\r\n"
                          "ufciMglRankrcBSBvvn38NgzZDtzrMG6\r\n"
                          "-----END CERTIFICATE-----");

    class Model : public QObject
    {
        Q_OBJECT

        void Parser(QDataStream & mess);

    public:

        explicit Model(QObject *parent = 0);
        ~Model();

        void connectToHost(const QString &hostIP);
        QMap<QString, clientInfo>*GetClients();

    signals:

        //signals for view
        void signalOK();
        void signalNewClient(const QPair<QString, clientInfo> & newClient);
        void signalNewMessage(const QString & sender, const QTime &time, const QString & message);
        void signalNewFile(const QString &sender, const QTime &time, const QString & fileName, const QByteArray &file);
        void signalConnectResult(const QString & err);
        void signalWrongLogin();
        void signalLoginExist();
        void signalMessageError(const QString & mess);

        void signalError(const QString & errStr);

    public slots:

        void SendMessage(const int & label, const QString & message, const QByteArray & file = QByteArray());
        void slotRead();
        void slotConnectError(const QList<QSslError> &err);
        void slotConnected();
        void slotDisconnected();

    private:

        std::unique_ptr<QSslSocket> socket;
        uint blockSize = 0;
        QMap<QString, clientInfo> clientsList;
        int port;
    };
}
