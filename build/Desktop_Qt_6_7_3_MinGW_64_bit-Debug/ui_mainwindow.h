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
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton_stat;
    QPushButton *pushButton_trie;
    QLabel *label;
    QPushButton *pushButton_supprimer;
    QTableWidget *tableWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;
    QLabel *label_date;
    QLabel *label_lieu;
    QLineEdit *lineEdit_lieu;
    QLabel *label_volume;
    QLineEdit *lineEdit_volume;
    QLabel *label_etat;
    QLineEdit *lineEdit_etat;
    QPushButton *pushButton_ajouter;
    QPushButton *pushButton_annuler;
    QDateEdit *dateEdit;
    QWidget *tab_2;
    QPushButton *pushButton_modifier;
    QPushButton *pushButton_annuler_m;
    QLineEdit *lineEdit_etat_3;
    QLabel *label_nom_3;
    QLabel *label_lieu_3;
    QLabel *label_date_3;
    QLabel *label_etat_3;
    QLineEdit *lineEdit_volume_3;
    QLineEdit *lineEdit_lieu_3;
    QLineEdit *lineEdit_nom_3;
    QLabel *label_volume_3;
    QLineEdit *lineEdit_volume_4;
    QLineEdit *lineEdit_etat_4;
    QGroupBox *groupBox_2;
    QDateEdit *lineEdit_date_3;
    QLabel *logo;
    QPushButton *pushButton_pdf;
    QComboBox *comboBox;
    QLabel *label_recherche;
    QLineEdit *lineEdit_recherche;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1645, 977);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pushButton_stat = new QPushButton(centralwidget);
        pushButton_stat->setObjectName("pushButton_stat");
        pushButton_stat->setGeometry(QRect(660, 580, 120, 32));
        pushButton_stat->setStyleSheet(QString::fromUtf8("\n"
"      QPushButton {\n"
"          background-color: #4CAF50;\n"
"          color: white;\n"
"          border-radius: 6px;\n"
"          padding: 6px;\n"
"      }\n"
"      QPushButton:hover {\n"
"          background-color: #45a049;\n"
"      }\n"
"     "));
        pushButton_trie = new QPushButton(centralwidget);
        pushButton_trie->setObjectName("pushButton_trie");
        pushButton_trie->setGeometry(QRect(850, 190, 81, 31));
        pushButton_trie->setStyleSheet(QString::fromUtf8("\n"
"      QPushButton {\n"
"          background-color: #4CAF50;\n"
"          color: white;\n"
"          border-radius: 6px;\n"
"          padding: 6px;\n"
"      }\n"
"      QPushButton:hover {\n"
"          background-color: #45a049;\n"
"      }\n"
"     "));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(580, 40, 241, 131));
        label->setStyleSheet(QString::fromUtf8("font: 20pt \"Segoe UI\";"));
        pushButton_supprimer = new QPushButton(centralwidget);
        pushButton_supprimer->setObjectName("pushButton_supprimer");
        pushButton_supprimer->setGeometry(QRect(930, 580, 120, 32));
        pushButton_supprimer->setStyleSheet(QString::fromUtf8("\n"
"      QPushButton {\n"
"          background-color: #4CAF50;\n"
"          color: white;\n"
"          border-radius: 6px;\n"
"          padding: 6px;\n"
"      }\n"
"      QPushButton:hover {\n"
"          background-color: #45a049;\n"
"      }\n"
"     "));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(460, 230, 601, 341));
        tableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(217, 244, 255);\n"
