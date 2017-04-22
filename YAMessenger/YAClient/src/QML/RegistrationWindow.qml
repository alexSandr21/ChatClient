import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Rectangle
{

    property alias nameFocus:name.focus
    id:registrationWindow
    width: 300
    height: 400



    anchors.centerIn: parent
    radius: 10

    Keys.onEnterPressed:
    {
        registrationButton.funcClicked()
    }

    function funcRegistrationResult(result)
    {
        if(result)
        {
            registrationWindow.visible = false
            fade.visible = false
        }
        else
        {
            loginErrors.text = "Login already exist"
            userLogin.border.color = "red"
        }

    }

    Rectangle
    {
        id:header
        color:"#512A41"

        anchors.fill: parent
        anchors.bottomMargin: 372
        radius: 5

        Text
        {
            anchors.centerIn: parent
            text:"Registration"
            color: "lightgray"
            font.pixelSize: 16
        }
    }

    Text
    {
        id: textName
        x: 126
        y: 41

        text:" Name"
        font.family: "Tahoma"
        color: "black"
        font.pixelSize: 14


    }

    Rectangle
    {
        id: userName
        y: 64
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#F6E4EF"
        anchors.horizontalCenterOffset: 0
        height: 24
        width: 150

        border.color: "black"
        border.width: 0.5
        radius: 5

        TextInput
        {
            id:name
            font.pixelSize: 14
            anchors.fill: parent
            clip: true

            anchors.topMargin: 3
            anchors.leftMargin: 5
            onTextChanged:
            {
                userName.border.color ="black"
                nameErrors.text =""
            }
            Keys.onTabPressed: surname.focus = true
            maximumLength: 17
        }
    }



    Rectangle
    {
        id:registrationButton
        y: 317
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#512A41"
        anchors.horizontalCenterOffset: 0


        height: 27
        width: 150

        radius: 5

        Text
        {
            anchors.centerIn: parent
            color: "lightgray"
            font.pixelSize: 12
            text:"Registration"
        }

        MouseArea
        {
            anchors.bottomMargin: 0
            anchors.fill:parent
            onPressed:
            {
               registrationButton.color =  "#6E3A59"
            }
            onReleased:
            {
                registrationButton.color = "#512A41"
                registrationButton.funcClicked()
            }
        }
        function funcClicked()
        {
            userName.border.color = "black"
            userSurname.border.color = "black"
            userLogin.border.color = "black"
            userPassword.border.color = "black"

            nameErrors.text = ""
            surnameErrors.text =""
            loginErrors.text =""
            passwordErrors.text =""


            var res = true;
            var err = "Not use .,*;:'\"?"

            if(name.text =="")
            {
                userName.border.color = "red"
                res = false
            }
            if(surname.text == "")
            {
                userSurname.border.color = "red"
                res = false
            }
            if(login.text =="")
            {
                userLogin.border.color = "red"
                res = false
            }
            if(password.text =="")
            {
                userPassword.border.color = "red"
                res = false
            }


            if(!presenter.methCheckStringAll(name.getText(0, name.text.length)))
            {
                res = false
                userName.border.color = "red"
                nameErrors.text = err
            }
            if(!presenter.methCheckStringAll(surname.getText(0, surname.text.length)))
            {
                res = false
                userSurname.border.color = "red"
                surnameErrors.text = err
            }
            if(!presenter.methCheckStringAll(login.getText(0, login.text.length)))
            {
                res = false
                userLogin.border.color= "red"
                loginErrors.text = err
            }
            if(!presenter.methCheckStringAll(password.getText(0,password.text.length)))
            {
                res = false
                userPassword.border.color = "red"
                passwordErrors.text = err
            }

            if(res)
                presenter.slotRegistration(name.getText(0, name.text.length),
                                          surname.getText(0, surname.text.length),
                                           login.getText(0, login.text.length),
                                          password.getText(0,password.text.length))
        }


    }

    Rectangle {
        id: backButton
        x: -1
        y: 356
        width: 150
        height: 27
        color: "#512a41"
        radius: 5
        Text {
            color: "#d3d3d3"
            text: "Back"
            anchors.centerIn: parent
            font.pixelSize: 12
        }

        MouseArea {
            anchors.fill: parent

            onPressed:
            {
               backButton.color =  "#6E3A59"
            }
            onReleased:
            {
                backButton.color = "#512A41"

                registrationWindow.visible = false
                startWindow.visible = true

                name.text = ""
                surname.text = ""
                password.text = ""
                login.text = ""

                nameErrors.text = ""
                surnameErrors.text = ""
                loginErrors.text =""
                passwordErrors.text = ""

                userName.border.color = "black"
                userSurname.border.color = "black"
                userLogin.border.color = "black"
                userPassword.border.color = "black"
            }
        }
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0
    }

    Text {
        id: textSurame
        x: 117
        y: 106
        color: "#000000"
        text: " Surname"
        font.pixelSize: 14
        font.family: "Tahoma"
    }

    Rectangle {
        id: userSurname
        x: 7
        y: 129
        width: 150
        height: 24
        color: "#f6e4ef"
        radius: 5
        TextInput
        {
            id: surname
            font.pixelSize: 14
            anchors.fill: parent
            clip: true

            anchors.topMargin: 3
            anchors.leftMargin: 5

            maximumLength: 17
            onTextChanged:
            {
                userSurname.border.color = "black"
                surnameErrors.text = ""
            }
            Keys.onTabPressed: login.focus = true
        }
        border.width: 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#000000"
    }

    Text {
        id: textLogin
        x: 128
        y: 173
        color: "#000000"
        text: " Login"
        font.pixelSize: 14
        font.family: "Tahoma"
    }

    Rectangle {
        id: userLogin
        x: -8
        y: 196
        width: 150
        height: 24
        color: "#f6e4ef"
        radius: 5
        TextInput
        {
            id: login
            font.pixelSize: 14
            anchors.fill: parent
            clip: true

            anchors.topMargin: 3
            anchors.leftMargin: 5

            maximumLength: 17
            onTextChanged:
            {
                userLogin.border.color = "black"
                loginErrors.text =""
            }
            Keys.onTabPressed: password.focus = true
        }
        border.width: 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#000000"
    }

    Text {
        id: textPassword
        x: 115
        y: 238
        color: "#000000"
        text: " Password"
        fontSizeMode: Text.VerticalFit
        font.pixelSize: 14
        font.family: "Tahoma"

    }

    Rectangle {
        id: userPassword
        x: -1
        y: 261
        width: 150
        height: 24
        color: "#f6e4ef"
        radius: 5
        TextInput
        {
            id: password
            font.pixelSize: 14
            anchors.fill: parent
            clip: true

            anchors.topMargin: 3
            anchors.leftMargin: 5

            maximumLength: 17
            onTextChanged:
            {
                userPassword.border.color = "black"
                passwordErrors.text = ""
            }
            Keys.onTabPressed: name.focus = true
            echoMode: TextInput.Password
        }
        border.width: 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        border.color: "#000000"
    }

    Text {
        id: nameErrors
        x: 76
        y: 87
        color: "red"
        text: ""
        font.pixelSize: 13
        font.family: "Times New Roman"
    }

    Text {
        id: surnameErrors
        x: 76
        y: 154
        color: "red"
        text: ""
        font.pixelSize: 13
        font.family: "Times New Roman"
    }

    Text {
        id: loginErrors
        x: 76
        y: 220
        color: "red"
        text: ""
        font.pixelSize: 13
        font.family: "Times New Roman"
    }

    Text {
        id: passwordErrors
        x: 76
        y: 285
        color: "red"
        text: ""
        font.pixelSize: 13
        font.family: "Times New Roman"
    }

}
