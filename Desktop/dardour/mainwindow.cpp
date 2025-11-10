#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "alerte.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>

// Objet global pour gérer les alertes
alerte A;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Afficher les alertes au lancement
    A.afficherTable(ui->tableWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ======================== AJOUTER ========================
void MainWindow::on_pushButton_ajouter_tab_clicked()
{
    // Récupération des valeurs saisies
    QString id = ui->lineEdit_id_2->text();
    QString type = ui->lineEdit_type->text();
    QString client = ui->lineEdit_client->text();
    QString gravite = ui->lineEdit_gravite->text();
    QString description = ui->lineEdit_description->text();
    QString datee = ui->lineEdit_date->text();
    QString statut = ui->lineEdit_statut->text();

    // Vérifier que la base est ouverte
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen())
    {
        QMessageBox::critical(this, "Erreur", "Base non ouverte. Vérifie DSN, utilisateur et mot de passe.");
        return;
    }

    // Vérifier si l'ID existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM ALERTES WHERE ID=:id");
    checkQuery.bindValue(":id", id.toInt());
    if (checkQuery.exec() && checkQuery.next())
    {
        if (checkQuery.value(0).toInt() > 0)
        {
            QMessageBox::warning(this, "Erreur", "ID déjà existant !");
            return;
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'ID : " + checkQuery.lastError().text());
        return;
    }

    // Vérifier et formater la date
    QString dateFormatted = QDate::fromString(datee, "dd/MM/yyyy").toString("yyyy-MM-dd");
    if (dateFormatted.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Format de date invalide !");
        return;
    }

    // Créer l'objet alerte
    alerte temp(id, type, client, statut, description, gravite, dateFormatted);

    // Tenter l'ajout
    if (temp.ajouter())
    {
        // Mise à jour du tableau
        A.afficherTable(ui->tableWidget);

        QMessageBox::information(this, "Succès", "Alerte ajoutée !");
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.");
    }
}

// ======================== SUPPRIMER ========================
void MainWindow::on_pushButton_annuler_clicked()
{
    int id = ui->lineEdit_id->text().toInt();

    if(A.supprimer(id))
    {
        A.afficherTable(ui->tableWidget);
        QMessageBox::information(this, "Succès", "Alerte supprimée");
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Échec de la suppression");
    }
}

// ======================== MODIFIER ========================
void MainWindow::on_pushButton_modifier_tab_clicked()
{
    QString id = ui->lineEdit_id->text();
    QString type = ui->lineEdit_8->text();
    QString client = ui->lineEdit_9->text();
    QString gravite = ui->lineEdit_10->text();
    QString description = ui->lineEdit_11->text();
    QString datee = ui->lineEdit_12->text();
    QString statut = ui->lineEdit_15->text();

    // Vérifier et formater la date
    QString dateFormatted = QDate::fromString(datee, "dd/MM/yyyy").toString("yyyy-MM-dd");
    if (dateFormatted.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Format de date invalide !");
        return;
    }

    alerte temp(id, type, client, statut, description, gravite, dateFormatted);

    if(temp.modifier(id.toInt()))
    {
        A.afficherTable(ui->tableWidget);
        QMessageBox::information(this, "Succès", "Alerte modifiée");
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Échec de la modification");
    }
}
