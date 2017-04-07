#include "stdafx.h"
#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
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
