#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    if(this->m_server.StartServer(3004, ui->txtBServerInfo))
    {
        ui->txtBServerInfo->append("[+] Server started");
    }
    ui->lblServerIP->setText(this->m_server.GetIP()+" : 3004");
}

Dialog::~Dialog()
{
    delete ui;
}
