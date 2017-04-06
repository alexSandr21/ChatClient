#include "presenter.h"

Presenter::Presenter(QObject *parent) : QObject(parent)
{
    connect(&model, SIGNAL(signalConnect(QString)), this, SLOT(slotConnectResult(QString)));
    connect(&model, SIGNAL(signalWrongLogin()), this, SLOT(slotWrongLogin()));
    connect(&model, SIGNAL(signalOK()), this, SLOT(slotOK()));
    connect(&model, SIGNAL(signalLoginExist()), this, SLOT(slotLoginExist()));
    connect(&model, SIGNAL(signalNewMessage(QString, QTime, QString)), this, SLOT(slotNewMessage(QString, QTime, QString)));
    connect(&model, SIGNAL(signalNewFile(QString,QTime,QString,QByteArray)), this, SLOT(slotNewFile(QString,QTime,QString,QByteArray)));
    connect(&model, SIGNAL(signalNewClient(QPair<QString,clientInfo>)), this, SLOT(slotNewClient(QPair<QString,clientInfo>)));
    connect(&model, SIGNAL(signalMessageError()), this, SIGNAL(signalMessageError()));
}

bool Presenter::methCheckString(QString str)
{
    if(str.indexOf(DELIM)==-1)
       return true;

        return false;
}

bool Presenter::methCheckStringAll(QString str)
{
    bool res = true;
    std::for_each(WRONGCH.cbegin(), WRONGCH.cend(), [str, &res](const char ch)
    {
        if(str.indexOf(ch)!=-1)
            res = false;
    });


    return res;

}

QString Presenter::methGetReceiver()
{
    return myReceiver;
}

QString Presenter::methGetLogin()
{
    return myLogin;
}

bool Presenter::methIsOnline(QString receiver)
{
       auto find = clientsMap->find(receiver);

       if(find!=clientsMap->cend())
           if(find->Online)
               return true;

       return false;
}

void Presenter::slotGetUsers()
{

    for(auto iter = clientsMap->cbegin(); iter!=clientsMap->cend(); iter++)
        emit signalSetUsers((iter.value().name+" "+iter.value().secName),(iter.value().Online?"online":""), iter.key());

}


void Presenter::slotSendMessage(QString mess)
{
    MessageStruct message{myReceiver, 1, mess, QTime::currentTime().toString()};
    dBase.Insert(message);

    emit signalWriteMessage(message.interlocutor,
                            message.myAnswer?true:false,
                            message.message,
                            message.time);

    QString messForModel(myLogin+DELIM+myReceiver+DELIM+mess);

    model.SendMessage(L_MESS, messForModel);
}


void Presenter::slotConnect(QString serverIP)
{
    model.connectToHost(serverIP);
}

void Presenter::slotLogin(QString login, QString pass)
{
    myLogin = login;
    login += (DELIM+pass);
    model.SendMessage(L_LOG, login);
}

void Presenter::slotRegistration(QString name, QString surname, QString login, QString pass)
{
    myLogin = login;
    login+=(DELIM+pass+DELIM+name+DELIM+surname);
    model.SendMessage(L_REG, login);
}

void Presenter::slotSetReceiver(QString receiver)
{
        myReceiver = receiver;
        auto find = clientsMap->find(receiver);

        if(find!=clientsMap->cend())
            receiverName = find->name;
        else
            return;

        QVector<MessageStruct> messages = dBase.GetMessages(receiver);

        for(int i=0; i<messages.size(); i++)
            emit signalWriteMessage(receiverName,
                                    messages[i].myAnswer?true:false,
                                    messages[i].message,
                                    messages[i].time);


}

void Presenter::slotSendFile(QString path)
{
    try
    {
        path.remove(0, 8);
        QScopedPointer<QFile, CustomDeleter> pF(new QFile(path));

        if(!pF->open(QIODevice::ReadOnly))
        {
            emit signalMessageError();
            return;
        }

        if(pF->size()>MAX_FILE_SIZE)
        {
            emit signalTooBigFile();
            return;
        }

        QString mess;
        auto pos = path.lastIndexOf('/');
        if(pos==-1)
            mess = path;
        else
            mess = path.remove(0, pos+1);

        dBase.Insert(MessageStruct{myReceiver, 1, "file: "+mess, QTime::currentTime().toString()});
        emit signalWriteMessage(myReceiver, 1, "file: "+mess, QTime::currentTime().toString());

        QByteArray fileContent;
        fileContent = pF->readAll();

        QString messForModel(myLogin+DELIM+myReceiver+DELIM+mess);

        model.SendMessage(L_FILE, messForModel, fileContent);
    }
    catch(QException&ex)
    {
        //write error in log file
        emit signalMessageError();
    }

}

void Presenter::slotConnectResult(QString result)
{
    emit signalConnectResult(result);
}

void Presenter::slotNewClient(QPair<QString, clientInfo> newClient)
{
    QString name = newClient.second.name+" "+newClient.second.secName;
    QString status = newClient.second.Online?"online":"";
    QString login = newClient.first;

    if(login!=myLogin)
        emit signalNewUser(name,status, login);

    if(login == myReceiver)
        emit signalSetFriendStatus(login);
}

void Presenter::slotNewMessage(QString sender, QTime time, QString message)
{
    dBase.Insert(MessageStruct{sender, 0, message, time.toString()});

    if(sender == myReceiver) 
        emit signalWriteMessage(sender, 0, message, time.toString());

    else
        emit signalNewMessage(sender);

}

void Presenter::slotNewFile(QString sender, QTime time, QString fileName, QByteArray file)
{
    QDir dir;
    QString message = "file: "+fileName;

    dir.mkdir("YAMessenger");

    dir.mkdir("YAMessenger//"+myLogin);
    dir.mkdir("YAMessenger//"+myLogin+"//from "+sender);

    QString filePath("YAMessenger//"+myLogin+"//from "+sender+"//"+fileName);



    while(QFile::exists(filePath))
    {
        int i=1;
        QString s;
        QTextStream in(&s);

        in<<i;

        auto pos = filePath.lastIndexOf('.');

        if(pos!=-1)
            filePath.insert(pos, s);
        else
            return;
        ++i;

    }

    QScopedPointer<QFile, CustomDeleter> pF(new QFile(filePath));

     //QFile f(filePath);

    if(!pF->open(QIODevice::WriteOnly))
        message = "Error open file";
    else
        pF->write(file);

    dBase.Insert(MessageStruct{sender, 0, message , time.toString()});

    if(sender == myReceiver)
        emit signalWriteMessage(sender, 0, message, time.toString());
    else
        emit signalNewMessage(sender);
}

void Presenter::slotWrongLogin()
{
    emit signalLoginResult(false);
}

void Presenter::slotLoginExist()
{
    emit signalRegistrationResult(false);
}

void Presenter::slotOK()
{
    clientsMap = model.GetClients();
    clientsMap->remove(myLogin);


    dBase.OpenDataBase();
    dBase.CreateTabel(myLogin);

    emit signalLoginResult(true);
    emit signalRegistrationResult(true);
    emit signalSetTitle(myLogin);
}
