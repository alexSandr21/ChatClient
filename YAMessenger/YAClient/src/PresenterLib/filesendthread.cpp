#include "filesendthread.h"


namespace YAClient
{
FileSendThread::FileSendThread(QString filePath, QString fileName, QString myLogin, QString myReceiver, std::shared_ptr<QMutex>mut)
    :m_filePath(filePath),m_fileName(fileName), m_myLogin(myLogin), m_myReceiver(myReceiver), m_mut(mut)
{

}

void FileSendThread::run()
{
    try
    {
        std::unique_ptr<QFile, void(*)(QFile*)> pF(new QFile(m_filePath),[](QFile *f){f->close();delete f;});

        if(!pF->open(QIODevice::ReadOnly))
        {
            emit signalSendResult(L_ERROR, m_myReceiver, m_fileName);
            return;
        }

        QByteArray fileContent;
        QString messForModel;

        do
        {
            fileContent.clear();

            fileContent = pF->read(BUF_SIZE);

            if(!fileContent.isEmpty())
                messForModel = (m_myLogin+DELIM+m_myReceiver+DELIM+"1"+m_fileName);

            else
                messForModel = (m_myLogin+DELIM+m_myReceiver+DELIM+"2"+m_fileName);

            //m_mut->lock();
            emit signalSendFile(L_FILE, messForModel, fileContent);

        }while(!fileContent.isEmpty());

        emit signalSendResult(L_OK, m_myReceiver, m_fileName);

    }
    catch(const std::exception&ex)
    {
        emit signalSendResult(L_ERROR, m_myReceiver, m_fileName);
        //write error in log file
    }
}

}

