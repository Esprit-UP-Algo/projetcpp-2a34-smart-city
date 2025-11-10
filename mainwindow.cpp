#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dechet.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDate>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---- Table setup ----
    ui->tableWidget->setColumnCount(5);
    QStringList headers;
    headers << "Cn" << "Date de collecte" << "Lieu de collecte" << "Volume/Poids" << "Etat";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // ---- Load data initially ----
    rafraichir();

    // ---- Fill the "Modifier" tab when a row is clicked ----
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, [this](int row, int){
        if (!ui->tableWidget->item(row, 0)) return;

        ui->lineEdit_nom_3->setText(ui->tableWidget->item(row, 0)->text());

        const QString dateText = ui->tableWidget->item(row, 1)->text();
        QDate d = parseDateFromLineEdit(dateText);
        if (d.isValid())
            ui->lineEdit_date_3->setDate(d);
        else
            ui->lineEdit_date_3->clear();

        ui->lineEdit_lieu_3->setText(ui->tableWidget->item(row, 2)->text());
        ui->lineEdit_volume_3->setText(ui->tableWidget->item(row, 3)->text());
        ui->lineEdit_etat_3->setText(ui->tableWidget->item(row, 4)->text());

        if (ui->tabWidget)
            ui->tabWidget->setCurrentIndex(1);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// -------------------- Helpers --------------------

QDate MainWindow::parseDateFromLineEdit(const QString& s)
{
    QDate d = QDate::fromString(s.trimmed(), "yyyy-MM-dd");
    if (!d.isValid())
        d = QDate::fromString(s.trimmed(), "dd/MM/yyyy");
    return d;
}

void MainWindow::rafraichirAvecModele(QSqlQueryModel* model)
{
    const int rows = model ? model->rowCount() : 0;
    ui->tableWidget->setRowCount(rows);

    for (int r = 0; r < rows; ++r) {
        const QSqlRecord rec = model->record(r);
        ui->tableWidget->setItem(r, 0, new QTableWidgetItem(rec.value("CN").toString()));
        ui->tableWidget->setItem(r, 1, new QTableWidgetItem(rec.value("DATE_COLLECTE").toDate().toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(r, 2, new QTableWidgetItem(rec.value("LIEU").toString()));
        ui->tableWidget->setItem(r, 3, new QTableWidgetItem(rec.value("VOLUME_POIDS").toString()));
        ui->tableWidget->setItem(r, 4, new QTableWidgetItem(rec.value("ETAT").toString()));
    }

    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::rafraichir()
{
    QSqlQueryModel* m = Dechet::afficher();
    rafraichirAvecModele(m);
    m->deleteLater();
}

// -------------------- Slots --------------------

void MainWindow::on_pushButton_ajouter_clicked()
{
    const int cn = ui->lineEdit_nom->text().toInt();
    const QDate date = ui->dateEdit->date();
    const QString lieu = ui->lineEdit_lieu->text().trimmed();
    const double volume = ui->lineEdit_volume->text().toDouble();
    const QString etat = ui->lineEdit_etat->text().trimmed();

    if (cn <= 0 || !date.isValid() || lieu.isEmpty() || etat.isEmpty()) {
        QMessageBox::warning(this, "Champs invalides",
                             "Vérifie CN (>0), Date, Lieu et État.");
        return;
    }

    Dechet d(cn, date, lieu, volume, etat);
    const bool ok = d.ajouter();
    if (ok) {
        QMessageBox::information(this, "Ajout", "Déchet ajouté avec succès.");
        rafraichir();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout (clé CN existante ?).");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    const auto items = ui->tableWidget->selectedItems();
    if (items.isEmpty()) {
        QMessageBox::warning(this, "Suppression", "Sélectionne d'abord une ligne dans le tableau.");
        return;
    }
    const int row = items.first()->row();
    const int cn = ui->tableWidget->item(row, 0)->text().toInt();
    if (cn <= 0) {
        QMessageBox::warning(this, "Suppression", "CN invalide.");
        return;
    }

    if (QMessageBox::question(this, "Confirmer", "Supprimer cet enregistrement ?") == QMessageBox::Yes) {
        if (Dechet::supprimer(cn)) {
            QMessageBox::information(this, "Suppression", "Supprimé avec succès.");
            rafraichir();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    const int cn = ui->lineEdit_nom_3->text().toInt();
    const QDate date = ui->lineEdit_date_3->date();
    const QString lieu = ui->lineEdit_lieu_3->text().trimmed();
    const double volume = ui->lineEdit_volume_3->text().toDouble();
    const QString etat = ui->lineEdit_etat_3->text().trimmed();

    if (cn <= 0 || !date.isValid() || lieu.isEmpty() || etat.isEmpty()) {
        QMessageBox::warning(this, "Champs invalides",
                             "Vérifie CN (>0), Date, Lieu et État.");
        return;
    }

    Dechet d(cn, date, lieu, volume, etat);
    if (d.modifier()) {
        QMessageBox::information(this, "Modification", "Enregistrement modifié avec succès.");
        rafraichir();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification. Vérifie que l'ID existe.");
    }
}

void MainWindow::on_pushButton_trie_clicked()
{
    QString col = ui->comboBox->currentText().toUpper();
    if (!Dechet::colonneValide(col)) col = "CN";

    QSqlQueryModel* m = Dechet::afficherTriePar(col, true);
    rafraichirAvecModele(m);
    m->deleteLater();
}

void MainWindow::on_lineEdit_recherche_textChanged(const QString &text)
{
    QSqlQueryModel* m = Dechet::rechercher(text);
    rafraichirAvecModele(m);
    m->deleteLater();
}
