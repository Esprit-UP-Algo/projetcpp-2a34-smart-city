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

    qDebug() << "=== DEMARRAGE APPLICATION ===";
    qDebug() << "Vérification des widgets...";

    // 🔹 Vérification du tableau principal
    qDebug() << "tableWidget_2 existe ?" << (ui->tableWidget_2 != nullptr);

    // 🔹 Initialisation du tableau AVANT le chargement
    if (ui->tableWidget_2) {
        qDebug() << "✅ Initialisation de tableWidget_2...";
        ui->tableWidget_2->setColumnCount(8);

        QStringList headers = {"ID", "Nom", "Localisation", "Capacité",
                               "Places libres", "Statut", "Type", "Tarification"};
        ui->tableWidget_2->setHorizontalHeaderLabels(headers);

        // Configuration visuelle
        ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
        ui->tableWidget_2->setAlternatingRowColors(true);
        ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        qDebug() << "✅ tableWidget_2 initialisé avec succès";
    } else {
        qDebug() << "❌ ERREUR: tableWidget_2 n'existe pas!";
    }

    qDebug() << "Tentative de chargement du tableau...";

    // Chargement initial du tableau
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
    // Génération d'un ID unique
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
// 🔹 Clic sur une ligne du tableau (pour remplir le formulaire)
// =========================
void MainWindow::on_tableWidget_2_itemClicked(QTableWidgetItem *item)
{
    if (!item) return;

    int row = item->row();

    // Remplir les champs avec les données de la ligne sélectionnée
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
    qDebug() << "=== MODIFICATION ===";

    // Récupération de l'ID (champ en lecture seule normalement)
    QString idParking = ui->id_parking->text().trimmed();

    if (idParking.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un parking à modifier !\n\n"
                                                "Cliquez sur une ligne du tableau pour charger les données.");
        return;
    }

    // Récupération des champs modifiés
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

    // Confirmation
    QMessageBox::StandardButton reply;
    QString message = QString("Voulez-vous vraiment modifier le parking :\n\n"
                              "ID : %1\n"
                              "Nom : %2 ?").arg(idParking, nom);

    reply = QMessageBox::question(this, "Confirmation", message,
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        qDebug() << "Modification annulée par l'utilisateur";
        return;
    }

    // Connexion à la base
    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    // Mise à jour SQL
    QSqlQuery query(db);
    query.prepare("UPDATE NOUR.PARKING SET "
                  "NOM = :nom, "
                  "LOCALISATION = :localisation, "
                  "CAPACITE = :capacite, "
                  "PLACESLIBRES = :placeslibres, "
                  "STATUT = :statut, "
                  "TYPE = :type, "
                  "TARIFICATION = :tarification "
                  "WHERE ID_PARKING = :id");

    query.bindValue(":id",           idParking);
    query.bindValue(":nom",          nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":capacite",     capacite);
    query.bindValue(":placeslibres", placesLibres);
    query.bindValue(":statut",       statut);
    query.bindValue(":type",         type);
    query.bindValue(":tarification", tarif);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Parking modifié avec succès !");
        qDebug() << "✅ Parking modifié - ID:" << idParking;

        // Vider les champs après modification
        ui->id_parking->clear();
        ui->nom_parking->clear();
        ui->loco_parking->clear();
        ui->capacite_parking->clear();
        ui->place_parking->clear();
        ui->statut_parking->clear();
        ui->type_parking->clear();
        ui->tarif_parking->clear();

        // Recharger le tableau
        loadParkingTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification :\n" + query.lastError().text());
        qDebug() << "❌ Erreur modification:" << query.lastError().text();
    }
}

