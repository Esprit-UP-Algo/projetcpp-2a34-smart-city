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
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_25;
    QLabel *label_28;
    QLabel *label;
    QLabel *label_29;
    QPushButton *pushButton_14;
    QLabel *label_24;
    QLineEdit *lineEdit_25;
    QLineEdit *lineEdit_22;
    QFrame *frame_2;
    QTableWidget *tableWidget_2;
    QLabel *label_27;
    QLabel *label_3;
    QLabel *label_9;
    QPushButton *pushButton_17;
    QPushButton *pushButton_13;
    QLineEdit *lineEdit_23;
    QLineEdit *lineEdit_21;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_26;
    QLineEdit *lineEdit_24;
    QTableWidget *tableWidget;
    QPushButton *pushButton_15;
    QPushButton *pushButton_4;
    QPushButton *pushButton_16;
    QListView *listView_2;
    QLabel *label_2;
    QLabel *label_26;
    QPushButton *pushButton_12;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1380, 714);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        label_25 = new QLabel(centralWidget);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(20, 210, 63, 20));
        label_28 = new QLabel(centralWidget);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(30, 300, 63, 20));
        label = new QLabel(centralWidget);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 150, 63, 20));
        label_29 = new QLabel(centralWidget);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(20, 330, 101, 20));
        pushButton_14 = new QPushButton(centralWidget);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(170, 100, 93, 29));
        label_24 = new QLabel(centralWidget);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(960, 580, 81, 21));
        lineEdit_25 = new QLineEdit(centralWidget);
        lineEdit_25->setObjectName("lineEdit_25");
        lineEdit_25->setGeometry(QRect(170, 330, 131, 26));
        lineEdit_22 = new QLineEdit(centralWidget);
        lineEdit_22->setObjectName("lineEdit_22");
        lineEdit_22->setGeometry(QRect(170, 210, 131, 26));
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(450, 450, 301, 141));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        tableWidget_2 = new QTableWidget(frame_2);
        if (tableWidget_2->columnCount() < 2)
            tableWidget_2->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget_2->rowCount() < 2)
            tableWidget_2->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_2->setItem(0, 0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setItem(0, 1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setItem(1, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setItem(1, 1, __qtablewidgetitem7);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(10, 10, 281, 121));
        tableWidget_2->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: #ffffff;   /* fond blanc des cellules */\n"
"    border: 2px solid #3399ff;   /* bordure bleue */\n"
"    border-radius: 6px;\n"
"    gridline-color: #e6f7f5;     /* lignes de s\303\251paration discr\303\250tes */\n"
"    selection-background-color: #4CAF50; /* fond vert quand s\303\251lectionn\303\251 */\n"
"    selection-color: white;      /* texte blanc quand s\303\251lectionn\303\251 */\n"
"    font-size: 13px;\n"
"    color: #003366; /* texte bleu fonc\303\251 */\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #4CAF50; /* vert pour l'ent\303\252te */\n"
"    color: white;\n"
"    padding: 6px;\n"
"    border: 1px solid #3399ff; /* contour bleu */\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QTableWidget::item:hover {\n"
"    background-color: #cceeff;  /* bleu clair au survol */\n"
"    color: #004d66;\n"
"}"));
        label_27 = new QLabel(centralWidget);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(30, 270, 63, 20));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(360, 470, 101, 20));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 180, 101, 20));
        pushButton_17 = new QPushButton(centralWidget);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setGeometry(QRect(770, 230, 131, 29));
        pushButton_13 = new QPushButton(centralWidget);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(60, 100, 93, 29));
        lineEdit_23 = new QLineEdit(centralWidget);
        lineEdit_23->setObjectName("lineEdit_23");
        lineEdit_23->setGeometry(QRect(170, 240, 131, 26));
        lineEdit_21 = new QLineEdit(centralWidget);
        lineEdit_21->setObjectName("lineEdit_21");
        lineEdit_21->setGeometry(QRect(170, 180, 131, 26));
        lineEdit_6 = new QLineEdit(centralWidget);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(170, 150, 131, 26));
        lineEdit_26 = new QLineEdit(centralWidget);
        lineEdit_26->setObjectName("lineEdit_26");
        lineEdit_26->setGeometry(QRect(170, 270, 131, 26));
        lineEdit_24 = new QLineEdit(centralWidget);
        lineEdit_24->setObjectName("lineEdit_24");
        lineEdit_24->setGeometry(QRect(170, 300, 131, 26));
        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(50, 380, 301, 301));
        pushButton_15 = new QPushButton(centralWidget);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(770, 150, 131, 29));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(20, 390, 93, 29));
        pushButton_16 = new QPushButton(centralWidget);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(770, 190, 131, 29));
        listView_2 = new QListView(centralWidget);
        listView_2->setObjectName("listView_2");
        listView_2->setGeometry(QRect(370, 140, 381, 301));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(370, 110, 181, 20));
        label_26 = new QLabel(centralWidget);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(20, 240, 91, 20));
        pushButton_12 = new QPushButton(centralWidget);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(140, 390, 93, 29));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(560, 110, 191, 26));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1380, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des Clients", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Capacit\303\251", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Nom ", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Tarification", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Recherche", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Parking", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Occupation", nullptr));

        const bool __sortingEnabled = tableWidget_2->isSortingEnabled();
        tableWidget_2->setSortingEnabled(false);
        tableWidget_2->setSortingEnabled(__sortingEnabled);

        label_27->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "localisation", nullptr));
        pushButton_17->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Tri par localisation", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "supprimer", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Recherche selon localistion", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Places libres", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