"color: rgb(0, 0, 0);\n"
"border-color: rgb(51, 153, 255);"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(90, 192, 340, 401));
        tab = new QWidget();
        tab->setObjectName("tab");
        label_nom = new QLabel(tab);
        label_nom->setObjectName("label_nom");
        label_nom->setGeometry(QRect(10, 30, 111, 20));
        lineEdit_nom = new QLineEdit(tab);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setGeometry(QRect(130, 30, 180, 28));
        label_date = new QLabel(tab);
        label_date->setObjectName("label_date");
        label_date->setGeometry(QRect(10, 70, 121, 20));
        label_lieu = new QLabel(tab);
        label_lieu->setObjectName("label_lieu");
        label_lieu->setGeometry(QRect(10, 110, 121, 20));
        lineEdit_lieu = new QLineEdit(tab);
        lineEdit_lieu->setObjectName("lineEdit_lieu");
        lineEdit_lieu->setGeometry(QRect(130, 110, 180, 28));
        label_volume = new QLabel(tab);
        label_volume->setObjectName("label_volume");
        label_volume->setGeometry(QRect(10, 150, 121, 20));
        lineEdit_volume = new QLineEdit(tab);
        lineEdit_volume->setObjectName("lineEdit_volume");
        lineEdit_volume->setGeometry(QRect(130, 150, 180, 28));
        label_etat = new QLabel(tab);
        label_etat->setObjectName("label_etat");
        label_etat->setGeometry(QRect(10, 190, 111, 20));
        lineEdit_etat = new QLineEdit(tab);
        lineEdit_etat->setObjectName("lineEdit_etat");
        lineEdit_etat->setGeometry(QRect(130, 190, 180, 28));
        pushButton_ajouter = new QPushButton(tab);
        pushButton_ajouter->setObjectName("pushButton_ajouter");
        pushButton_ajouter->setGeometry(QRect(20, 310, 120, 32));
        pushButton_annuler = new QPushButton(tab);
        pushButton_annuler->setObjectName("pushButton_annuler");
        pushButton_annuler->setGeometry(QRect(190, 310, 120, 32));
        dateEdit = new QDateEdit(tab);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(130, 70, 181, 29));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        pushButton_modifier = new QPushButton(tab_2);
        pushButton_modifier->setObjectName("pushButton_modifier");
        pushButton_modifier->setGeometry(QRect(20, 260, 120, 32));
        pushButton_annuler_m = new QPushButton(tab_2);
        pushButton_annuler_m->setObjectName("pushButton_annuler_m");
        pushButton_annuler_m->setGeometry(QRect(190, 260, 120, 32));
        lineEdit_etat_3 = new QLineEdit(tab_2);
        lineEdit_etat_3->setObjectName("lineEdit_etat_3");
        lineEdit_etat_3->setGeometry(QRect(130, 180, 180, 28));
        label_nom_3 = new QLabel(tab_2);
        label_nom_3->setObjectName("label_nom_3");
        label_nom_3->setGeometry(QRect(10, 20, 111, 20));
        label_lieu_3 = new QLabel(tab_2);
        label_lieu_3->setObjectName("label_lieu_3");
        label_lieu_3->setGeometry(QRect(10, 100, 121, 20));
        label_date_3 = new QLabel(tab_2);
        label_date_3->setObjectName("label_date_3");
        label_date_3->setGeometry(QRect(10, 60, 121, 20));
        label_etat_3 = new QLabel(tab_2);
        label_etat_3->setObjectName("label_etat_3");
        label_etat_3->setGeometry(QRect(10, 180, 111, 20));
        lineEdit_volume_3 = new QLineEdit(tab_2);
        lineEdit_volume_3->setObjectName("lineEdit_volume_3");
        lineEdit_volume_3->setGeometry(QRect(130, 140, 180, 28));
        lineEdit_lieu_3 = new QLineEdit(tab_2);
        lineEdit_lieu_3->setObjectName("lineEdit_lieu_3");
        lineEdit_lieu_3->setGeometry(QRect(130, 100, 180, 28));
        lineEdit_nom_3 = new QLineEdit(tab_2);
        lineEdit_nom_3->setObjectName("lineEdit_nom_3");
        lineEdit_nom_3->setGeometry(QRect(130, 20, 180, 28));
        label_volume_3 = new QLabel(tab_2);
        label_volume_3->setObjectName("label_volume_3");
        label_volume_3->setGeometry(QRect(10, 140, 121, 20));
        lineEdit_volume_4 = new QLineEdit(tab_2);
        lineEdit_volume_4->setObjectName("lineEdit_volume_4");
        lineEdit_volume_4->setGeometry(QRect(130, 100, 180, 28));
        lineEdit_etat_4 = new QLineEdit(tab_2);
        lineEdit_etat_4->setObjectName("lineEdit_etat_4");
        lineEdit_etat_4->setGeometry(QRect(130, 140, 180, 28));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(258, -233, 120, 80));
        lineEdit_date_3 = new QDateEdit(tab_2);
        lineEdit_date_3->setObjectName("lineEdit_date_3");
        lineEdit_date_3->setGeometry(QRect(130, 58, 181, 31));
        tabWidget->addTab(tab_2, QString());
        logo = new QLabel(centralwidget);
        logo->setObjectName("logo");
        logo->setGeometry(QRect(880, 20, 141, 151));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/helabhima-removebg-preview.png")));
        logo->setScaledContents(true);
        pushButton_pdf = new QPushButton(centralwidget);
        pushButton_pdf->setObjectName("pushButton_pdf");
        pushButton_pdf->setGeometry(QRect(790, 580, 121, 31));
        pushButton_pdf->setStyleSheet(QString::fromUtf8("\n"
"      QPushButton {\n"
"          background-color: #4CAF50;\n"
"          color: white;\n"
"          border-radius: 6px;\n"
"          padding: 6px;\n"
"      }\n"
"      QPushButton:hover {\n"
"          background-color: #45a049;\n"
"      }\n"
"     "));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(950, 190, 82, 31));
        label_recherche = new QLabel(centralwidget);
        label_recherche->setObjectName("label_recherche");
        label_recherche->setGeometry(QRect(470, 190, 111, 21));
        lineEdit_recherche = new QLineEdit(centralwidget);
        lineEdit_recherche->setObjectName("lineEdit_recherche");
        lineEdit_recherche->setGeometry(QRect(590, 190, 200, 28));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1645, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_stat->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques", nullptr));
        pushButton_trie->setText(QCoreApplication::translate("MainWindow", "\360\237\224\244 Trier", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Smart city ", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221 Supprimer", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Cn", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Date de collecte", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Lieu de collecte", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        label_nom->setText(QCoreApplication::translate("MainWindow", "Cn", nullptr));
        label_date->setText(QCoreApplication::translate("MainWindow", "Date de collecte", nullptr));
        label_lieu->setText(QCoreApplication::translate("MainWindow", "Lieu de collecte", nullptr));
        label_volume->setText(QCoreApplication::translate("MainWindow", "Volume/Poids", nullptr));
        label_etat->setText(QCoreApplication::translate("MainWindow", "\303\211tat", nullptr));
        pushButton_ajouter->setText(QCoreApplication::translate("MainWindow", "\342\234\224 Ajouter", nullptr));
        pushButton_annuler->setText(QCoreApplication::translate("MainWindow", "\342\234\226 Annuler", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("MainWindow", "\342\234\224 valider", nullptr));
        pushButton_annuler_m->setText(QCoreApplication::translate("MainWindow", "\342\234\226 Annuler", nullptr));
        label_nom_3->setText(QCoreApplication::translate("MainWindow", "Cn", nullptr));
        label_lieu_3->setText(QCoreApplication::translate("MainWindow", "Lieu de collecte", nullptr));
        label_date_3->setText(QCoreApplication::translate("MainWindow", "Date de collecte", nullptr));
        label_etat_3->setText(QCoreApplication::translate("MainWindow", "\303\211tat", nullptr));
        label_volume_3->setText(QCoreApplication::translate("MainWindow", "Volume/Poids", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\342\234\217 Modifier", nullptr));
        logo->setText(QString());
        pushButton_pdf->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 PDF", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "cn", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "localisation", nullptr));
        comboBox->setItemText(2, QString());

        label_recherche->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Recherche", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
