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
#include <QtWidgets/QComboBox>
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
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;
    QLabel *label_date;
    QLineEdit *lineEdit_date;
    QLabel *label_lieu;
    QLineEdit *lineEdit_lieu;
    QLabel *label_volume;
    QLineEdit *lineEdit_volume;
    QLabel *label_etat;
    QLineEdit *lineEdit_etat;
    QPushButton *pushButton_ajouter;
    QPushButton *pushButton_annuler;
    QWidget *tab_2;
    QPushButton *pushButton_modifier;
    QPushButton *pushButton_annuler_m;
    QLineEdit *lineEdit_date_3;
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
    QLabel *label_recherche;
    QLineEdit *lineEdit_recherche;
    QPushButton *pushButton_trie;
    QPushButton *pushButton_supprimer;
    QPushButton *pushButton_pdf;
    QPushButton *pushButton_stat;
    QTableWidget *tableWidget;
    QComboBox *comboBox;
    QLabel *logo;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1381, 709);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"QWidget {\n"
"    background-color: #e6f7f5;\n"
"    color: #003366;\n"
"    font-size: 14px;\n"
"    font-family: \"Segoe UI\", sans-serif;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background-color: #3399ff;\n"
"    color: white;\n"
"    border: none;\n"
"    padding: 6px 14px;\n"
"    border-radius: 8px;\n"
"    font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #0066cc;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #004c99;\n"
"}\n"
"\n"
"QLineEdit, QTextEdit {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #3399ff;\n"
"    border-radius: 6px;\n"
"    padding: 4px;\n"
"}\n"
"QLineEdit:focus, QTextEdit:focus {\n"
"    border: 2px solid #0066cc;\n"
"    background-color: #f0fbff;\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #004d66;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTabWidget::pane {\n"
"    border: 2px solid #3399ff;\n"
"    border-radius: 8px;\n"
"    background: #ffffff;\n"
"    margin-top: 2px;\n"
"}\n"
"QTabBar::tab {\n"
"    background: #3399ff;\n"
""
                        "    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 8px 16px;\n"
