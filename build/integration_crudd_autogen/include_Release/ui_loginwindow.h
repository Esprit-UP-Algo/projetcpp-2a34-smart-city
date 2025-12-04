/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QTabWidget *tabWidget;
    QWidget *tab_6;
    QLabel *label_21;
    QLabel *label_19;
    QLineEdit *lineEdit_cin_2;
    QLabel *label_20;
    QLineEdit *lineEdit_cin_3;
    QLabel *label_25;
    QPushButton *pushButton;
    QPushButton *btnMotDePasseOublie;
    QPushButton *pushButton_2;
    QPushButton *faceLoginButton;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->resize(1411, 671);
        LoginWindow->setStyleSheet(QString::fromUtf8("QDialog {\n"
"    background-color: #e0f7fa;\n"
"}"));
        tabWidget = new QTabWidget(LoginWindow);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(30, 0, 1411, 671));
        tab_6 = new QWidget();
        tab_6->setObjectName("tab_6");
        label_21 = new QLabel(tab_6);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(150, 160, 521, 331));
        label_21->setPixmap(QPixmap(QString::fromUtf8(":/assets/logo.png")));
        label_21->setScaledContents(true);
        label_19 = new QLabel(tab_6);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(830, 180, 81, 31));
        label_19->setStyleSheet(QString::fromUtf8("font: 11pt \"Segoe UI\";\n"
"color: white;"));
        lineEdit_cin_2 = new QLineEdit(tab_6);
        lineEdit_cin_2->setObjectName("lineEdit_cin_2");
        lineEdit_cin_2->setGeometry(QRect(870, 220, 261, 81));
        lineEdit_cin_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    padding: 10px;\n"
"    border: 1px solid #444;\n"
"    border-radius: 3px;\n"
"    font-size: 11pt;\n"
"    background-color: #3a3a3a;\n"
"    color: white;\n"
"}"));
        label_20 = new QLabel(tab_6);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(820, 310, 121, 20));
        label_20->setStyleSheet(QString::fromUtf8("font: 11pt \"Segoe UI\";\n"
"color: white;"));
        lineEdit_cin_3 = new QLineEdit(tab_6);
        lineEdit_cin_3->setObjectName("lineEdit_cin_3");
        lineEdit_cin_3->setGeometry(QRect(870, 340, 271, 81));
        lineEdit_cin_3->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    padding: 10px;\n"
"    border: 1px solid #444;\n"
"    border-radius: 3px;\n"
"    font-size: 11pt;\n"
"    background-color: #3a3a3a;\n"
"    color: white;\n"
"}"));
        lineEdit_cin_3->setEchoMode(QLineEdit::Password);
        label_25 = new QLabel(tab_6);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(1150, 370, 31, 20));
        pushButton = new QPushButton(tab_6);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(1090, 450, 141, 31));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #5cb85c;\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 12px;\n"
"    border-radius: 3px;\n"
"    font-weight: bold;\n"
"    font-size: 12pt;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #4cae4c;\n"
"}"));
        btnMotDePasseOublie = new QPushButton(tab_6);
        btnMotDePasseOublie->setObjectName("btnMotDePasseOublie");
        btnMotDePasseOublie->setGeometry(QRect(810, 450, 191, 31));
        btnMotDePasseOublie->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent;\n"
"    color: #5cb85c;\n"
"    border: none;\n"
"    padding: 8px;\n"
"    text-decoration: underline;\n"
"    font-size: 10pt;\n"
"}\n"
"QPushButton:hover {\n"
"    color: #4cae4c;\n"
"}"));
        pushButton_2 = new QPushButton(tab_6);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(810, 500, 171, 51));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #337ab7;\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 12px;\n"
"    border-radius: 3px;\n"
"    font-weight: bold;\n"
"    font-size: 11pt;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #286090;\n"
"}"));
        faceLoginButton = new QPushButton(tab_6);
        faceLoginButton->setObjectName("faceLoginButton");
        faceLoginButton->setGeometry(QRect(810, 560, 171, 51));
        faceLoginButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #337ab7;\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 12px;\n"
"    border-radius: 3px;\n"
"    font-weight: bold;\n"
"    font-size: 11pt;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #286090;\n"
"}"));
        tabWidget->addTab(tab_6, QString());

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "Connexion - Syst\303\250me de Gestion", nullptr));
        label_21->setText(QString());
        label_19->setText(QCoreApplication::translate("LoginWindow", "Email\360\237\223\247", nullptr));
        lineEdit_cin_2->setPlaceholderText(QCoreApplication::translate("LoginWindow", "Entrez votre email", nullptr));
        label_20->setText(QCoreApplication::translate("LoginWindow", "mot de passe\360\237\224\220", nullptr));
        lineEdit_cin_3->setPlaceholderText(QCoreApplication::translate("LoginWindow", "Entrez votre mot de passe", nullptr));
        label_25->setText(QCoreApplication::translate("LoginWindow", "\360\237\221\201\357\270\217", nullptr));
        pushButton->setText(QCoreApplication::translate("LoginWindow", "se connecter\342\234\205", nullptr));
        btnMotDePasseOublie->setText(QCoreApplication::translate("LoginWindow", "mot de passe oubli\303\251?\360\237\224\221\342\235\223", nullptr));
        pushButton_2->setText(QCoreApplication::translate("LoginWindow", "FaceRecognition\360\237\221\201\357\270\217\360\237\223\261", nullptr));
        faceLoginButton->setText(QCoreApplication::translate("LoginWindow", "Connexion Faciale\360\237\221\244", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QCoreApplication::translate("LoginWindow", "connexion\360\237\224\220", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
