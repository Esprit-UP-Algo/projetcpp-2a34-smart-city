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
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDate>
#include <QPageLayout>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QDir>  // Ajouter cette ligne avec les autres #include au début du fichier
#include <QInputDialog>  // Pour la sélection du parking
#include <QTime>

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

    if (ui->pdf_bt) {
        connect(ui->pdf_bt, &QPushButton::clicked,
                this, &MainWindow::on_pdf_bt_clicked);
        qDebug() << "✅ Bouton pdf_bt connecté";
    } else {
        qDebug() << "❌ pdf_bt n'existe pas";
    }

    qDebug() << "Vérification des widgets...";
    if (ui->map_bt) {
        connect(ui->map_bt, &QPushButton::clicked,
                this, &MainWindow::on_map_bt_clicked);
        qDebug() << "Bouton MAP connecté";
    } else {
        qDebug() << "ERREUR: map_bt n'existe pas dans l'interface !";
    }
    if (ui->statistiques_bt) {
        connect(ui->statistiques_bt, &QPushButton::clicked,
                this, &MainWindow::on_statistiques_bt_clicked);
        qDebug() << "✅ Bouton statistiques_bt connecté";
    }

}   // ← ACCOLADE AJOUTÉE ICI : FIN DU CONSTRUCTEUR !!!

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
        loadParkingTable();
    } else {
        QMessageBox::information(this, "Recherche effectuée",
                                 QString("%1 parking(s) trouvé(s)").arg(row));
    }

    qDebug() << "✅ Recherche terminée:" << row << "résultat(s)";
}

