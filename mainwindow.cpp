#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QDebug>
#include <QPushButton>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "=== DEMARRAGE APPLICATION ===";

    // 🔹 CONNEXIONS MANUELLES DES BOUTONS
    qDebug() << "Connexion des boutons...";

    if (ui->modifier_bt_2) {
        connect(ui->modifier_bt_2, &QPushButton::clicked,
                this, &MainWindow::on_modifier_bt_clicked);
        qDebug() << "✅ Bouton modifier_bt_2 connecté";
    } else {
        qDebug() << "❌ modifier_bt_2 n'existe pas";
    }

    if (ui->ajouter_bt) {
        connect(ui->ajouter_bt, &QPushButton::clicked,
                this, &MainWindow::on_ajouter_bt_clicked);
        qDebug() << "✅ Bouton ajouter_bt connecté";
    } else {
        qDebug() << "❌ ajouter_bt n'existe pas";
    }

    if (ui->supprimer_bt) {
        connect(ui->supprimer_bt, &QPushButton::clicked,
                this, &MainWindow::on_supprimer_bt_clicked);
        qDebug() << "✅ Bouton supprimer_bt connecté";
    } else {
        qDebug() << "❌ supprimer_bt n'existe pas";
    }

    if (ui->tri_bt) {
        connect(ui->tri_bt, &QPushButton::clicked,
                this, &MainWindow::on_tri_bt_clicked);
        qDebug() << "✅ Bouton tri_bt connecté";
    } else {
        qDebug() << "❌ tri_bt n'existe pas";
    }

    if (ui->ajouter_bt_2) {
        connect(ui->ajouter_bt_2, &QPushButton::clicked,
                this, &MainWindow::on_ajouter_bt_2_clicked);
        qDebug() << "✅ Bouton ajouter_bt_2 (rechercher) connecté";
    } else {
        qDebug() << "❌ ajouter_bt_2 n'existe pas";
    }

    qDebug() << "Vérification des widgets...";

    if (ui->tableWidget_2) {
        qDebug() << "✅ Initialisation de tableWidget_2...";
        ui->tableWidget_2->setColumnCount(8);

        QStringList headers = {"ID", "Nom", "Localisation", "Capacité",
                               "Places libres", "Statut", "Type", "Tarification"};
        ui->tableWidget_2->setHorizontalHeaderLabels(headers);

        ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
        ui->tableWidget_2->setAlternatingRowColors(true);
        ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        qDebug() << "✅ tableWidget_2 initialisé avec succès";
    } else {
        qDebug() << "❌ ERREUR: tableWidget_2 n'existe pas!";
    }

    qDebug() << "Tentative de chargement du tableau...";
    loadParkingTable();
    qDebug() << "=== FIN DEMARRAGE ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =========================
// 🔹 Bouton Ajouter
// =========================
void MainWindow::on_ajouter_bt_clicked()
{
    qDebug() << "🟢 BOUTON AJOUTER CLIQUÉ";

    QString idParking = "P" + QString::number(QRandomGenerator::global()->bounded(1000, 9999));
    QString nom          = ui->nom_parking->text().trimmed();
    QString localisation = ui->loco_parking->text().trimmed();
    QString capaciteStr  = ui->capacite_parking->text().trimmed();
    QString placesStr    = ui->place_parking->text().trimmed();
    QString statut       = ui->statut_parking->text().trimmed();
    QString type         = ui->type_parking->text().trimmed();
    QString tarifStr     = ui->tarif_parking->text().trimmed();

    if (nom.isEmpty() || localisation.isEmpty() || capaciteStr.isEmpty() || tarifStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    bool ok;
    int capacite = capaciteStr.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "La capacité doit être un nombre !");
        return;
    }

    int placesLibres = placesStr.isEmpty() ? 0 : placesStr.toInt(&ok);
    if (!ok && !placesStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Les places libres doivent être un nombre !");
        return;
    }

    double tarif = tarifStr.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "La tarification doit être un nombre décimal !");
        return;
    }

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO NOUR.PARKING ("
                  "ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, "
                  "STATUT, TYPE, TARIFICATION"
                  ") VALUES ("
                  ":id, :nom, :localisation, :capacite, :placeslibres, "
                  ":statut, :type, :tarification"
                  ")");

    query.bindValue(":id",           idParking);
    query.bindValue(":nom",          nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite",     capacite);
    query.bindValue(":placeslibres", placesLibres);
    query.bindValue(":statut",       statut);
    query.bindValue(":type",         type);
    query.bindValue(":tarification", tarif);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Parking ajouté !\nID : " + idParking);
        ui->id_parking->clear();
        ui->nom_parking->clear();
        ui->loco_parking->clear();
        ui->capacite_parking->clear();
        ui->place_parking->clear();
        ui->statut_parking->clear();
        ui->type_parking->clear();
        ui->tarif_parking->clear();
        loadParkingTable();
    } else {
        QMessageBox::critical(this, "Erreur Oracle", query.lastError().text());
    }
}

