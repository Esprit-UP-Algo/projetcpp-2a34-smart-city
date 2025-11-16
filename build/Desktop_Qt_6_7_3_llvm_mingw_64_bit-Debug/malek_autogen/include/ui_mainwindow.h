/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_cin;
    QLineEdit *lineEdit_nom;
    QLineEdit *lineEdit_prenom;
    QLineEdit *lineEdit_email;
    QLineEdit *lineEdit_motdepass;
    QLineEdit *lineEdit_telephon;
    QPushButton *ajouter;
    QPushButton *pushButton_7;
    QLabel *label_15;
    QLineEdit *lineEdit_poste;
    QLabel *label_16;
    QLineEdit *lineEdit_salaire;
    QWidget *tab_2;
    QLineEdit *lineEdit_cin2;
    QLabel *label_7;
    QLineEdit *lineEdit_email2;
    QLabel *label_8;
    QLineEdit *lineEdit_tel2;
    QLabel *label_9;
    QLineEdit *lineEdit_prenom2;
    QLineEdit *lineEdit_motdepass2;
    QLineEdit *lineEdit_nom2;
    QLabel *label_10;
    QPushButton *modifier;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *pushButton_6;
    QLineEdit *lineEdit_poste_2;
    QLabel *label_17;
    QLabel *label_18;
    QLineEdit *lineEdit_salaire_2;
    QWidget *tab_3;
    QPushButton *refresh_3;
    QListView *listView;
    QLabel *label_14;
    QPushButton *refresh;
    QLineEdit *lineEdit_13;
    QPushButton *pushButton_5;
    QPushButton *supprimer;
    QTableView *tab_emp;
    QFrame *frame;
    QPushButton *refresh_2;
    QComboBox *comboBox;
    QLabel *label_13;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(2102, 837);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: #e6f7f5; /* light greenish background */\n"
"    color: #003366; /* dark blue text */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #4CAF50; /* green button */\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 6px 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #45a049; /* darker green on hover */\n"
"}\n"
"\n"
"QLineEdit, QTextEdit {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #3399ff; /* blue border */\n"
"    border-radius: 4px;\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QLineEdit:focus, QTextEdit:focus {\n"
"    border: 2px solid #0066cc; /* darker blue when focused */\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #004d66; /* teal text */\n"
"    font-weight: bold;\n"
"}\n"
"QTabWidget::pane {\n"
"    border: 2px solid #3399ff; /* blue border */\n"
"    border-radius: 6px;\n"
"    background: #e6f7f5; /* light greenish background */\n"
"    padding: 4px;\n"
"}\n"
"\n"
"QTabBar"
                        "::tab {\n"
"    background: #4CAF50; /* green tabs */\n"
"    color: white;\n"
"    border: 1px solid #3399ff; /* blue border */\n"
"    border-radius: 6px;\n"
"    padding: 6px 12px;\n"
"    margin-right: 2px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: #3399ff; /* blue when selected */\n"
"    color: white;\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background: #45a049; /* darker green hover */\n"
"}\n"
"QListView {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #3399ff; /* blue border */\n"
"    border-radius: 6px;\n"
"    padding: 4px;\n"
"    show-decoration-selected: 1; /* highlight whole row */\n"
"}\n"
"\n"
"QListView::item {\n"
"    padding: 6px;\n"
"    border-bottom: 1px solid #e6f7f5; /* subtle divider */\n"
"    color: #003366; /* dark blue text */\n"
"}\n"
"\n"
"QListView::item:selected {\n"
"    background-color: #4CAF50; /* green selection */\n"
"    color: white;\n"
"    border: none;\n"
"}\n"
"\n"
"QListView::item:hover {\n"
"    background-color: #cceeff; /* lig"
                        "ht blue hover */\n"
