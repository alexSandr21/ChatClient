import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.1


ListModel
{
    property string name
    property string status
    property string login
    property bool newMessage: false

    function myInsert(name,status,login)
    {
        var mes = false;
        for(var i =0; i<mod.count; i++)
            if(mod.get(i).login === login)
            {
                mes = mod.get(i).newMessage
                mod.remove(i)
                break
            }

        mod.myAppend(name, status,login, mes)

    }

    function myAppend(name, status, login, mes)
    {
        if(status === "online")
            mod.insert(0, {name:name, status:status, login:login, newMessage:mes})
        else
            mod.append({name:name, status:status, login:login, newMessage:mes})

    }
    function funcReadedMessage(readed, login)
    {
        for(var i=0; i<mod.count; i++)
            if(mod.get(i).login === login)
            {
                if(readed)
                    mod.get(i).newMessage = false
                else
                    mod.get(i).newMessage = true
            }
    }

    function funcNewMessage(sender)
    {
        for(var i=0; i<mod.count; i++)
            if(mod.get(i).login === sender)
            {
                var name = mod.get(i).name
                var status = mod.get(i).status
                var login = mod.get(i).login

                mod.remove(i);

                mod.myAppend(name, status, login, true)

                break;
            }

    }
}
