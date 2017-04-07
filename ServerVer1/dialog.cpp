#include "stdafx.h"
#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    if(this->m_server.StartServer(3004, ui->txtBServerInfo))
//    {
//        ui->txtBServerInfo->append("[+] Server started");
//    }
    //    ui->lblServerIP->setText(m_server.GetIP()+" : 3004");
}

void Dialog::SetIpInfo(QString strIp)
{
    ui->lblServerIP->setText(strIp);
}

void Dialog::WriteToTextBrowser(QString strMsg)
{
    ui->txtBServerInfo->append(strMsg);
}

Dialog::~Dialog()
{
    delete ui;
}
