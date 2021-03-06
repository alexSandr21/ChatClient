#include "stdafx.h"
#include "presenter.h"



YAClient::Presenter::Presenter(std::shared_ptr<QFile> log, QObject *parent) : QObject(parent),m_contact(false), m_logFile(log)
{
    connect(&m_model, SIGNAL(signalConnectResult(QString)), this, SIGNAL(signalConnectResult(QString)));
    connect(&m_model, SIGNAL(signalWrongLogin()), this, SLOT(slotWrongLogin()));
    connect(&m_model, SIGNAL(signalOK()), this, SLOT(slotOK()));
    connect(&m_model, SIGNAL(signalLoginExist()), this, SLOT(slotLoginExist()));
    connect(&m_model, SIGNAL(signalNewMessage(QString, QTime, QString)), this, SLOT(slotNewMessage(QString, QTime, QString)));
    connect(&m_model, SIGNAL(signalNewFile(QString,QTime,QString,QByteArray)), this, SLOT(slotNewFile(QString,QTime,QString,QByteArray)));
    connect(&m_model, SIGNAL(signalNewClient(QPair<QString,clientInfo>)), this, SLOT(slotNewClient(QPair<QString,clientInfo>)));
    connect(&m_model, SIGNAL(signalMessageError(QString)), this, SIGNAL(signalMessageError(QString)));
    connect(&m_model, SIGNAL(signalError(QString)), this, SLOT(slotWriteLog(QString)));
    connect(&m_dBase, SIGNAL(signalError(QString)), this, SLOT(slotWriteLog(QString)));
}

bool YAClient::Presenter::methCheckString(QString str)
{
    if(str.indexOf(DELIM)==-1)
       return true;

    return false;
}

bool YAClient::Presenter::methCheckStringAll(QString str)
{
    bool res = true;
    std::for_each(WRONGCH.cbegin(), WRONGCH.cend(), [str, &res](const char ch)
    {
        if(str.indexOf(ch)!=-1)
            res = false;
    });


    return res;

}

QString YAClient::Presenter::methGetReceiver()
{
    return m_myReceiver;
}

QString YAClient::Presenter::methGetLogin()
{
    return m_myLogin;
}

bool YAClient::Presenter::methIsOnline(QString receiver)
{
    auto find = m_clientsMap->find(receiver);

    if(find!=m_clientsMap->cend())
        if(find->Online)
            return true;

    return false;
}

bool YAClient::Presenter::methIsContact()
{
    return m_contact;
}

void YAClient::Presenter::slotGetUsers()
{

    for(auto iter = m_clientsMap->cbegin(); iter!=m_clientsMap->cend(); iter++)
        emit signalSetUsers((iter.value().name+" "+iter.value().secName),(iter.value().Online?"online":""), iter.key());

}


void YAClient::Presenter::slotSendMessage(QString mess)
{
    MessageStruct message{m_myReceiver, 1, mess, QTime::currentTime().toString()};
    m_dBase.Insert(message);

    emit signalWriteMessage(message.interlocutor,
                            message.myAnswer?true:false,
                            message.message,
                            message.time);

    QString messForModel(m_myLogin+DELIM+m_myReceiver+DELIM+mess);

    m_model.SendMessage(L_MESS, messForModel);
}


void YAClient::Presenter::slotConnect(QString serverIP)
{
    m_model.connectToHost(serverIP);
}

void YAClient::Presenter::slotLogin(QString login, QString pass)
{
    m_myLogin = login;
    login += (DELIM+pass);

    m_model.SendMessage(L_LOG, login);
}

void YAClient::Presenter::slotRegistration(QString name, QString surname, QString login, QString pass)
{
    m_myLogin = login;
    login+=(DELIM+pass+DELIM+name+DELIM+surname);

    m_model.SendMessage(L_REG, login);
}

