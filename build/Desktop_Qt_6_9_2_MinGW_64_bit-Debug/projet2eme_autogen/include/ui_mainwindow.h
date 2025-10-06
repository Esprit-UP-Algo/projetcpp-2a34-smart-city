/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QWidget *contentWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *leftLayout;
    QPushButton *btnAjouterTop;
    QPushButton *btnModifierTop;
    QGroupBox *formGroup;
    QFormLayout *formLayout;
    QLabel *label_id;
    QLineEdit *lineEdit_id;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;
    QLabel *label_prenom;
    QLineEdit *lineEdit_prenom;
    QLabel *label_email;
    QLineEdit *lineEdit_email;
    QLabel *label_mdp;
    QLineEdit *lineEdit_mdp;
    QLabel *label_tel;
    QLineEdit *lineEdit_tel;
    QPushButton *btnAjouterForm;
    QVBoxLayout *centerLayout;
    QLineEdit *lineEdit_recherche;
    QTableView *tableView;
    QVBoxLayout *rightLayout;
    QPushButton *btnTrier;
    QPushButton *btnSupprimer;
    QPushButton *btnPDF;
    QGroupBox *grpStat;
    QVBoxLayout *statLayout;
    QLabel *label_stat;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("\n"
"      QWidget {\n"
"        background-color: #d6e5c8;\n"
"        font-family: \"Segoe UI\";\n"
"        font-size: 10pt;\n"
"      }\n"
"      QLineEdit {\n"
"        background: white;\n"
"        border: 1px solid #a0c49d;\n"
"        border-radius: 4px;\n"
"        padding: 4px;\n"
"      }\n"
"      QPushButton {\n"
"        background-color: #4caf50;\n"
"        color: white;\n"
"        border-radius: 4px;\n"
"        padding: 6px 12px;\n"
"      }\n"
"      QPushButton:hover {\n"
"        background-color: #45a049;\n"
"      }\n"
"      QPushButton:pressed {\n"
"        background-color: #388e3c;\n"
"      }\n"
"      QTableView {\n"
"        background-color: white;\n"
"        gridline-color: #a0c49d;\n"
"        selection-background-color: #c8e6c9;\n"
"      }\n"
"      QHeaderView::section {\n"
"        background-color: #a5d6a7;\n"
"        padding: 4px;\n"
"        border: 1px solid #81c784;\n"
"      }\n"
"   "));
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        contentWidget = new QWidget(centralwidget);
        contentWidget->setObjectName("contentWidget");
        horizontalLayout = new QHBoxLayout(contentWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        leftLayout = new QVBoxLayout();
        leftLayout->setObjectName("leftLayout");
        btnAjouterTop = new QPushButton(contentWidget);
        btnAjouterTop->setObjectName("btnAjouterTop");

        leftLayout->addWidget(btnAjouterTop);

        btnModifierTop = new QPushButton(contentWidget);
        btnModifierTop->setObjectName("btnModifierTop");

        leftLayout->addWidget(btnModifierTop);

        formGroup = new QGroupBox(contentWidget);
        formGroup->setObjectName("formGroup");
        formLayout = new QFormLayout(formGroup);
        formLayout->setObjectName("formLayout");
        label_id = new QLabel(formGroup);
        label_id->setObjectName("label_id");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_id);

        lineEdit_id = new QLineEdit(formGroup);
        lineEdit_id->setObjectName("lineEdit_id");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit_id);

        label_nom = new QLabel(formGroup);
        label_nom->setObjectName("label_nom");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_nom);

        lineEdit_nom = new QLineEdit(formGroup);
        lineEdit_nom->setObjectName("lineEdit_nom");

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_nom);

        label_prenom = new QLabel(formGroup);
        label_prenom->setObjectName("label_prenom");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_prenom);

        lineEdit_prenom = new QLineEdit(formGroup);
        lineEdit_prenom->setObjectName("lineEdit_prenom");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, lineEdit_prenom);

        label_email = new QLabel(formGroup);
        label_email->setObjectName("label_email");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_email);

        lineEdit_email = new QLineEdit(formGroup);
        lineEdit_email->setObjectName("lineEdit_email");

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, lineEdit_email);

        label_mdp = new QLabel(formGroup);
        label_mdp->setObjectName("label_mdp");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_mdp);

        lineEdit_mdp = new QLineEdit(formGroup);
        lineEdit_mdp->setObjectName("lineEdit_mdp");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, lineEdit_mdp);

        label_tel = new QLabel(formGroup);
        label_tel->setObjectName("label_tel");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, label_tel);

        lineEdit_tel = new QLineEdit(formGroup);
        lineEdit_tel->setObjectName("lineEdit_tel");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, lineEdit_tel);

        btnAjouterForm = new QPushButton(formGroup);
        btnAjouterForm->setObjectName("btnAjouterForm");

        formLayout->setWidget(6, QFormLayout::ItemRole::SpanningRole, btnAjouterForm);


        leftLayout->addWidget(formGroup);


        horizontalLayout->addLayout(leftLayout);

        centerLayout = new QVBoxLayout();
        centerLayout->setObjectName("centerLayout");
        lineEdit_recherche = new QLineEdit(contentWidget);
        lineEdit_recherche->setObjectName("lineEdit_recherche");

        centerLayout->addWidget(lineEdit_recherche);

        tableView = new QTableView(contentWidget);
        tableView->setObjectName("tableView");

        centerLayout->addWidget(tableView);


        horizontalLayout->addLayout(centerLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName("rightLayout");
        btnTrier = new QPushButton(contentWidget);
        btnTrier->setObjectName("btnTrier");

        rightLayout->addWidget(btnTrier);

        btnSupprimer = new QPushButton(contentWidget);
        btnSupprimer->setObjectName("btnSupprimer");

        rightLayout->addWidget(btnSupprimer);

        btnPDF = new QPushButton(contentWidget);
        btnPDF->setObjectName("btnPDF");

        rightLayout->addWidget(btnPDF);

        grpStat = new QGroupBox(contentWidget);
        grpStat->setObjectName("grpStat");
        statLayout = new QVBoxLayout(grpStat);
        statLayout->setObjectName("statLayout");
        label_stat = new QLabel(grpStat);
        label_stat->setObjectName("label_stat");
        label_stat->setAlignment(Qt::AlignTop);

        statLayout->addWidget(label_stat);


        rightLayout->addWidget(grpStat);


        horizontalLayout->addLayout(rightLayout);


        mainLayout->addWidget(contentWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion Utilisateurs", nullptr));
        btnAjouterTop->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btnModifierTop->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        formGroup->setTitle(QCoreApplication::translate("MainWindow", "Formulaire", nullptr));
        label_id->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_nom->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_prenom->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        label_email->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_mdp->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        label_tel->setText(QCoreApplication::translate("MainWindow", "N Tel", nullptr));
        btnAjouterForm->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        lineEdit_recherche->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche...", nullptr));
        btnTrier->setText(QCoreApplication::translate("MainWindow", "Trier Par Nom", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        btnPDF->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        grpStat->setTitle(QCoreApplication::translate("MainWindow", "Stat", nullptr));
        label_stat->setText(QCoreApplication::translate("MainWindow", "Aucune donn\303\251e", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