"    margin: 2px;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background: #0066cc;\n"
"}\n"
"QTabBar::tab:hover {\n"
"    background: #5dade2;\n"
"}\n"
"\n"
"QListView {\n"
"    background-color: #ffffff;\n"
"    border: 2px solid #3399ff;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"    font-size: 13px;\n"
"}\n"
"QListView::item {\n"
"    padding: 8px;\n"
"    border-bottom: 1px solid #e6f7f5;\n"
"    color: #003366;\n"
"}\n"
"QListView::item:selected {\n"
"    background-color: #3399ff;\n"
"    color: white;\n"
"    border-radius: 4px;\n"
"}\n"
"QListView::item:hover {\n"
"    background-color: #cceeff;\n"
"}\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(50, 170, 340, 401));
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
        lineEdit_date = new QLineEdit(tab);
        lineEdit_date->setObjectName("lineEdit_date");
        lineEdit_date->setGeometry(QRect(130, 70, 180, 28));
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
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        pushButton_modifier = new QPushButton(tab_2);
        pushButton_modifier->setObjectName("pushButton_modifier");
        pushButton_modifier->setGeometry(QRect(20, 310, 120, 32));
        pushButton_annuler_m = new QPushButton(tab_2);
        pushButton_annuler_m->setObjectName("pushButton_annuler_m");
        pushButton_annuler_m->setGeometry(QRect(190, 310, 120, 32));
        lineEdit_date_3 = new QLineEdit(tab_2);
        lineEdit_date_3->setObjectName("lineEdit_date_3");
        lineEdit_date_3->setGeometry(QRect(130, 60, 180, 28));
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
        tabWidget->addTab(tab_2, QString());
        label_recherche = new QLabel(centralwidget);
        label_recherche->setObjectName("label_recherche");
        label_recherche->setGeometry(QRect(430, 170, 111, 21));
        lineEdit_recherche = new QLineEdit(centralwidget);
        lineEdit_recherche->setObjectName("lineEdit_recherche");
        lineEdit_recherche->setGeometry(QRect(550, 170, 200, 28));
        pushButton_trie = new QPushButton(centralwidget);
        pushButton_trie->setObjectName("pushButton_trie");
        pushButton_trie->setGeometry(QRect(810, 170, 81, 31));
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
        pushButton_supprimer = new QPushButton(centralwidget);
        pushButton_supprimer->setObjectName("pushButton_supprimer");
        pushButton_supprimer->setGeometry(QRect(890, 560, 120, 32));
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
        pushButton_pdf = new QPushButton(centralwidget);
        pushButton_pdf->setObjectName("pushButton_pdf");
        pushButton_pdf->setGeometry(QRect(750, 560, 121, 31));
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
        pushButton_stat = new QPushButton(centralwidget);
        pushButton_stat->setObjectName("pushButton_stat");
        pushButton_stat->setGeometry(QRect(620, 560, 120, 32));
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
        if (tableWidget->rowCount() < 5)
            tableWidget->setRowCount(5);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setItem(0, 3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setItem(1, 0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget->setItem(1, 1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget->setItem(1, 2, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget->setItem(1, 3, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget->setItem(2, 0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget->setItem(2, 1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget->setItem(2, 2, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget->setItem(2, 3, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget->setItem(3, 0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget->setItem(3, 1, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget->setItem(3, 2, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget->setItem(3, 3, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget->setItem(4, 0, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableWidget->setItem(4, 1, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tableWidget->setItem(4, 2, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tableWidget->setItem(4, 3, __qtablewidgetitem28);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(420, 210, 601, 341));
        tableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(217, 244, 255);\n"
"border-color: rgb(51, 153, 255);"));
        comboBox = new QComboBox(centralwidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(910, 170, 82, 31));
        logo = new QLabel(centralwidget);
        logo->setObjectName("logo");
        logo->setGeometry(QRect(840, 0, 141, 151));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/helabhima-removebg-preview.png")));
        logo->setScaledContents(true);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(540, 20, 241, 131));
        label->setStyleSheet(QString::fromUtf8("font: 20pt \"Segoe UI\";"));
        MainWindow->setCentralWidget(centralwidget);
        tableWidget->raise();
        tabWidget->raise();
        label_recherche->raise();
        lineEdit_recherche->raise();
        pushButton_trie->raise();
        pushButton_supprimer->raise();
        pushButton_pdf->raise();
        pushButton_stat->raise();
        comboBox->raise();
        logo->raise();
        label->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1381, 24));
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des Collectes", nullptr));
        label_nom->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
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
        label_recherche->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Recherche", nullptr));
        pushButton_trie->setText(QCoreApplication::translate("MainWindow", "\360\237\224\244 Trier", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221 Supprimer", nullptr));
        pushButton_pdf->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 PDF", nullptr));
        pushButton_stat->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Cn", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Date de collecte", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Lieu de collecte", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Nouvelle ligne", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Nouvelle ligne", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Nouvelle ligne", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Nouvelle ligne", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Nouvelle ligne", nullptr));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->item(0, 0);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "N225", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->item(0, 1);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "10/02", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->item(0, 2);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "tunis", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->item(0, 3);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "oui", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->item(1, 0);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "N452", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->item(1, 1);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "15/8/24", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget->item(1, 2);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "ariana2", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget->item(1, 3);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "non", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget->item(2, 0);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "N254", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget->item(2, 1);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "14/7/24", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget->item(2, 2);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "regueb", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget->item(2, 3);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "oui", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget->item(3, 0);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "N135", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget->item(3, 1);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "16/3/24", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget->item(3, 2);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "gabes", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget->item(3, 3);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "non", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidget->item(4, 0);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "N145", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableWidget->item(4, 1);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "22/1/24", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tableWidget->item(4, 2);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "bni khaled", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = tableWidget->item(4, 3);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "oui", nullptr));
        tableWidget->setSortingEnabled(__sortingEnabled);

        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "cn", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "localisation", nullptr));
        comboBox->setItemText(2, QString());

        logo->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Smart city ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