// =========================
// 🔹 Clic sur ligne
// =========================
void MainWindow::on_tableWidget_2_itemClicked(QTableWidgetItem *item)
{
    if (!item) return;
    int row = item->row();
    ui->id_parking->setText(ui->tableWidget_2->item(row, 0)->text());
    ui->nom_parking->setText(ui->tableWidget_2->item(row, 1)->text());
    ui->loco_parking->setText(ui->tableWidget_2->item(row, 2)->text());
    ui->capacite_parking->setText(ui->tableWidget_2->item(row, 3)->text());
    ui->place_parking->setText(ui->tableWidget_2->item(row, 4)->text());
    ui->statut_parking->setText(ui->tableWidget_2->item(row, 5)->text());
    ui->type_parking->setText(ui->tableWidget_2->item(row, 6)->text());
    ui->tarif_parking->setText(ui->tableWidget_2->item(row, 7)->text());
    qDebug() << "Ligne sélectionnée - ID:" << ui->tableWidget_2->item(row, 0)->text();
}

// =========================
// 🔹 Bouton Modifier
// =========================
void MainWindow::on_modifier_bt_clicked()
{
    qDebug() << "🔵 BOUTON MODIFIER CLIQUÉ";

    QString idParking = ui->id_parking->text().trimmed();
    if (idParking.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un parking à modifier !");
        return;
    }

    QString nom          = ui->nom_parking->text().trimmed();
    QString localisation = ui->loco_parking->text().trimmed();
    QString capaciteStr  = ui->capacite_parking->text().trimmed();
    QString placesStr    = ui->place_parking->text().trimmed();
    QString statut       = ui->statut_parking->text().trimmed();
    QString type         = ui->type_parking->text().trimmed();
    QString tarifStr     = ui->tarif_parking->text().trimmed();

    if (nom.isEmpty() || localisation.isEmpty() || capaciteStr.isEmpty() || tarifStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires !");
        return;
    }

    bool ok;
    int capacite = capaciteStr.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "La capacité doit être un nombre !");
        return;
    }

    int placesLibres = placesStr.isEmpty() ? 0 : placesStr.toInt(&ok);
    if (!ok && !placesStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Les places libres doivent être un nombre !");
        return;
    }

    double tarif = tarifStr.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "La tarification doit être un nombre décimal !");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              QString("Voulez-vous vraiment modifier :\n\nID : %1\nNom : %2 ?").arg(idParking, nom),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) return;

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE NOUR.PARKING SET "
                  "NOM = :nom, LOCALISATION = :localisation, CAPACITE = :capacite, "
                  "PLACESLIBRES = :placeslibres, STATUT = :statut, TYPE = :type, "
                  "TARIFICATION = :tarification WHERE ID_PARKING = :id");

    query.bindValue(":id", idParking);
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placeslibres", placesLibres);
    query.bindValue(":statut", statut);
    query.bindValue(":type", type);
    query.bindValue(":tarification", tarif);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Parking modifié !");
        ui->id_parking->clear();
        ui->nom_parking->clear();
        ui->loco_parking->clear();
        ui->capacite_parking->clear();
        ui->place_parking->clear();
        ui->statut_parking->clear();
        ui->type_parking->clear();
        ui->tarif_parking->clear();
        loadParkingTable();
    } else {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}

