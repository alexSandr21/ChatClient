#include "presenter.h"

Presenter::Presenter(QObject *parent) : QObject(parent)
{
    connect(&model, SIGNAL(signalConnect(QString)), this, SLOT(slotConnectResult(QString)));
    connect(&model, SIGNAL(signalWrongLogin()), this, SLOT(slotWrongLogin()));
    connect(&model, SIGNAL(signalOK()), this, SLOT(slotOK()));
    connect(&model, SIGNAL(signalLoginExist()), this, SLOT(slotLoginExist()));
    connect(&model, SIGNAL(signalNewMessage(QString, QTime, QString)), this, SLOT(slotNewMessage(QString, QTime, QString)));
    connect(&model, SIGNAL(signalNewClient(QPair<QString,clientInfo>)), this, SLOT(slotNewClient(QPair<QString,clientInfo>)));
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
            receiverName = "Unknown";



        QVector<MessageStruct> messages = dBase.GetMessages(receiver);

        for(int i=0; i<messages.size(); i++)
            emit signalWriteMessage(receiverName,
                                    messages[i].myAnswer?true:false,
                                    messages[i].message,
                                    messages[i].time);


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
}

void Presenter::slotNewMessage(QString sender, QTime time, QString message)
{
    dBase.Insert(MessageStruct{sender, 0, message, time.toString()});

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
}
