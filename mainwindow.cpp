#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    QWidget *central = new QWidget;
    QHBoxLayout *rootLayout = new QHBoxLayout(central);

    QWidget *sidebar = new QWidget;
    sidebar->setFixedWidth(200);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(6,20,6,20);
    sidebarLayout->setSpacing(12);

    QStringList buttons = {
        "Gestion des locaux",
        "Gestion des parkings intelligents",
        "Gestion des equipements",
        "Gestion employe",
        "Gestion des dechts",
        "Gestion d'alerte"
    };

    for(auto &text : buttons){
        QPushButton *b = new QPushButton(text);
        b->setProperty("sky", true);
        b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        sidebarLayout->addWidget(b);
    }

    QWidget *parkingPage = createParkingPage();

    rootLayout->addWidget(sidebar);
    rootLayout->addWidget(parkingPage);

    setCentralWidget(central);
    setWindowTitle("Gestion de parking");
    resize(1200,700);

    central->setStyleSheet(R"(
        QWidget{background-color:#e6f9e6;}
        QPushButton{background-color:#0b6623;color:white;padding:6px;border-radius:6px;min-height:36px;}
        QPushButton[sky="true"]{background-color:#87ceeb;color:black;font-weight:bold;}
        QLineEdit,QComboBox,QDateTimeEdit{background:white;padding:4px;border:1px solid #ccc;border-radius:4px;}
        QTableWidget{background:white;gridline-color:#ccc;}
        QHeaderView::section{background-color:#0b6623;color:white;padding:4px;}
    )");
}

QWidget* MainWindow::createParkingPage(){
    QWidget *page = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    QHBoxLayout *topBar = new QHBoxLayout;
    QLineEdit *search = new QLineEdit;
    search->setPlaceholderText("Rechercher...");
    QPushButton *refreshBtn = new QPushButton("Rafraîchir");
    QPushButton *exportBtn = new QPushButton("Exporter");
    topBar->addWidget(search);
    topBar->addWidget(refreshBtn);
    topBar->addWidget(exportBtn);
    mainLayout->addLayout(topBar);

    QSplitter *split = new QSplitter;

    QWidget *formWidget = new QWidget;
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);
    QGroupBox *formGroup = new QGroupBox("Ajouter / Modifier Parking");
    QGridLayout *grid = new QGridLayout(formGroup);

    QLineEdit *idEdit = new QLineEdit;
    QComboBox *typeCombo = new QComboBox; typeCombo->addItems({"Public","Privé","Souterrain"});
    QLineEdit *NomParkingEdit = new QLineEdit;
    QLineEdit *PlaceslibresEdit = new QLineEdit;
    QComboBox *LocalisationCombo = new QComboBox; LocalisationCombo->addItems({"GPS","Adresse"});
    QLineEdit *CapacitéTotaleEdit = new QLineEdit;
    QComboBox *statutCombo = new QComboBox; statutCombo->addItems({"Ouverte","Fermé"});
    QLineEdit *TarifHoraireEdit = new QLineEdit;

    grid->addWidget(new QLabel("ID:"),0,0); grid->addWidget(idEdit,0,1);
    grid->addWidget(new QLabel("Type:"),1,0); grid->addWidget(typeCombo,1,1);
    grid->addWidget(new QLabel("NomParking:"),2,0); grid->addWidget(NomParkingEdit,2,1);
    grid->addWidget(new QLabel("Placeslibres:"),3,0); grid->addWidget(PlaceslibresEdit,3,1);
    grid->addWidget(new QLabel("Localisation:"),4,0); grid->addWidget(LocalisationCombo,4,1);
    grid->addWidget(new QLabel("CapacitéTotale:"),5,0); grid->addWidget(CapacitéTotaleEdit,5,1);
    grid->addWidget(new QLabel("Statut:"),6,0); grid->addWidget(statutCombo,6,1);
    grid->addWidget(new QLabel("TarifHoraire:"),7,0); grid->addWidget(TarifHoraireEdit,7,1);
    QHBoxLayout *actions = new QHBoxLayout;
    actions->addWidget(new QPushButton("Ajouter"));
    actions->addWidget(new QPushButton("Modifier"));
    actions->addWidget(new QPushButton("Supprimer"));

    formLayout->addWidget(formGroup);
    formLayout->addLayout(actions);

    QTableWidget *table = new QTableWidget;
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"ID","Type","NomParking","Placeslibres","Localisation","CapacitéTotale","Statut","TarifHoraire"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    split->addWidget(formWidget);
    split->addWidget(table);
    split->setStretchFactor(1,1);

    mainLayout->addWidget(split);
    return page;
}