// =========================
// 🔹 Bouton Supprimer
// =========================
void MainWindow::on_supprimer_bt_clicked()
{
    qDebug() << "=== SUPPRESSION ===";

    // Vérifier qu'une ligne est sélectionnée
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget_2->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un parking à supprimer !");
        return;
    }

    // Récupérer l'ID du parking sélectionné (colonne 0)
    int selectedRow = ui->tableWidget_2->currentRow();

    if (selectedRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une ligne !");
        return;
    }

    QString idParking = ui->tableWidget_2->item(selectedRow, 0)->text();
    QString nomParking = ui->tableWidget_2->item(selectedRow, 1)->text();

    qDebug() << "Parking sélectionné - ID:" << idParking << "Nom:" << nomParking;

    // Confirmation de suppression
    QMessageBox::StandardButton reply;
    QString message = QString("Voulez-vous vraiment supprimer le parking :\n\n"
                              "ID : %1\n"
                              "Nom : %2 ?").arg(idParking, nomParking);

    reply = QMessageBox::question(this, "Confirmation", message,
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        qDebug() << "Suppression annulée par l'utilisateur";
        return;
    }

    // Connexion à la base
    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    // Suppression SQL
    QSqlQuery query(db);
    query.prepare("DELETE FROM NOUR.PARKING WHERE ID_PARKING = :id");
    query.bindValue(":id", idParking);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Parking supprimé avec succès !");
        qDebug() << "✅ Parking supprimé - ID:" << idParking;

        // Recharger le tableau
        loadParkingTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression :\n" + query.lastError().text());
        qDebug() << "❌ Erreur suppression:" << query.lastError().text();
    }
}

// =========================
// 🔹 Chargement du tableau
// =========================
void MainWindow::loadParkingTable()
{
    qDebug() << "=== DEBUT loadParkingTable() ===";

    // 🔹 Vérification que le tableau existe
    if (!ui->tableWidget_2) {
        qDebug() << "❌ ERREUR CRITIQUE : tableWidget_2 n'existe pas !";
        QMessageBox::critical(this, "Erreur", "Le tableau tableWidget_2 n'existe pas dans l'interface !");
        return;
    }

    qDebug() << "✅ tableWidget_2 existe";
    qDebug() << "   Colonnes configurées :" << ui->tableWidget_2->columnCount();
    qDebug() << "   Lignes actuelles :" << ui->tableWidget_2->rowCount();

    // Connexion à la base
    qDebug() << "Tentative de connexion à la base...";
    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen()) {
        qDebug() << "Base fermée, tentative de connexion...";
        if (!c.createConnect()) {
            qDebug() << "❌ Connexion à la base impossible.";
            QMessageBox::warning(this, "Erreur", "Impossible de se connecter à la base de données.");
            return;
        }
    }

    qDebug() << "✅ Connexion à la base OK";
    qDebug() << "   Driver:" << db.driverName();
    qDebug() << "   Database:" << db.databaseName();

    // Préparation de la requête
    qDebug() << "Préparation de la requête SQL...";
    QSqlQuery query(db);

    QString sql = "SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, "
                  "STATUT, TYPE, TARIFICATION "
                  "FROM NOUR.PARKING ORDER BY ID_PARKING";

    qDebug() << "SQL:" << sql;

    query.prepare(sql);

    qDebug() << "Exécution de la requête...";
    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL :" << query.lastError().text();
        qDebug() << "   Type erreur:" << query.lastError().type();
        qDebug() << "   Code erreur:" << query.lastError().nativeErrorCode();
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    qDebug() << "✅ Requête SQL exécutée avec succès";

    // Nettoyage du tableau avant rechargement
    qDebug() << "Nettoyage du tableau...";
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->clearContents();
    qDebug() << "✅ Tableau nettoyé";

    int row = 0;
    qDebug() << "Parcours des résultats...";

    while (query.next()) {
        qDebug() << "  -> Insertion ligne" << row;

        try {
            ui->tableWidget_2->insertRow(row);

            // Insertion des données avec vérification
            for (int col = 0; col < 8; ++col) {
                QString value = query.value(col).toString();
                qDebug() << "     Col" << col << "=" << value;

                QTableWidgetItem* item = new QTableWidgetItem(value);

                if (!item) {
                    qDebug() << "❌ ERREUR: Impossible de créer QTableWidgetItem!";
                    continue;
                }

                // Désactiver l'édition
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

                ui->tableWidget_2->setItem(row, col, item);
            }

            qDebug() << "  ✅ Ligne" << row << "insérée";
            ++row;

        } catch (const std::exception& e) {
            qDebug() << "❌ EXCEPTION lors de l'insertion:" << e.what();
        } catch (...) {
            qDebug() << "❌ EXCEPTION INCONNUE lors de l'insertion";
        }
    }

    qDebug() << "Ajustement des colonnes...";
    // Ajustement des colonnes
    ui->tableWidget_2->resizeColumnsToContents();

    qDebug() << "✅ Tableau chargé avec" << row << "enregistrements.";
    qDebug() << "=== FIN loadParkingTable() ===";
}

