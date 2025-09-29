// mainwindow.cpp
#include "mainwindow.h"
#include <QHeaderView>
#include <QSplitter>
#include <QTableWidgetItem>
#include <QSizePolicy>
#include <QFont>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    QWidget *central = createLocauxInterface();
    central->setObjectName("centralWidget");
    setCentralWidget(central);
    setWindowTitle("Gestion des Locaux");
    resize(1300, 820);

    QString style = R"(
        QWidget#centralWidget { background-color: #e6f9e6; }
        QPushButton {
            background-color: #0b6623;
            color: white;
            border: none;
            padding: 6px;
            border-radius: 6px;
            min-height: 36px;
            min-width: 120px;
        }
        QPushButton:hover { opacity: 0.95; }
        QPushButton[sky="true"] {
            background-color: rgba(135,206,235,0.85);
            color: #062b3a;
            border: none;
            padding: 6px 10px;
            margin: 6px 4px;
            border-radius: 8px;
            min-height: 30px;
            min-width: 90px;
            font-weight: 500;
            box-shadow: none;
        }
        QPushButton[sky="true"]:hover {
            background-color: rgba(135,206,235,1.0);
        }
        QLineEdit, QComboBox, QSpinBox {
            background: white;
            padding: 6px;
            border: 1px solid #cfeac9;
            border-radius: 6px;
            min-height: 34px;
            min-width: 220px;
        }
        QGroupBox {
            border: 1px solid #b7e0b7;
            border-radius: 8px;
            padding: 10px;
            margin-top: 6px;
            background: rgba(255,255,255,0.75);
        }
        QTableWidget {
            background: white;
            gridline-color: #dfeadc;
            border-radius: 6px;
        }
        QHeaderView::section {
            background: #cde9c8;
            padding: 6px;
            border: 0px;
        }
    )";
    central->setStyleSheet(style);
}

