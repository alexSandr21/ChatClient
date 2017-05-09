#include "stdafx.h"
#include "serverdialog.h"
#include "ui_serverdialog.h"

YAServer::ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    m_pUi(new Ui::ServerDialog)
{
    m_pUi->setupUi(this);
}

void YAServer::ServerDialog::SetIpInfo(QString strIp)
{
    m_pUi->lblServerIP->setText(strIp);
}

void YAServer::ServerDialog::WriteToTextBrowser(QString strMsg)
{
    m_pUi->txtBServerInfo->append(strMsg);
}
YAServer::ServerDialog::~ServerDialog()
{
    delete m_pUi;
}
