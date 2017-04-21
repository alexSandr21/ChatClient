/********************************************************************************
** Form generated from reading UI file 'serverdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERDIALOG_H
#define UI_SERVERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *lblServerIP;
    QTextBrowser *txtBServerInfo;

    void setupUi(QDialog *ServerDialog)
    {
        if (ServerDialog->objectName().isEmpty())
            ServerDialog->setObjectName(QStringLiteral("ServerDialog"));
        ServerDialog->resize(420, 330);
        layoutWidget = new QWidget(ServerDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 401, 311));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lblServerIP = new QLabel(layoutWidget);
        lblServerIP->setObjectName(QStringLiteral("lblServerIP"));
        QFont font;
        font.setPointSize(12);
        lblServerIP->setFont(font);

        verticalLayout->addWidget(lblServerIP);

        txtBServerInfo = new QTextBrowser(layoutWidget);
        txtBServerInfo->setObjectName(QStringLiteral("txtBServerInfo"));
        QFont font1;
        font1.setPointSize(10);
        txtBServerInfo->setFont(font1);

        verticalLayout->addWidget(txtBServerInfo);


        retranslateUi(ServerDialog);

        QMetaObject::connectSlotsByName(ServerDialog);
    } // setupUi

    void retranslateUi(QDialog *ServerDialog)
    {
        ServerDialog->setWindowTitle(QApplication::translate("ServerDialog", "Dialog", Q_NULLPTR));
        lblServerIP->setText(QApplication::translate("ServerDialog", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ServerDialog: public Ui_ServerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERDIALOG_H
