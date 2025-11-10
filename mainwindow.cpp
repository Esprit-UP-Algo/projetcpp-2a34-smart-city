#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Chargement initial du tableau
    loadParkingTable();
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
    // Génération d’un ID unique
    QString idParking = "P" + QString::number(QRandomGenerator::global()->bounded(1000, 9999));

    // Récupération des champs de ton interface
    QString nom          = ui->nom_parking->text().trimmed();
    QString localisation = ui->loco_parking->text().trimmed();
    QString capaciteStr  = ui->capacite_parking->text().trimmed();
    QString placesStr    = ui->place_parking->text().trimmed();
    QString statut       = ui->statut_parking->text().trimmed();
    QString type         = ui->type_parking->text().trimmed();
    QString tarifStr     = ui->tarif_parking->text().trimmed();

    // Validation
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

    // Connexion à la base
    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();
    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    // Insertion SQL
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

        // Vider les champs après ajout
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
// 🔹 Chargement du tableau
// =========================
void MainWindow::loadParkingTable()
{
    qDebug() << "Chargement du tableau...";

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen() && !c.createConnect()) {
        qDebug() << "❌ Connexion à la base impossible.";
        return;
    }

    QSqlQuery query("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, "
                    "STATUT, TYPE, TARIFICATION "
                    "FROM NOUR.PARKING ORDER BY ID_PARKING", db);

    if (!query.isActive()) {
        qDebug() << "❌ Erreur SQL :" << query.lastError().text();
        return;
    }

    // Nettoyage du tableau avant rechargement
    ui->tableWidget_4->clearContents();
    ui->tableWidget_4->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget_4->insertRow(row);

        ui->tableWidget_4->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // ID
        ui->tableWidget_4->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Nom
        ui->tableWidget_4->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // Localisation
        ui->tableWidget_4->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // Capacité
        ui->tableWidget_4->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // Places libres
        ui->tableWidget_4->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // Statut
        ui->tableWidget_4->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // Type
        ui->tableWidget_4->setItem(row, 7, new QTableWidgetItem(query.value(7).toString())); // Tarification

        for (int col = 0; col < 8; ++col) {
            ui->tableWidget_4->item(row, col)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }

        ++row;
    }

    // Titres des colonnes
    QStringList headers = {"ID", "Nom", "Localisation", "Capacité",
                           "Places libres", "Statut", "Type", "Tarification"};
    ui->tableWidget_4->setHorizontalHeaderLabels(headers);
    ui->tableWidget_4->resizeColumnsToContents();

    qDebug() << "✅ Tableau chargé avec" << row << "enregistrements.";
}

