#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Afficher toutes les alertes au démarrage
    afficherAlertes();


    // Configurer le tableWidget
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Connecter le double-clic sur une ligne pour remplir les champs
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, [this]() {
        int row = ui->tableWidget->currentRow();
        if (row >= 0) {
            ui->lineEdit_id->setText(ui->tableWidget->item(row, 0)->text());
            ui->lineEdit_type->setText(ui->tableWidget->item(row, 1)->text());
            ui->lineEdit_client->setText(ui->tableWidget->item(row, 2)->text());
            ui->lineEdit_statut->setText(ui->tableWidget->item(row, 3)->text());
            ui->lineEdit_description->setText(ui->tableWidget->item(row, 4)->text());
            ui->lineEdit_gravite->setText(ui->tableWidget->item(row, 5)->text());
            // Pour la date, tu devras l'adapter selon ton widget (dateEdit ou lineEdit)
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================== AFFICHER LES ALERTES ====================
void MainWindow::afficherAlertes()
{
    QSqlQueryModel* model = alerteTmp.afficher();

    // Configurer le tableau
    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount());

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Type" << "ID Client" << "Statut" << "Description" << "Gravité" << "Date";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Remplir le tableau
    for (int row = 0; row < model->rowCount(); row++) {
        for (int col = 0; col < model->columnCount(); col++) {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget->resizeColumnsToContents();
}

// ==================== VIDER LES CHAMPS ====================
void MainWindow::clearFields()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_type->clear();
    ui->lineEdit_client->clear();
    ui->lineEdit_statut->clear();
    ui->lineEdit_description->clear();
    ui->lineEdit_gravite->clear();
    // Si tu as un dateEdit, fais : ui->dateEdit_date->setDate(QDate::currentDate());
}

// ==================== VALIDER LES CHAMPS ====================
bool MainWindow::validateFields()
{
    if (ui->lineEdit_type->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le type est obligatoire !");
        return false;
    }

    if (ui->lineEdit_client->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le client est obligatoire !");
        return false;
    }

    if (ui->lineEdit_statut->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le statut est obligatoire !");
        return false;
    }

    if (ui->lineEdit_gravite->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "La gravité est obligatoire !");
        return false;
    }

    // Vérifier que la gravité est valide
    QStringList gravitesValides = {"Faible", "Moyenne", "Élevée", "Critique"};
    if (!gravitesValides.contains(ui->lineEdit_gravite->text())) {
        QMessageBox::warning(this, "Erreur", "Gravité invalide ! Choisir : Faible, Moyenne, Élevée, ou Critique");
        return false;
    }

    return true;
}

// ==================== BOUTON AJOUTER ====================
void MainWindow::on_pushButton_3_clicked()
{
    // Valider les champs
    if (!validateFields()) {
        return;
    }

    // Créer un objet Alerte avec les données saisies
    Alerte a;
    a.setType(ui->lineEdit_type->text());
    a.setIdClient(ui->lineEdit_client->text().toInt());
    a.setStatut(ui->lineEdit_statut->text());
    a.setDescription(ui->lineEdit_description->text());
    a.setGravite(ui->lineEdit_gravite->text());
    a.setDate(QDate::currentDate()); // Ou utilise ui->dateEdit_date->date() si tu as un dateEdit

    // Ajouter à la base de données
    if (a.ajouter()) {
        QMessageBox::information(this, "Succès", "Alerte ajoutée avec succès !");
        afficherAlertes();  // Rafraîchir l'affichage
        clearFields();       // Vider les champs
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout de l'alerte !");
    }
}

// ==================== BOUTON MODIFIER ====================
void MainWindow::on_pushButton_5_clicked()
{
    // Vérifier qu'un ID est renseigné
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une alerte à modifier !");
        return;
    }

    // Valider les champs
    if (!validateFields()) {
        return;
    }

    // Créer un objet Alerte avec les données saisies
    Alerte a;
    a.setId(ui->lineEdit_id->text().toInt());
    a.setType(ui->lineEdit_type->text());
    a.setIdClient(ui->lineEdit_client->text().toInt());
    a.setStatut(ui->lineEdit_statut->text());
    a.setDescription(ui->lineEdit_description->text());
    a.setGravite(ui->lineEdit_gravite->text());
    a.setDate(QDate::currentDate());

    // Modifier dans la base de données
    if (a.modifier()) {
        QMessageBox::information(this, "Succès", "Alerte modifiée avec succès !");
        afficherAlertes();
        clearFields();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification !");
    }
}

// ==================== BOUTON ANNULER ====================
void MainWindow::on_pushButton_4_clicked()
{
    clearFields();
}

// ==================== BOUTON SUPPRIMER ====================
void MainWindow::on_pushButton_supprimer_clicked()
{
    int row = ui->tableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une alerte à supprimer !");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cette alerte ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Alerte a;
        if (a.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Alerte supprimée avec succès !");
            afficherAlertes();
            clearFields();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
        }
    }
}

// ==================== BOUTON TRIER ====================
void MainWindow::on_pushButton_trier_clicked()
{
    // Trier par date (tu peux adapter)
    QSqlQueryModel* model = alerteTmp.trierPar("DATE_ALERTE", "DESC");

    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount());

    for (int row = 0; row < model->rowCount(); row++) {
        for (int col = 0; col < model->columnCount(); col++) {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->resizeColumnsToContents();
}

// ==================== BOUTON RECHERCHER ====================
void MainWindow::on_pushButton_rechercher_clicked()
{
    QString critere = ui->lineEdit_recherche->text();

    if (critere.isEmpty()) {
        afficherAlertes();  // Si vide, afficher tout
        return;
    }

    QSqlQueryModel* model = alerteTmp.rechercher(critere);

    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount());

    for (int row = 0; row < model->rowCount(); row++) {
        for (int col = 0; col < model->columnCount(); col++) {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    ui->tableWidget->resizeColumnsToContents();
}

// ==================== BOUTON PDF ====================
void MainWindow::on_pushButton_pdf_clicked()
{
    QMessageBox::information(this, "PDF", "Fonctionnalité PDF à implémenter prochainement !");
    // Tu implémenteras la génération PDF plus tard
}