QWidget* MainWindow::createLocauxInterface() {
    QWidget *root = new QWidget;
    QHBoxLayout *rootLayout = new QHBoxLayout(root);
    rootLayout->setContentsMargins(10,10,10,10);
    rootLayout->setSpacing(12);

    QWidget *leftPanel = new QWidget;
    leftPanel->setFixedWidth(120);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(6,12,6,12);
    leftLayout->setSpacing(6);

    QStringList labels = {
        "locaux",
        "Parkings",
        "Equipements",
        "Employe",
        "Dechts",
        "alerte"
    };

    for (int i = 0; i < labels.size(); ++i) {
        QPushButton *b = new QPushButton(labels[i]);
        b->setProperty("sky", true);
        b->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        b->setFixedHeight(30);
        b->setMinimumWidth(90);
        leftLayout->addWidget(b, 0, Qt::AlignHCenter);
    }

    leftLayout->insertStretch(0, 1);
    leftLayout->addStretch(1);
    QLabel *lblExtras = new QLabel("Extras");
    lblExtras->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(lblExtras);

    QWidget *mainArea = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(mainArea);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(10);

    QHBoxLayout *topBar = new QHBoxLayout;
    QLineEdit *search = new QLineEdit;
    search->setPlaceholderText("Rechercher un local...");
    search->setMinimumWidth(380);
    QPushButton *btnRefresh = new QPushButton("Rafraîchir");
    topBar->addWidget(search);
    topBar->addStretch();
    topBar->addWidget(btnRefresh);
    mainLayout->addLayout(topBar);

    QSplitter *split = new QSplitter;
    split->setOrientation(Qt::Horizontal);

    QWidget *formWidget = new QWidget;
    formWidget->setMinimumWidth(460);
    QVBoxLayout *formArea = new QVBoxLayout(formWidget);
    formArea->setContentsMargins(6,6,6,6);
    formArea->setSpacing(10);

    QGroupBox *formBox = new QGroupBox("Ajouter / Modifier un Local");
    QGridLayout *formLayout = new QGridLayout;
    formLayout->setContentsMargins(8,8,8,8);
    formLayout->setHorizontalSpacing(12);
    formLayout->setVerticalSpacing(10);

    nomEdit = new QLineEdit;
    numEdit = new QLineEdit;
    aireSpin = new QSpinBox;
    aireSpin->setMaximum(100000);
    locEdit = new QLineEdit;
    chambresSpin = new QSpinBox;
    chambresSpin->setMaximum(1000);
    typeCombo = new QComboBox;
    typeCombo->addItems({"Appartement", "Maison", "Bureau", "Autre"});
    statutCombo = new QComboBox;
    statutCombo->addItems({"À louer", "À vendre", "Occupé"});
    prixSpin = new QSpinBox;
    prixSpin->setMaximum(1000000000);

    const int fldMinW = 240;
    const int fldH = 34;
    auto setUniform = [&](QWidget *w){
        w->setMinimumWidth(fldMinW);
        w->setMinimumHeight(fldH);
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    };
    setUniform(nomEdit); setUniform(numEdit); setUniform(aireSpin);
    setUniform(locEdit); setUniform(chambresSpin); setUniform(typeCombo);
    setUniform(statutCombo); setUniform(prixSpin);

    formLayout->addWidget(new QLabel("Nom Propriétaire:"), 0, 0, Qt::AlignRight);
    formLayout->addWidget(nomEdit, 0, 1);
    formLayout->addWidget(new QLabel("Numéro Propriétaire:"), 1, 0, Qt::AlignRight);
    formLayout->addWidget(numEdit, 1, 1);
    formLayout->addWidget(new QLabel("Aire (m²):"), 2, 0, Qt::AlignRight);
    formLayout->addWidget(aireSpin, 2, 1);
    formLayout->addWidget(new QLabel("Localisation:"), 3, 0, Qt::AlignRight);
    formLayout->addWidget(locEdit, 3, 1);
    formLayout->addWidget(new QLabel("Nombre de Chambres:"), 4, 0, Qt::AlignRight);
    formLayout->addWidget(chambresSpin, 4, 1);
    formLayout->addWidget(new QLabel("Type:"), 5, 0, Qt::AlignRight);
    formLayout->addWidget(typeCombo, 5, 1);
    formLayout->addWidget(new QLabel("Statut:"), 6, 0, Qt::AlignRight);
    formLayout->addWidget(statutCombo, 6, 1);
    formLayout->addWidget(new QLabel("Prix (DT):"), 7, 0, Qt::AlignRight);
    formLayout->addWidget(prixSpin, 7, 1);

    QHBoxLayout *btnsRow = new QHBoxLayout;
    btnsRow->setSpacing(12);
    QPushButton *btnAdd = new QPushButton("Ajouter");
    QPushButton *btnEdit = new QPushButton("Modifier");
    QPushButton *btnDelete = new QPushButton("Supprimer");
    btnAdd->setMinimumWidth(120); btnEdit->setMinimumWidth(120); btnDelete->setMinimumWidth(120);
    btnAdd->setMinimumHeight(36); btnEdit->setMinimumHeight(36); btnDelete->setMinimumHeight(36);
    btnAdd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnDelete->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnsRow->addWidget(btnAdd);
    btnsRow->addWidget(btnEdit);
    btnsRow->addWidget(btnDelete);

    QVBoxLayout *formBoxLayout = new QVBoxLayout;
    formBoxLayout->addLayout(formLayout);
    formBoxLayout->addSpacing(6);
    formBoxLayout->addLayout(btnsRow);
    formBox->setLayout(formBoxLayout);

    formArea->addWidget(formBox);
    formArea->addStretch();

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addLocal);

    QWidget *tableWidget = new QWidget;
    QVBoxLayout *tableLayout = new QVBoxLayout(tableWidget);
    tableLayout->setContentsMargins(6,6,6,6);
    tableLayout->setSpacing(8);

    table = new QTableWidget;
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({
        "Propriétaire", "Numéro", "Aire (m²)", "Localisation",
        "Chambres", "Type", "Statut", "Prix (DT)"
    });
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultSectionSize(30);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont tableFont = table->font();
    tableFont.setPointSize(tableFont.pointSize() + 1);
    table->setFont(tableFont);

    tableLayout->addWidget(table);

    split->addWidget(formWidget);
    split->addWidget(tableWidget);
    split->setStretchFactor(0, 0);
    split->setStretchFactor(1, 1);

    mainLayout->addWidget(split);

    rootLayout->addWidget(leftPanel);
    rootLayout->addWidget(mainArea, 1);

    return root;
}

void MainWindow::addLocal() {
    int row = table->rowCount();
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(nomEdit->text()));
    table->setItem(row, 1, new QTableWidgetItem(numEdit->text()));
    table->setItem(row, 2, new QTableWidgetItem(QString::number(aireSpin->value())));
    table->setItem(row, 3, new QTableWidgetItem(locEdit->text()));
    table->setItem(row, 4, new QTableWidgetItem(QString::number(chambresSpin->value())));
    table->setItem(row, 5, new QTableWidgetItem(typeCombo->currentText()));
    table->setItem(row, 6, new QTableWidgetItem(statutCombo->currentText()));
    table->setItem(row, 7, new QTableWidgetItem(QString::number(prixSpin->value())));
    nomEdit->clear();
    numEdit->clear();
    aireSpin->setValue(0);
    locEdit->clear();
    chambresSpin->setValue(0);
    prixSpin->setValue(0);
}