// =========================
// 🔹 Bouton Supprimer
// =========================
void MainWindow::on_supprimer_bt_clicked()
{
    qDebug() << "🔴 BOUTON SUPPRIMER CLIQUÉ";

    int selectedRow = ui->tableWidget_2->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un parking !");
        return;
    }

    QString idParking = ui->tableWidget_2->item(selectedRow, 0)->text();
    QString nomParking = ui->tableWidget_2->item(selectedRow, 1)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
                                                              QString("Supprimer :\n\nID : %1\nNom : %2 ?").arg(idParking, nomParking),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) return;

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion échouée !");
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM NOUR.PARKING WHERE ID_PARKING = :id");
    query.bindValue(":id", idParking);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Parking supprimé !");
        loadParkingTable();
    } else {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}

// =========================
// 🔹 Bouton Trier
// =========================
void MainWindow::on_tri_bt_clicked()
{
    qDebug() << "🟡 BOUTON TRI CLIQUÉ";
    trierParCapacite();
}

void MainWindow::trierParCapacite()
{
    if (!ui->tableWidget_2) return;

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion échouée !");
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, "
                    "STATUT, TYPE, TARIFICATION FROM NOUR.PARKING ORDER BY CAPACITE DESC")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    ui->tableWidget_2->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(row, col, item);
        }
        ++row;
    }

    ui->tableWidget_2->resizeColumnsToContents();
    QMessageBox::information(this, "Tri effectué",
                             QString("Tableau trié par capacité\n%1 parkings").arg(row));
}

// =========================
// 🔹 Bouton Rechercher (ajouter_bt_2)
// =========================
void MainWindow::on_ajouter_bt_2_clicked()
{
    qDebug() << "🔍 BOUTON RECHERCHER CLIQUÉ";

    QString localisation = ui->lineEdit->text().trimmed();

    if (localisation.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez entrer une localisation à rechercher !");
        return;
    }

    rechercherParLocalisation(localisation);
}

void MainWindow::rechercherParLocalisation(QString localisation)
{
    if (!ui->tableWidget_2) return;

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, "
                  "STATUT, TYPE, TARIFICATION FROM NOUR.PARKING "
                  "WHERE LOWER(LOCALISATION) LIKE LOWER(:localisation) "
                  "ORDER BY ID_PARKING");

    query.bindValue(":localisation", "%" + localisation + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    ui->tableWidget_2->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(row, col, item);
        }
        ++row;
    }

    ui->tableWidget_2->resizeColumnsToContents();

    if (row == 0) {
        QMessageBox::information(this, "Recherche",
                                 QString("Aucun parking trouvé pour la localisation : %1").arg(localisation));
        // Recharger tous les parkings si aucun résultat
        loadParkingTable();
    } else {
        QMessageBox::information(this, "Recherche effectuée",
                                 QString("%1 parking(s) trouvé(s)").arg(row));
    }

    qDebug() << "✅ Recherche terminée:" << row << "résultat(s)";
}

// =========================
// 🔹 Chargement tableau
// =========================
void MainWindow::loadParkingTable()
{
    if (!ui->tableWidget_2) return;

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) return;

    QSqlQuery query(db);
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, "
                    "STATUT, TYPE, TARIFICATION FROM NOUR.PARKING ORDER BY ID_PARKING")) {
        return;
    }

    ui->tableWidget_2->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_2->setItem(row, col, item);
        }
        ++row;
    }

    ui->tableWidget_2->resizeColumnsToContents();
    qDebug() << "✅ Tableau chargé:" << row << "lignes";
}
