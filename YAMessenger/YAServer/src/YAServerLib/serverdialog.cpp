#include "stdafx.h"
#include "serverdialog.h"
#include "ui_serverdialog.h"

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog)
{
    ui->setupUi(this);
}

void ServerDialog::SetIpInfo(QString strIp)
{
    ui->lblServerIP->setText(strIp);
}

void ServerDialog::WriteToTextBrowser(QString strMsg)
{
    ui->txtBServerInfo->append(strMsg);
}
ServerDialog::~ServerDialog()
{
    delete ui;
}
