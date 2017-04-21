#pragma once

#include <QFile>
#include <QtCore>
#include <QDataStream>
#include <QThread>
#include <QSharedPointer>
#include "src/ModelLib/model.h"

#include <QObject>


namespace YAClient
{

const int BUF_SIZE = 4096;

class FileSendThread: public QThread
{
    Q_OBJECT
public:
    FileSendThread(){}
    FileSendThread(QString filePath, QString fileName, QString myLogin, QString myReceiver, std::shared_ptr<QMutex>mut);

protected:
    void run() override;

signals:
    void signalSendFile(int  label,  QString  message,  QByteArray file);
    void signalSendResult( int label, QString myReceiver, QString fileName);

private:

    QString m_filePath;
    QString m_fileName;
    QString m_myLogin;
    QString m_myReceiver;
    std::shared_ptr<QMutex> m_mut;
};
}

