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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_14;
    QPushButton *pushButton_5;
    QLineEdit *lineEdit_13;
    QFrame *frame;
    QLabel *label_13;
    QPushButton *pushButton_3;
    QTableWidget *tableWidget;
    QPushButton *pushButton_4;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_id_2;
    QLineEdit *lineEdit_type;
    QLineEdit *lineEdit_client;
    QLineEdit *lineEdit_date;
    QLineEdit *lineEdit_gravite;
    QLineEdit *lineEdit_description;
    QPushButton *pushButton_ajouter_tab;
    QPushButton *pushButton_annuler_2;
    QLabel *label_15;
    QLineEdit *lineEdit_statut;
    QWidget *tab_2;
    QLineEdit *lineEdit_id;
    QLabel *label_7;
    QLineEdit *lineEdit_8;
    QLabel *label_8;
    QLineEdit *lineEdit_9;
    QLabel *label_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLineEdit *lineEdit_12;
    QLabel *label_10;
    QPushButton *pushButton__modifier_tab;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *pushButton_annuler;
    QLineEdit *lineEdit_15;
    QLabel *label_16;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1324, 652);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(700, 450, 41, 20));
        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(1210, 140, 101, 29));
        lineEdit_13 = new QLineEdit(centralWidget);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(600, 10, 121, 31));
        frame = new QFrame(centralWidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(730, 450, 261, 191));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(400, 10, 191, 21));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(1200, 50, 101, 29));
        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(320, 50, 801, 341));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(1200, 100, 101, 29));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 20, 311, 401));
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
        label_4->setGeometry(QRect(10, 150, 101, 20));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 190, 91, 20));
        label_6 = new QLabel(tab);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 230, 91, 20));
        lineEdit_id_2 = new QLineEdit(tab);
        lineEdit_id_2->setObjectName("lineEdit_id_2");
        lineEdit_id_2->setGeometry(QRect(120, 30, 131, 28));
        lineEdit_type = new QLineEdit(tab);
        lineEdit_type->setObjectName("lineEdit_type");
        lineEdit_type->setGeometry(QRect(120, 70, 131, 28));
        lineEdit_client = new QLineEdit(tab);
        lineEdit_client->setObjectName("lineEdit_client");
        lineEdit_client->setGeometry(QRect(120, 110, 131, 28));
        lineEdit_date = new QLineEdit(tab);
        lineEdit_date->setObjectName("lineEdit_date");
        lineEdit_date->setGeometry(QRect(120, 150, 131, 28));
        lineEdit_gravite = new QLineEdit(tab);
        lineEdit_gravite->setObjectName("lineEdit_gravite");
        lineEdit_gravite->setGeometry(QRect(120, 190, 131, 28));
        lineEdit_description = new QLineEdit(tab);
        lineEdit_description->setObjectName("lineEdit_description");
        lineEdit_description->setGeometry(QRect(120, 230, 131, 28));
        pushButton_ajouter_tab = new QPushButton(tab);
        pushButton_ajouter_tab->setObjectName("pushButton_ajouter_tab");
        pushButton_ajouter_tab->setGeometry(QRect(0, 310, 90, 29));
        pushButton_annuler_2 = new QPushButton(tab);
        pushButton_annuler_2->setObjectName("pushButton_annuler_2");
        pushButton_annuler_2->setGeometry(QRect(200, 310, 90, 29));
        label_15 = new QLabel(tab);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 270, 63, 20));
        lineEdit_statut = new QLineEdit(tab);
        lineEdit_statut->setObjectName("lineEdit_statut");
        lineEdit_statut->setGeometry(QRect(120, 270, 131, 28));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        lineEdit_id = new QLineEdit(tab_2);
        lineEdit_id->setObjectName("lineEdit_id");
        lineEdit_id->setGeometry(QRect(130, 30, 131, 28));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 70, 63, 20));
        lineEdit_8 = new QLineEdit(tab_2);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(130, 150, 131, 28));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 230, 63, 20));
        lineEdit_9 = new QLineEdit(tab_2);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(130, 230, 131, 28));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 150, 63, 20));
        lineEdit_10 = new QLineEdit(tab_2);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(130, 110, 131, 28));
        lineEdit_11 = new QLineEdit(tab_2);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(130, 190, 131, 28));
        lineEdit_12 = new QLineEdit(tab_2);
        lineEdit_12->setObjectName("lineEdit_12");
        lineEdit_12->setGeometry(QRect(130, 70, 131, 28));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(20, 190, 91, 20));
        pushButton__modifier_tab = new QPushButton(tab_2);
        pushButton__modifier_tab->setObjectName("pushButton__modifier_tab");
        pushButton__modifier_tab->setGeometry(QRect(0, 310, 90, 29));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 30, 51, 20));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, 110, 63, 20));
        pushButton_annuler = new QPushButton(tab_2);
        pushButton_annuler->setObjectName("pushButton_annuler");
        pushButton_annuler->setGeometry(QRect(180, 310, 90, 29));
        lineEdit_15 = new QLineEdit(tab_2);
        lineEdit_15->setObjectName("lineEdit_15");
        lineEdit_15->setGeometry(QRect(130, 270, 131, 28));
        label_16 = new QLabel(tab_2);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(20, 270, 63, 20));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1324, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des Clients", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Stat", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Recherche selon type:", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Trie Par Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Client", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Gravit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "statut", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Type:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Client:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Gravit\303\251:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Description:", nullptr));
        pushButton_ajouter_tab->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_annuler_2->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Statut:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Type:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Statut:", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Gravite", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Description:", nullptr));
        pushButton__modifier_tab->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Client:", nullptr));
        pushButton_annuler->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Date:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Modifier", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