void YAClient::Presenter::slotSetReceiver(QString receiver)
{
        m_myReceiver = receiver;
        auto find = m_clientsMap->find(receiver);

        if(find!=m_clientsMap->cend())
            m_receiverName = find->name;
        else
            return;

        QVector<MessageStruct> messages = m_dBase.GetMessages(receiver);

        for(int i=0; i<messages.size(); i++)
            emit signalWriteMessage(m_receiverName,
                                    messages[i].myAnswer?true:false,
                                    messages[i].message,
                                    messages[i].time);


}

void YAClient::Presenter::slotSendFile(QString path)
{
    try
    {

       path.remove(0, CORRECT_FILE_PATH);

       QFile file(path);

        if(!file.open(QIODevice::ReadOnly))
        {
            emit signalMessageError("Error open file");
            return;
        }

        if(file.size()>MAX_FILE_SIZE)
        {
            emit signalMessageError("File must be not more 30MB");
            return;
        }

        QString fileName;
        auto pos = path.lastIndexOf('/');

        if(pos==-1)
            fileName = path;
        else
            fileName = path.remove(0, pos+1);

        m_dBase.Insert(MessageStruct{m_myReceiver, 1, "file: "+fileName, QTime::currentTime().toString()});
        emit signalWriteMessage(m_myReceiver, 1, "file: "+fileName, QTime::currentTime().toString());

        QByteArray fileContent;
        fileContent = file.readAll();

        QString messForModel(m_myLogin+DELIM+m_myReceiver+DELIM+fileName);

        m_model.SendMessage(L_FILE, messForModel, fileContent);

        file.close();
    }
    catch(const std::exception&ex)
    {
        slotWriteLog(ex.what());
        emit signalMessageError("Error send file");
    }

}

void YAClient::Presenter::slotNewClient(const QPair<QString, clientInfo> & newClient)
{
    QString name = newClient.second.name+" "+newClient.second.secName;
    QString status = newClient.second.Online?"online":"";
    QString login = newClient.first;

    if(login!=m_myLogin)
        emit signalNewUser(name,status, login);

    if(login == m_myReceiver)
        emit signalSetFriendStatus(login);
}

void YAClient::Presenter::slotNewMessage(const QString & sender, const QTime & time, const QString & message)
{
    m_dBase.Insert(MessageStruct{sender, 0, message, time.toString()});

    if(sender == m_myReceiver)
        emit signalWriteMessage(sender, 0, message, time.toString());

    else
        emit signalNewMessage(sender);

}

void YAClient::Presenter::slotNewFile(const QString & sender, const QTime & time, const QString & fileName, const QByteArray & file)
{
    QDir dir;
    QString message = "file: "+fileName;

    dir.mkdir("Received files");
    dir.mkdir("Received files//"+m_myLogin);
    dir.mkdir("Received files//"+m_myLogin+"//from "+sender);

    QString filePath("Received files//"+m_myLogin+"//from "+sender+"//"+fileName);

    QFile rFile(filePath);

    if(!rFile.open(QIODevice::WriteOnly))
        message = "Error open file";
    else
        rFile.write(file);

    m_dBase.Insert(MessageStruct{sender, 0, message , time.toString()});

    if(sender == m_myReceiver)
        emit signalWriteMessage(sender, 0, message, time.toString());
    else
        emit signalNewMessage(sender);

    rFile.close();

}

void YAClient::Presenter::slotWrongLogin()
{
    emit signalLoginResult(false);
}

void YAClient::Presenter::slotLoginExist()
{
    emit signalRegistrationResult(false);
}

void YAClient::Presenter::slotOK()
{
    m_clientsMap = m_model.GetClients();
    m_clientsMap->remove(m_myLogin);


    if(m_dBase.OpenDataBase())
        m_dBase.CreateTabel(m_myLogin);

    else
        slotWriteLog("Error data base open");

    emit signalLoginResult(true);
    emit signalRegistrationResult(true);
    emit signalSetTitle(m_myLogin);

    m_contact = true;
}

void YAClient::Presenter::slotWriteLog(const QString &errStr)
{
    if(m_logFile->isOpen())
    {
        QByteArray str;
        str.append(QDateTime::currentDateTime().toString());
        str.append(" ");
        str.append(errStr);
        str.append("\r\n");

        m_logFile->write(str);
    }
}