"    color: #004d66;\n"
"}\n"
"\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(270, 30, 311, 461));
        tab = new QWidget();
        tab->setObjectName("tab");
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 30, 51, 20));
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 70, 63, 20));
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 110, 63, 20));
        label_4 = new QLabel(tab);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 150, 63, 20));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 190, 91, 20));
        label_6 = new QLabel(tab);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 230, 63, 20));
        lineEdit_cin = new QLineEdit(tab);
        lineEdit_cin->setObjectName("lineEdit_cin");
        lineEdit_cin->setGeometry(QRect(120, 30, 131, 28));
        lineEdit_nom = new QLineEdit(tab);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(120, 70, 131, 28));
        lineEdit_prenom = new QLineEdit(tab);
        lineEdit_prenom->setObjectName("lineEdit_prenom");
        lineEdit_prenom->setGeometry(QRect(120, 110, 131, 28));
        lineEdit_email = new QLineEdit(tab);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setGeometry(QRect(120, 150, 131, 28));
        lineEdit_motdepass = new QLineEdit(tab);
        lineEdit_motdepass->setObjectName("lineEdit_motdepass");
        lineEdit_motdepass->setGeometry(QRect(120, 190, 131, 28));
        lineEdit_telephon = new QLineEdit(tab);
        lineEdit_telephon->setObjectName("lineEdit_telephon");
        lineEdit_telephon->setGeometry(QRect(120, 230, 131, 28));
        ajouter = new QPushButton(tab);
        ajouter->setObjectName("ajouter");
        ajouter->setGeometry(QRect(0, 350, 90, 29));
        pushButton_7 = new QPushButton(tab);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(200, 350, 90, 29));
        label_15 = new QLabel(tab);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 270, 63, 20));
        lineEdit_poste = new QLineEdit(tab);
        lineEdit_poste->setObjectName("lineEdit_poste");
        lineEdit_poste->setGeometry(QRect(120, 270, 131, 28));
        label_16 = new QLabel(tab);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(10, 310, 63, 20));
        lineEdit_salaire = new QLineEdit(tab);
        lineEdit_salaire->setObjectName("lineEdit_salaire");
        lineEdit_salaire->setGeometry(QRect(120, 320, 131, 28));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        lineEdit_cin2 = new QLineEdit(tab_2);
        lineEdit_cin2->setObjectName("lineEdit_cin2");
        lineEdit_cin2->setGeometry(QRect(130, 30, 131, 28));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 70, 63, 20));
        lineEdit_email2 = new QLineEdit(tab_2);
        lineEdit_email2->setObjectName("lineEdit_email2");
        lineEdit_email2->setGeometry(QRect(130, 150, 131, 28));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 230, 63, 20));
        lineEdit_tel2 = new QLineEdit(tab_2);
        lineEdit_tel2->setObjectName("lineEdit_tel2");
        lineEdit_tel2->setGeometry(QRect(130, 230, 131, 28));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 150, 63, 20));
        lineEdit_prenom2 = new QLineEdit(tab_2);
        lineEdit_prenom2->setObjectName("lineEdit_prenom2");
        lineEdit_prenom2->setGeometry(QRect(130, 110, 131, 28));
        lineEdit_motdepass2 = new QLineEdit(tab_2);
        lineEdit_motdepass2->setObjectName("lineEdit_motdepass2");
        lineEdit_motdepass2->setGeometry(QRect(130, 190, 131, 28));
        lineEdit_nom2 = new QLineEdit(tab_2);
        lineEdit_nom2->setObjectName("lineEdit_nom2");
        lineEdit_nom2->setGeometry(QRect(130, 70, 131, 28));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(20, 190, 91, 20));
        modifier = new QPushButton(tab_2);
        modifier->setObjectName("modifier");
        modifier->setGeometry(QRect(0, 370, 90, 29));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 30, 51, 20));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, 110, 63, 20));
        pushButton_6 = new QPushButton(tab_2);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(180, 370, 90, 29));
        lineEdit_poste_2 = new QLineEdit(tab_2);
        lineEdit_poste_2->setObjectName("lineEdit_poste_2");
        lineEdit_poste_2->setGeometry(QRect(130, 270, 131, 28));
        label_17 = new QLabel(tab_2);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(20, 310, 63, 20));
        label_18 = new QLabel(tab_2);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(20, 270, 63, 20));
        lineEdit_salaire_2 = new QLineEdit(tab_2);
        lineEdit_salaire_2->setObjectName("lineEdit_salaire_2");
        lineEdit_salaire_2->setGeometry(QRect(130, 320, 131, 28));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        refresh_3 = new QPushButton(tab_3);
        refresh_3->setObjectName("refresh_3");
        refresh_3->setGeometry(QRect(70, 360, 141, 29));
        listView = new QListView(tab_3);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(10, 30, 281, 281));
        tabWidget->addTab(tab_3, QString());
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(1100, 790, 41, 20));
        refresh = new QPushButton(centralwidget);
        refresh->setObjectName("refresh");
        refresh->setGeometry(QRect(1090, 30, 101, 29));
        lineEdit_13 = new QLineEdit(centralwidget);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(870, 30, 121, 31));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(1260, 310, 101, 51));
        supprimer = new QPushButton(centralwidget);
        supprimer->setObjectName("supprimer");
        supprimer->setGeometry(QRect(1260, 220, 101, 51));
        tab_emp = new QTableView(centralwidget);
        tab_emp->setObjectName("tab_emp");
        tab_emp->setGeometry(QRect(620, 90, 601, 391));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(1130, 790, 261, 191));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        refresh_2 = new QPushButton(centralwidget);
        refresh_2->setObjectName("refresh_2");
        refresh_2->setGeometry(QRect(690, 30, 151, 29));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(1320, 120, 91, 31));
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(1260, 130, 61, 20));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 2102, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "N Tel", nullptr));
        ajouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        lineEdit_cin2->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "N Tel", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        modifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        refresh_3->setText(QCoreApplication::translate("MainWindow", "afficher_stat", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "stat", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Stat", nullptr));
        refresh->setText(QCoreApplication::translate("MainWindow", "refresh", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        refresh_2->setText(QCoreApplication::translate("MainWindow", "Recherche Par Cin", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "trie par salaire", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "trie par nom", nullptr));

        label_13->setText(QCoreApplication::translate("MainWindow", "Trie", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
