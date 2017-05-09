#pragma once

namespace Ui {
    class ServerDialog;
}

namespace YAServer {

    class ServerDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit ServerDialog(QWidget *parent = 0);
        void SetIpInfo(QString strIp);
        void WriteToTextBrowser(QString strMsg);
        ~ServerDialog();

    private:
        Ui::ServerDialog *m_pUi;
};

}


