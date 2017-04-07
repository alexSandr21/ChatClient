#pragma once



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void SetIpInfo(QString strIp);
    void WriteToTextBrowser(QString strMsg);
    ~Dialog();

private:
    Ui::Dialog *ui;
};

