// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    refreshTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTable()
{
    ui->tableWidget->setColumnCount(7);
    QStringList headers = {"ID", "Type", "Client", "Statut", "Description", "Gravité", "Date"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    alerte a;
    a.afficherTable(ui->tableWidget);
}

void MainWindow::clearForm()
{
    // TAB AJOUTER
    ui->lineEdit_id_2->clear();
    ui->lineEdit_type->clear();
    ui->lineEdit_client->clear();
    ui->lineEdit_statut->clear();
    ui->lineEdit_description->clear();
    ui->lineEdit_gravite->clear();
    ui->lineEdit_date->clear();

    // TAB MODIFIER
    ui->lineEdit_id->clear();
    ui->lineEdit_12->clear();  // type
    ui->lineEdit_10->clear();  // client
    ui->lineEdit_9->clear();   // statut
    ui->lineEdit_11->clear();  // description
    ui->lineEdit_8->clear();   // gravite
    ui->lineEdit_15->clear();  // date
}

// ========== TAB AJOUTER ==========
void MainWindow::on_pushButton_ajouter_tab_clicked()
{
    int id = ui->lineEdit_id_2->text().toInt();
    QString type = ui->lineEdit_type->text();
    QString client = ui->lineEdit_client->text();
    QString statut = ui->lineEdit_statut->text();
    QString desc = ui->lineEdit_description->text();
    QString grav = ui->lineEdit_gravite->text();
    QString date = ui->lineEdit_date->text();

    if (id <= 0 || type.isEmpty() || client.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "ID, Type et Client obligatoires !");
        return;
    }

    alerte a(id, type, client, statut, desc, grav, date);
    if (a.ajouter()) {
        QMessageBox::information(this, "Succès", "Alerte ajoutée !");
        clearForm();
        refreshTable();
    } else {
        QMessageBox::critical(this, "Échec", "Échec ajout ! Voir console.");
    }
}

// ========== TAB MODIFIER ==========
void MainWindow::on_pushButton__modifier_tab_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une alerte dans le tableau !");
        return;
    }

    QString type = ui->lineEdit_12->text();
    QString client = ui->lineEdit_10->text();
    QString statut = ui->lineEdit_9->text();
    QString desc = ui->lineEdit_11->text();
    QString grav = ui->lineEdit_8->text();
    QString date = ui->lineEdit_15->text();

    alerte a(id, type, client, statut, desc, grav, date);
    if (a.modifier()) {
        QMessageBox::information(this, "Succès", "Alerte modifiée !");
        clearForm();
        refreshTable();
    } else {
        QMessageBox::critical(this, "Échec", "Échec modification !");
    }
}

// ========== BOUTONS PRINCIPAUX ==========
void MainWindow::on_pushButton_3_clicked()
{
    // Trier par type (à implémenter si nécessaire)
    QMessageBox::information(this, "Info", "Tri à implémenter");
}

void MainWindow::on_pushButton_4_clicked()
{
    // SUPPRIMER
    int row = ui->tableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne !");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();

    if (alerte().supprimer(id)) {
        QMessageBox::information(this, "Succès", "Alerte supprimée !");
        clearForm();
        refreshTable();
    } else {
        QMessageBox::critical(this, "Échec", "Échec suppression !");
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    // Export PDF (à implémenter)
    QMessageBox::information(this, "Info", "Export PDF à implémenter");
}

// ========== SELECTION TABLEAU ==========
void MainWindow::on_tableWidget_itemSelectionChanged()
{
    int row = ui->tableWidget->currentRow();
    if (row == -1) return;

    // Remplir le TAB MODIFIER
    ui->lineEdit_id->setText(ui->tableWidget->item(row, 0)->text());
    ui->lineEdit_12->setText(ui->tableWidget->item(row, 1)->text());  // type
    ui->lineEdit_10->setText(ui->tableWidget->item(row, 2)->text());  // client
    ui->lineEdit_9->setText(ui->tableWidget->item(row, 3)->text());   // statut
    ui->lineEdit_11->setText(ui->tableWidget->item(row, 4)->text());  // description
    ui->lineEdit_8->setText(ui->tableWidget->item(row, 5)->text());   // gravite
    ui->lineEdit_15->setText(ui->tableWidget->item(row, 6)->text());  // date
}

// ========== BOUTONS ANNULER ==========
void MainWindow::on_pushButton_annuler_clicked()
{
    clearForm();
}

void MainWindow::on_pushButton_annuler_2_clicked()
{
    clearForm();
}

// ========== RECHERCHE ==========
void MainWindow::on_lineEdit_13_textChanged(const QString &arg1)
{
    // Recherche par type
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        bool match = ui->tableWidget->item(row, 1)->text().contains(arg1, Qt::CaseInsensitive);
        ui->tableWidget->setRowHidden(row, !match);
    }
}