// =========================
// 🔹 Bouton PDF - GRILLE VISUELLE
// =========================
void MainWindow::on_pdf_bt_clicked()
{
    qDebug() << "📄 BOUTON PDF CLIQUÉ - Génération ticket";

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    // Demander à l'utilisateur de sélectionner un parking
    QSqlQuery query(db);
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, TARIFICATION FROM NOUR.PARKING ORDER BY NOM")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    // Créer une liste de parkings pour la sélection
    QStringList parkingList;
    QMap<QString, QStringList> parkingData;

    while (query.next()) {
        QString id = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString localisation = query.value(2).toString();
        QString tarif = query.value(3).toString();

        QString displayText = QString("%1 - %2 (%3)").arg(id, nom, localisation);
        parkingList << displayText;

        QStringList data;
        data << id << nom << localisation << tarif;
        parkingData[displayText] = data;
    }

    if (parkingList.isEmpty()) {
        QMessageBox::information(this, "Information", "Aucun parking disponible.");
        return;
    }

    // Dialogue de sélection
    bool ok;
    QString selectedParking = QInputDialog::getItem(this, "Sélectionner un Parking",
                                                    "Choisissez un parking pour générer le ticket:",
                                                    parkingList, 0, false, &ok);

    if (!ok || selectedParking.isEmpty()) {
        return;
    }

    // Récupérer les données du parking sélectionné
    QStringList data = parkingData[selectedParking];
    QString parkingId = data[0];
    QString parkingNom = data[1];
    QString parkingLoc = data[2];
    QString parkingTarif = data[3];

    // Générer le numéro de ticket unique
    QString ticketNumber = QString("TK-%1-%2")
                               .arg(QDate::currentDate().toString("yyyyMMdd"))
                               .arg(QRandomGenerator::global()->bounded(1000, 9999));

    // Date et heure actuelles
    QString dateEntree = QDate::currentDate().toString("dd/MM/yyyy");
    QString heureEntree = QTime::currentTime().toString("HH:mm");

    // Construire le HTML du ticket
    QString htmlContent = QString(
                              "<!DOCTYPE html>"
                              "<html>"
                              "<head>"
                              "<meta charset='UTF-8'>"
                              "<style>"
                              "body { font-family: 'Courier New', monospace; margin: 0; padding: 20px; }"
                              ".ticket { width: 300px; margin: 0 auto; border: 3px dashed #333; "
                              "          padding: 20px; background: white; box-shadow: 0 0 10px rgba(0,0,0,0.1); }"
                              ".header { text-align: center; border-bottom: 2px solid #333; padding-bottom: 15px; margin-bottom: 15px; }"
                              ".header h1 { margin: 0; font-size: 24px; color: #2c5f2d; }"
                              ".header h2 { margin: 5px 0 0 0; font-size: 14px; color: #666; }"
                              ".ticket-number { text-align: center; font-size: 18px; font-weight: bold; "
                              "                 background: #f0f0f0; padding: 10px; margin: 15px 0; "
                              "                 border: 2px solid #333; }"
                              ".info-section { margin: 15px 0; }"
                              ".info-row { display: flex; justify-content: space-between; margin: 8px 0; "
                              "            font-size: 14px; }"
                              ".info-label { font-weight: bold; }"
                              ".info-value { text-align: right; }"
                              ".separator { border-top: 1px dashed #999; margin: 15px 0; }"
                              ".tarif-section { background: #fff9e6; padding: 15px; border: 2px solid #ffcc00; "
                              "                 border-radius: 5px; margin: 15px 0; }"
                              ".tarif-title { font-weight: bold; text-align: center; margin-bottom: 10px; "
                              "               font-size: 16px; color: #cc8800; }"
                              ".tarif-info { font-size: 20px; text-align: center; font-weight: bold; color: #cc8800; }"
                              ".fill-section { margin: 20px 0; padding: 15px; background: #f9f9f9; border: 2px solid #ddd; }"
                              ".fill-title { font-weight: bold; margin-bottom: 10px; text-align: center; }"
                              ".fill-line { margin: 15px 0; border-bottom: 1px solid #333; padding-bottom: 5px; }"
                              ".fill-label { font-size: 12px; color: #666; }"
                              ".footer { text-align: center; font-size: 11px; color: #666; "
                              "          margin-top: 20px; border-top: 2px solid #333; padding-top: 10px; }"
                              ".barcode { text-align: center; font-family: 'Libre Barcode 39', monospace; "
                              "           font-size: 40px; letter-spacing: 2px; margin: 10px 0; }"
                              "</style>"
                              "</head>"
                              "<body>"
                              "<div class='ticket'>"

                              "<!-- En-tête -->"
                              "<div class='header'>"
                              "<h1>🅿️ PARKING</h1>"
                              "<h2>%1</h2>"
                              "<h2>%2</h2>"
                              "</div>"

                              "<!-- Numéro de ticket -->"
                              "<div class='ticket-number'>TICKET N° %3</div>"

                              "<!-- Code-barres stylisé -->"
                              "<div class='barcode'>*%3*</div>"

                              "<!-- Informations d'entrée -->"
                              "<div class='info-section'>"
                              "<div class='info-row'>"
                              "<span class='info-label'>DATE D'ENTRÉE:</span>"
                              "<span class='info-value'>%4</span>"
                              "</div>"
                              "<div class='info-row'>"
                              "<span class='info-label'>HEURE D'ENTRÉE:</span>"
                              "<span class='info-value'>%5</span>"
                              "</div>"
                              "<div class='info-row'>"
                              "<span class='info-label'>ID PARKING:</span>"
                              "<span class='info-value'>%6</span>"
                              "</div>"
                              "</div>"

                              "<div class='separator'></div>"

                              "<!-- Tarification -->"
                              "<div class='tarif-section'>"
                              "<div class='tarif-title'>💳 TARIFICATION</div>"
                              "<div class='tarif-info'>%7 DT / heure</div>"
                              "</div>"

                              "<div class='separator'></div>"

                              "<!-- Section à remplir -->"
                              "<div class='fill-section'>"
                              "<div class='fill-title'>📝 À REMPLIR À LA SORTIE</div>"

                              "<div class='fill-line'>"
                              "<div class='fill-label'>HEURE DE SORTIE:</div>"
                              "</div>"

                              "<div class='fill-line'>"
                              "<div class='fill-label'>DURÉE TOTALE:</div>"
                              "</div>"

                              "<div class='fill-line'>"
                              "<div class='fill-label'>MONTANT À PAYER:</div>"
                              "</div>"

                              "<div class='fill-line'>"
                              "<div class='fill-label'>SIGNATURE:</div>"
                              "</div>"
                              "</div>"

                              "<!-- Pied de page -->"
                              "<div class='footer'>"
                              "Merci de votre visite<br>"
                              "Conservez ce ticket<br>"
                              "Support: parking@contact.tn"
                              "</div>"

                              "</div>"
                              "</body>"
                              "</html>"
                              ).arg(parkingNom)           // %1 - Nom du parking
                              .arg(parkingLoc)           // %2 - Localisation
                              .arg(ticketNumber)         // %3 - Numéro de ticket (utilisé 2 fois)
                              .arg(dateEntree)           // %4 - Date
                              .arg(heureEntree)          // %5 - Heure
                              .arg(parkingId)            // %6 - ID Parking
                              .arg(parkingTarif);        // %7 - Tarif

    // Sauvegarder le PDF
    QString defaultFileName = QString("Ticket_Parking_%1.pdf").arg(ticketNumber);
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer le ticket PDF",
                                                    defaultFileName,
                                                    "PDF Files (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A5));
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    QTextDocument document;
    document.setHtml(htmlContent);
    document.print(&printer);

    QMessageBox::information(this, "Succès",
                             QString("Ticket généré avec succès !\n\nTicket N°: %1\nParking: %2")
                                 .arg(ticketNumber, parkingNom));

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));

    qDebug() << "✅ Ticket PDF généré:" << fileName;
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
    void MainWindow::on_map_bt_clicked()
{
    qDebug() << "🗺️ BOUTON MAP CLIQUÉ";

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES FROM NOUR.PARKING ORDER BY ID_PARKING")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("🗺️ Carte des Parkings");
    mapDialog->resize(1200, 800);

    QVBoxLayout *mainLayout = new QVBoxLayout(mapDialog);

    QLabel *titleLabel = new QLabel("🗺️ Carte des Parkings - Occupation en temps réel");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #2c5f2d; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    // 🗺️ CHARGER L'IMAGE DE LA CARTE DE TUNISIE
    // Mettez le chemin vers votre image de carte (à adapter selon votre projet)
    QString mapImagePath = ":/images/tunisia_map.png"; // Si l'image est dans les ressources
    // OU
    // QString mapImagePath = QCoreApplication::applicationDirPath() + "/tunisia_map.png"; // Si dans le dossier exe

    QPixmap mapPixmap(mapImagePath);

    if (!mapPixmap.isNull()) {
        // Si l'image est chargée avec succès
        QGraphicsPixmapItem *mapBackground = scene->addPixmap(mapPixmap.scaled(1200, 700, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        mapBackground->setPos(0, 0);
        mapBackground->setZValue(0); // Mettre l'image en arrière-plan
    } else {
        // Si l'image n'est pas trouvée, utiliser un fond de secours
        qDebug() << "⚠️ Image de carte non trouvée, utilisation du fond dégradé";
        QLinearGradient gradient(0, 0, 0, 700);
        gradient.setColorAt(0, QColor(220, 240, 255));
        gradient.setColorAt(1, QColor(180, 220, 240));
        view->setBackgroundBrush(QBrush(gradient));
    }

    // Positions approximatives basées sur les coordonnées GPS de la Tunisie
    // Ajustez ces valeurs selon votre image de carte
    QMap<QString, QPoint> positions;
    positions["tunis"] = QPoint(600, 250);
    positions["ariana"] = QPoint(650, 220);
    positions["manar"] = QPoint(620, 240);
    positions["manouba"] = QPoint(550, 250);
    positions["ben arous"] = QPoint(610, 290);
    positions["nabeul"] = QPoint(700, 260);
    positions["sousse"] = QPoint(600, 380);
    positions["sfax"] = QPoint(580, 500);
    positions["kairouan"] = QPoint(550, 400);

    int count = 0;

    while (query.next()) {
        QString id = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString localisation = query.value(2).toString().toLower();
        int capacite = query.value(3).toInt();
        int placesLibres = query.value(4).toInt();

        int placesOccupees = capacite - placesLibres;
        double tauxOccupation = capacite > 0 ? (placesOccupees * 100.0 / capacite) : 0.0;

        QColor couleur;
        if (tauxOccupation >= 80) {
            couleur = QColor(244, 67, 54); // Rouge
        } else if (tauxOccupation >= 50) {
            couleur = QColor(255, 152, 0); // Orange
        } else {
            couleur = QColor(76, 175, 80); // Vert
        }

        // Position sur la carte avec un petit décalage aléatoire
        QPoint pos = positions.value(localisation, QPoint(600, 300 + (count * 80)));

        int offsetX = (QRandomGenerator::global()->bounded(40)) - 20;
        int offsetY = (QRandomGenerator::global()->bounded(40)) - 20;
        int x = pos.x() + offsetX;
        int y = pos.y() + offsetY;

        // Dessiner le cercle du parking
        QGraphicsEllipseItem *circle = scene->addEllipse(x - 40, y - 40, 80, 80);
        circle->setBrush(QBrush(couleur));
        circle->setPen(QPen(Qt::black, 3));
        circle->setZValue(10); // Mettre les cercles au-dessus de la carte

        // ID du parking
        QGraphicsTextItem *idText = scene->addText(id);
        idText->setPos(x - 25, y - 30);
        idText->setDefaultTextColor(Qt::white);
        QFont idFont = idText->font();
        idFont.setPointSize(12);
        idFont.setBold(true);
        idText->setFont(idFont);
        idText->setZValue(11);

        // Nom du parking
        QGraphicsTextItem *nomText = scene->addText(nom);
        nomText->setPos(x - 35, y - 10);
        nomText->setDefaultTextColor(Qt::white);
        QFont nomFont = nomText->font();
        nomFont.setPointSize(9);
        nomFont.setBold(true);
        nomText->setFont(nomFont);
        nomText->setZValue(11);

        // Taux d'occupation
        QGraphicsTextItem *tauxText = scene->addText(QString::number(tauxOccupation, 'f', 0) + "%");
        tauxText->setPos(x - 20, y + 10);
        tauxText->setDefaultTextColor(Qt::white);
        QFont tauxFont = tauxText->font();
        tauxFont.setPointSize(14);
        tauxFont.setBold(true);
        tauxText->setFont(tauxFont);
        tauxText->setZValue(11);

        // Tooltip avec informations détaillées
        QString tooltip = QString(
                              "<b>%1 - %2</b><br>"
                              "Localisation: %3<br>"
                              "Capacité: %4 places<br>"
                              "Occupées: %5<br>"
                              "Libres: %6<br>"
                              "Taux: %7%"
                              ).arg(id).arg(nom).arg(localisation).arg(capacite)
                              .arg(placesOccupees).arg(placesLibres)
                              .arg(QString::number(tauxOccupation, 'f', 1));

        circle->setToolTip(tooltip);
        count++;
    }

    mainLayout->addWidget(view);

    // Légende
    QHBoxLayout *legendLayout = new QHBoxLayout();

    QLabel *legendTitle = new QLabel("<b>Légende:</b>");
    legendTitle->setStyleSheet("font-size: 14px; margin-right: 20px;");
    legendLayout->addWidget(legendTitle);

    // Vert
    QLabel *greenCircle = new QLabel("●");
    greenCircle->setStyleSheet("color: #4caf50; font-size: 30px;");
    QLabel *greenLabel = new QLabel("Disponible (< 50%)");
    greenLabel->setStyleSheet("font-size: 12px;");
    legendLayout->addWidget(greenCircle);
    legendLayout->addWidget(greenLabel);
    legendLayout->addSpacing(20);

    // Orange
    QLabel *orangeCircle = new QLabel("●");
    orangeCircle->setStyleSheet("color: #ff9800; font-size: 30px;");
    QLabel *orangeLabel = new QLabel("Modéré (50-80%)");
    orangeLabel->setStyleSheet("font-size: 12px;");
    legendLayout->addWidget(orangeCircle);
    legendLayout->addWidget(orangeLabel);
    legendLayout->addSpacing(20);

    // Rouge
    QLabel *redCircle = new QLabel("●");
    redCircle->setStyleSheet("color: #f44336; font-size: 30px;");
    QLabel *redLabel = new QLabel("Saturé (> 80%)");
    redLabel->setStyleSheet("font-size: 12px;");
    legendLayout->addWidget(redCircle);
    legendLayout->addWidget(redLabel);

    legendLayout->addStretch();

    QWidget *legendWidget = new QWidget();
    legendWidget->setLayout(legendLayout);
    legendWidget->setStyleSheet("background-color: #f5f5f5; padding: 10px; border-radius: 5px;");
    mainLayout->addWidget(legendWidget);

    mapDialog->exec();

    qDebug() << "✅ Carte affichée avec" << count << "parkings";
}
void MainWindow::on_statistiques_bt_clicked()
{
    qDebug() << "📊 BOUTON STATISTIQUES CLIQUÉ";

    Connection& c = Connection::createInstance();
    QSqlDatabase db = c.getDatabase();

    if (!db.isOpen() && !c.createConnect()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
        return;
    }

    // D'abord, voir TOUTES les valeurs de TYPE dans la base
    QSqlQuery debugQuery(db);
    debugQuery.exec("SELECT DISTINCT TYPE FROM NOUR.PARKING");
    qDebug() << "=== Types trouvés dans la base ===";
    while (debugQuery.next()) {
        qDebug() << "Type:" << debugQuery.value(0).toString();
    }

    // Compter TOUS les parkings par type (sans filtre)
    QSqlQuery query(db);
    if (!query.exec("SELECT TYPE, COUNT(*) as nb FROM NOUR.PARKING GROUP BY TYPE")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    int nbPublic = 0;
    int nbPrive = 0;

    while (query.next()) {
        QString type = query.value(0).toString().toLower().trimmed();
        int count = query.value(1).toInt();

        qDebug() << "Type trouvé:" << type << "- Nombre:" << count;

        // Accepter plusieurs variantes
        if (type == "public" || type == "publique") {
            nbPublic = count;
        } else if (type == "prive" || type == "privé" || type == "private") {
            nbPrive = count;
        }
    }

    qDebug() << "✅ Résultat final: Public=" << nbPublic << ", Privé=" << nbPrive;

    // Créer la fenêtre
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("📊 Statistiques");
    statsDialog->resize(600, 500);

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);

    // Titre
    QLabel *titleLabel = new QLabel("📊 Nombre de Parkings");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c5f2d; padding: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Scène graphique
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(QBrush(Qt::white));

    int margin = 50;
    int graphWidth = 500;
    int graphHeight = 300;
    int barWidth = 150;

    // Axes
    scene->addLine(margin, margin + graphHeight, margin + graphWidth, margin + graphHeight, QPen(Qt::black, 3));
    scene->addLine(margin, margin, margin, margin + graphHeight, QPen(Qt::black, 3));

    // Valeur max pour l'échelle
    int maxValue = qMax(nbPublic, nbPrive);
    if (maxValue == 0) maxValue = 10;

    // Fonction pour dessiner une barre
    auto drawBar = [&](int x, int value, QColor color, QString label) {
        double ratio = (double)value / maxValue;
        int barHeight = ratio * (graphHeight - 20);
        int y = margin + graphHeight - barHeight;

        // Barre
        QGraphicsRectItem *bar = scene->addRect(x, y, barWidth, barHeight);
        bar->setBrush(QBrush(color));
        bar->setPen(QPen(Qt::black, 3));

        // Valeur au-dessus
        QGraphicsTextItem *valueText = scene->addText(QString::number(value));
        QFont valueFont = valueText->font();
        valueFont.setPointSize(20);
        valueFont.setBold(true);
        valueText->setFont(valueFont);
        valueText->setDefaultTextColor(color.darker(150));
        valueText->setPos(x + 55, y - 35);

        // Label en bas
        QGraphicsTextItem *labelText = scene->addText(label);
        QFont labelFont = labelText->font();
        labelFont.setPointSize(14);
        labelFont.setBold(true);
        labelText->setFont(labelFont);
        labelText->setPos(x + 30, margin + graphHeight + 10);
    };

    // Dessiner les 2 barres
    drawBar(margin + 50, nbPublic, QColor(76, 175, 80), "Public");
    drawBar(margin + 280, nbPrive, QColor(33, 150, 243), "Privé");

    mainLayout->addWidget(view);

    statsDialog->exec();

    qDebug() << "✅ Stats affichées: Public=" << nbPublic << ", Privé=" << nbPrive;
}
