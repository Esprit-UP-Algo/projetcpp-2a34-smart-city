#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QInputDialog>
#include <QtDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configuration initiale
    setWindowTitle("Gestion Intégrée - Employés & Équipements");

    // Vérifier les tables disponibles
    qDebug() << "=== Tables disponibles dans la base de données ===";
    QSqlQuery query("SELECT table_name FROM user_tables");    while (query.next()) {
        qDebug() << "Table:" << query.value(0).toString();
    }

    // Vérifier la structure de la table EMPLOYE
    qDebug() << "=== Structure de la table EMPLOYE ===";
    QSqlQuery structQuery("SELECT column_name, data_type FROM user_tab_columns WHERE table_name = 'EMPLOYE'");
    while (structQuery.next()) {
        qDebug() << "Colonne:" << structQuery.value(0).toString()
        << "Type:" << structQuery.value(1).toString();
    }

    // Vérifier la structure de la table EQUIPEMENT
    qDebug() << "=== Structure de la table EQUIPEMENT ===";
    QSqlQuery structQuery2("SELECT column_name, data_type FROM user_tab_columns WHERE table_name = 'EQUIPEMENT'");
    while (structQuery2.next()) {
        qDebug() << "Colonne:" << structQuery2.value(0).toString()
        << "Type:" << structQuery2.value(1).toString();
    }

    // Charger les données initiales
    chargerEmployes();
    chargerEquipements();

    // Configuration des tables
    ui->tab_emp->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_emp->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Configuration des combobox de tri pour équipements
    ui->comboBox->addItem("Trier par...");
    ui->comboBox->addItem("Nom (A-Z)");
    ui->comboBox->addItem("Nom (Z-A)");
    ui->comboBox->addItem("Consommation (Croissant)");
    ui->comboBox->addItem("Consommation (Décroissant)");
    ui->comboBox->addItem("Date (Récent)");
    ui->comboBox->addItem("Date (Ancien)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================== MÉTHODES POUR EMPLOYÉS ====================

void MainWindow::chargerEmployes()
{
    QSqlQueryModel *model = gestionEmployes.afficher();
    if (model) {
        ui->tab_emp->setModel(model);
        ui->tab_emp->resizeColumnsToContents();
    }
}

bool MainWindow::validerEmploye(const QString& cin, const QString& nom, const QString& prenom,
                                const QString& email, const QString& telephone, const QString& salaire)
{
    // Validation CIN
    QRegularExpression regexCIN("^[0-9]{8}$");
    if (!regexCIN.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit contenir exactement 8 chiffres!");
        return false;
    }

    // Validation nom et prénom
    QRegularExpression regexText("^[a-zA-ZÀ-ÿ\\s]+$");
    if (!regexText.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne doit contenir que des lettres!");
        return false;
    }
    if (!regexText.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le prénom ne doit contenir que des lettres!");
        return false;
    }

    // Validation email
    QRegularExpression regexEmail("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Format d'email invalide!");
        return false;
    }

    // Validation téléphone
    QRegularExpression regexTel("^[0-9]{8}$");
    if (!regexTel.match(telephone).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le téléphone doit contenir 8 chiffres!");
        return false;
    }

    // Validation salaire
    bool ok;
    float sal = salaire.toFloat(&ok);
    if (!ok || sal <= 0) {
        QMessageBox::warning(this, "Erreur", "Le salaire doit être un nombre positif!");
        return false;
    }

    return true;
}

void MainWindow::viderChampsEmploye()
{
    ui->lineEdit_cin->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_motdepass->clear();
    ui->lineEdit_telephon->clear();
    ui->lineEdit_poste->clear();
    ui->lineEdit_salaire->clear();
}

void MainWindow::viderChampsModificationEmploye()
{
    ui->lineEdit_cin2->clear();
    ui->lineEdit_nom2->clear();
    ui->lineEdit_prenom2->clear();
    ui->lineEdit_email2->clear();
    ui->lineEdit_motdepass2->clear();
    ui->lineEdit_tel2->clear();
    ui->lineEdit_poste_2->clear();
    ui->lineEdit_salaire_2->clear();
}

// SLOTS EMPLOYÉS
void MainWindow::on_ajouter_clicked()
{
    qDebug() << "=== DÉBUT AJOUT EMPLOYÉ ===";

    QString cin = ui->lineEdit_cin->text();
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    QString email = ui->lineEdit_email->text();
    QString motdepasse = ui->lineEdit_motdepass->text();
    QString telephone = ui->lineEdit_telephon->text();
    QString poste = ui->lineEdit_poste->text();
    QString salaire = ui->lineEdit_salaire->text();

    // Vérifier que tous les champs sont remplis
    if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        motdepasse.isEmpty() || telephone.isEmpty() || poste.isEmpty() || salaire.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires!");
        return;
    }

    if (!validerEmploye(cin, nom, prenom, email, telephone, salaire)) {
        return;
    }

    // Vérifier si le CIN existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin.toInt());
    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "Erreur", "Un employé avec ce CIN existe déjà !");
            return;
        }
    }

    employee nouvelEmploye(cin.toInt(), nom, prenom, email, motdepasse,
                           telephone.toInt(), poste, salaire.toFloat());

    if (nouvelEmploye.ajouter()) {
        chargerEmployes();
        viderChampsEmploye();
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'employé!");
    }
}

void MainWindow::on_modifier_clicked()
{
    QString cin = ui->lineEdit_cin2->text();
    QString nom = ui->lineEdit_nom2->text();
    QString prenom = ui->lineEdit_prenom2->text();
    QString email = ui->lineEdit_email2->text();
    QString motdepasse = ui->lineEdit_motdepass2->text();
    QString telephone = ui->lineEdit_tel2->text();
    QString poste = ui->lineEdit_poste_2->text();
    QString salaire = ui->lineEdit_salaire_2->text();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé à modifier!");
        return;
    }

    if (!validerEmploye(cin, nom, prenom, email, telephone, salaire)) {
        return;
    }

    if (gestionEmployes.modifier(cin.toInt(), nom, prenom, email, motdepasse,
                                 telephone.toInt(), poste, salaire.toFloat())) {
        chargerEmployes();
        viderChampsModificationEmploye();
        QMessageBox::information(this, "Succès", "Employé modifié avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'employé!");
    }
}

void MainWindow::on_supprimer_clicked()
{
    QModelIndexList selection = ui->tab_emp->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    int row = selection.at(0).row();
    QString id = ui->tab_emp->model()->index(row, 0).data().toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer cet employé?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (gestionEmployes.supprimer(id)) {
            chargerEmployes();
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès!");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void MainWindow::on_refresh_clicked()
{
    chargerEmployes();
}

void MainWindow::on_refresh_2_clicked()
{
    bool ok;
    QString cinStr = QInputDialog::getText(this, "Recherche par CIN",
                                           "Entrez le CIN à rechercher (8 chiffres):",
                                           QLineEdit::Normal, "", &ok);
    if (ok && !cinStr.isEmpty()) {
        QSqlQueryModel* model = gestionEmployes.rechercherParCIN(cinStr.toInt());
        if (model) {
            ui->tab_emp->setModel(model);
            ui->tab_emp->resizeColumnsToContents();
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QSqlQueryModel* model = gestionEmployes.trierParNom();
    if (model) {
        ui->tab_emp->setModel(model);
        ui->tab_emp->resizeColumnsToContents();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::information(this, "Info", "Génération PDF employés - À implémenter");
}

void MainWindow::on_refresh_3_clicked()
{
    QMap<QString, int> stats = gestionEmployes.statistiquesParPoste();
    QMap<QString, float> salairesMoyens = gestionEmployes.salaireMoyenParPoste();

    QString statistiques;
    statistiques += "=== STATISTIQUES EMPLOYÉS ===\n\n";
    statistiques += "Nombre total d'employés: " + QString::number(gestionEmployes.nombreEmployes()) + "\n";
    statistiques += "Salaire moyen: " + QString::number(gestionEmployes.salaireMoyen(), 'f', 2) + "\n";
    statistiques += "Salaire minimum: " + QString::number(gestionEmployes.salaireMin(), 'f', 2) + "\n";
    statistiques += "Salaire maximum: " + QString::number(gestionEmployes.salaireMax(), 'f', 2) + "\n\n";

    statistiques += "Répartition par poste:\n";
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        statistiques += "- " + it.key() + ": " + QString::number(it.value()) + " employé(s)";
        if (salairesMoyens.contains(it.key())) {
            statistiques += " (Salaire moyen: " + QString::number(salairesMoyens[it.key()], 'f', 2) + ")";
        }
        statistiques += "\n";
    }

    QMessageBox::information(this, "Statistiques", statistiques);
}

void MainWindow::on_tab_emp_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    ui->lineEdit_cin->setText(ui->tab_emp->model()->index(row, 1).data().toString());
    ui->lineEdit_nom->setText(ui->tab_emp->model()->index(row, 2).data().toString());
    ui->lineEdit_prenom->setText(ui->tab_emp->model()->index(row, 3).data().toString());
    ui->lineEdit_email->setText(ui->tab_emp->model()->index(row, 4).data().toString());
    ui->lineEdit_motdepass->setText(ui->tab_emp->model()->index(row, 5).data().toString());
    ui->lineEdit_telephon->setText(ui->tab_emp->model()->index(row, 6).data().toString());
    ui->lineEdit_poste->setText(ui->tab_emp->model()->index(row, 7).data().toString());
    ui->lineEdit_salaire->setText(ui->tab_emp->model()->index(row, 8).data().toString());

    // Remplir aussi les champs de modification
    ui->lineEdit_cin2->setText(ui->tab_emp->model()->index(row, 1).data().toString());
    ui->lineEdit_nom2->setText(ui->tab_emp->model()->index(row, 2).data().toString());
    ui->lineEdit_prenom2->setText(ui->tab_emp->model()->index(row, 3).data().toString());
    ui->lineEdit_email2->setText(ui->tab_emp->model()->index(row, 4).data().toString());
    ui->lineEdit_motdepass2->setText(ui->tab_emp->model()->index(row, 5).data().toString());
    ui->lineEdit_tel2->setText(ui->tab_emp->model()->index(row, 6).data().toString());
    ui->lineEdit_poste_2->setText(ui->tab_emp->model()->index(row, 7).data().toString());
    ui->lineEdit_salaire_2->setText(ui->tab_emp->model()->index(row, 8).data().toString());
}

void MainWindow::on_pushButton_7_clicked()
{
    viderChampsEmploye();
}

void MainWindow::on_pushButton_6_clicked()
{
    viderChampsModificationEmploye();
}

// ==================== MÉTHODES POUR ÉQUIPEMENTS ====================

void MainWindow::chargerEquipements()
{
    qDebug() << "Chargement des équipements...";
    QSqlQueryModel *model = Equipement::afficher();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        qDebug() << "Équipements chargés:" << model->rowCount() << "lignes";
    }
}

bool MainWindow::validerEquipement(const QString& id, const QString& nom, const QString& type,
                                   const QString& zone, const QString& etat, const QString& consommation)
{
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "L'ID est obligatoire!");
        return false;
    }

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom est obligatoire!");
        return false;
    }

    bool ok;
    double conso = consommation.toDouble(&ok);
    if (!ok || conso < 0) {
        QMessageBox::warning(this, "Erreur", "La consommation doit être un nombre positif ou nul!");
        return false;
    }

    return true;
}

void MainWindow::viderChampsEquipement()
{
    ui->lineEdit_30->clear();
    ui->lineEdit_31->clear();
    ui->lineEdit_32->clear();
    ui->lineEdit_33->clear();
    ui->lineEdit_34->clear();
    ui->lineEdit_35->clear();
    ui->lineEdit_36->clear();
}

void MainWindow::viderChampsModificationEquipement()
{
    ui->lineEdit_37->clear();
    ui->lineEdit_42->clear();
    ui->lineEdit_40->clear();
    ui->lineEdit_38->clear();
    ui->lineEdit_41->clear();
    ui->lineEdit_39->clear();
}

// SLOTS ÉQUIPEMENTS - CORRECTION PRINCIPALE ICI
void MainWindow::on_pushButton_13_clicked()
{
    qDebug() << "=== DÉBUT AJOUT ÉQUIPEMENT ===";

    QString id = ui->lineEdit_30->text().trimmed();
    QString nom = ui->lineEdit_31->text().trimmed();
    QString consommation = ui->lineEdit_36->text().trimmed();
    QString idEmp = ui->lineEdit_32->text().trimmed(); // ID employé
    QString type = ui->lineEdit_33->text().trimmed();
    QString zone = ui->lineEdit_34->text().trimmed();
    QString etat = ui->lineEdit_35->text().trimmed();
    QDate dateEQ = QDate::currentDate();

    if (id.isEmpty() || nom.isEmpty() || consommation.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Les champs ID, Nom et Consommation sont obligatoires!");
        return;
    }

    // CORRECTION: Vérification améliorée de l'ID employé
    if (!idEmp.isEmpty()) {
        // Vérifier que l'ID employé est numérique
        bool ok;
        int idEmpInt = idEmp.toInt(&ok);
        if (!ok) {
            // Afficher les IDs disponibles
            QSqlQuery availableEmps("SELECT ID_EMP FROM EMPLOYE ORDER BY ID_EMP");
            QStringList availableIds;
            while (availableEmps.next()) {
                availableIds << availableEmps.value(0).toString();
            }

            QMessageBox::warning(this, "Erreur",
                                 QString("L'ID Employé doit être un nombre!\n\nIDs disponibles: %1")
                                     .arg(availableIds.join(", ")));
            return;
        }

        // Vérifier que l'ID employé existe dans la base
        QSqlQuery checkEmpQuery;
        checkEmpQuery.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE ID_EMP = :idEmp");
        checkEmpQuery.bindValue(":idEmp", idEmp);

        if (checkEmpQuery.exec() && checkEmpQuery.next()) {
            int empCount = checkEmpQuery.value(0).toInt();
            if (empCount == 0) {
                // Afficher les IDs disponibles
                QSqlQuery availableEmps("SELECT ID_EMP FROM EMPLOYE ORDER BY ID_EMP");
                QStringList availableIds;
                while (availableEmps.next()) {
                    availableIds << availableEmps.value(0).toString();
                }

                QMessageBox::warning(this, "Erreur",
                                     QString("L'ID Employé '%1' n'existe pas!\n\nIDs disponibles: %2")
                                         .arg(idEmp)
                                         .arg(availableIds.join(", ")));
                return;
            }
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification de l'ID employé!");
            return;
        }
    }

    // Si l'ID employé est vide, on le laisse comme NULL
    if (idEmp.isEmpty()) {
        idEmp = ""; // Laisser vide pour que la base de données gère le NULL
    }

    Equipement nouvelEquipement(id, nom, type, zone, etat, dateEQ, consommation);
    nouvelEquipement.setIdEmp(idEmp);

    if (nouvelEquipement.ajouter()) {
        chargerEquipements();
        viderChampsEquipement();
        QMessageBox::information(this, "Succès", QString("Équipement ajouté!\n\nID: %1\nNom: %2").arg(id).arg(nom));
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'équipement!\nVérifiez les données saisies.");
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    QString id = ui->lineEdit_37->text().trimmed();
    QString nom = ui->lineEdit_42->text().trimmed();
    QString consommation = ui->lineEdit_39->text().trimmed();
    QString idEmp = ui->lineEdit_38->text().trimmed();
    QString type = ui->lineEdit_40->text().trimmed();
    QString zone = ui->lineEdit_41->text().trimmed();
    QString etat = "";
    QDate dateEQ = QDate::currentDate();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un équipement à modifier!");
        return;
    }

    if (nom.isEmpty() || consommation.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom et la consommation sont obligatoires!");
        return;
    }

    // CORRECTION: Vérification de l'ID employé pour la modification
    if (!idEmp.isEmpty()) {
        bool ok;
        int idEmpInt = idEmp.toInt(&ok);
        if (!ok) {
            QSqlQuery availableEmps("SELECT ID_EMP FROM EMPLOYE ORDER BY ID_EMP");
            QStringList availableIds;
            while (availableEmps.next()) {
                availableIds << availableEmps.value(0).toString();
            }

            QMessageBox::warning(this, "Erreur",
                                 QString("L'ID Employé doit être un nombre!\n\nIDs disponibles: %1")
                                     .arg(availableIds.join(", ")));
            return;
        }

        QSqlQuery checkEmpQuery;
        checkEmpQuery.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE ID_EMP = :idEmp");
        checkEmpQuery.bindValue(":idEmp", idEmp);

        if (checkEmpQuery.exec() && checkEmpQuery.next()) {
            int empCount = checkEmpQuery.value(0).toInt();
            if (empCount == 0) {
                QSqlQuery availableEmps("SELECT ID_EMP FROM EMPLOYE ORDER BY ID_EMP");
                QStringList availableIds;
                while (availableEmps.next()) {
                    availableIds << availableEmps.value(0).toString();
                }

                QMessageBox::warning(this, "Erreur",
                                     QString("L'ID Employé '%1' n'existe pas!\n\nIDs disponibles: %2")
                                         .arg(idEmp)
                                         .arg(availableIds.join(", ")));
                return;
            }
        }
    }

    bool ok;
    double conso = consommation.toDouble(&ok);
    if (!ok || conso < 0) {
        QMessageBox::warning(this, "Erreur", "La consommation doit être un nombre positif ou nul!");
        return;
    }

    Equipement equipement(id, nom, type, zone, etat, dateEQ, consommation);
    equipement.setIdEmp(idEmp.isEmpty() ? "" : idEmp);

    if (equipement.modifier()) {
        chargerEquipements();
        viderChampsModificationEquipement();
        QMessageBox::information(this, "Succès", "Équipement modifié avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification!");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un équipement à supprimer.");
        return;
    }

    int row = selection.at(0).row();
    QString id = ui->tableView->model()->index(row, 0).data().toString();
    QString nom = ui->tableView->model()->index(row, 1).data().toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  QString("Supprimer cet équipement?\n\nID: %1\nNom: %2").arg(id).arg(nom),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Equipement::supprimer(id)) {
            chargerEquipements();
            viderChampsEquipement();
            viderChampsModificationEquipement();
            QMessageBox::information(this, "Succès", "Équipement supprimé!");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    chargerEquipements();
    ui->comboBox->setCurrentIndex(0);
}

void MainWindow::on_pushButton_14_clicked()
{
    viderChampsEquipement();
}

void MainWindow::on_pushButton_16_clicked()
{
    viderChampsModificationEquipement();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        chargerEquipements();
        return;
    }

    QSqlQueryModel *model = nullptr;
    switch (index) {
    case 1: model = Equipement::trierParNom(true); break;
    case 2: model = Equipement::trierParNom(false); break;
    case 3: model = Equipement::trierParConsommation(true); break;
    case 4: model = Equipement::trierParConsommation(false); break;
    case 5: model = Equipement::trierParDate(false); break;
    case 6: model = Equipement::trierParDate(true); break;
    default: model = Equipement::afficher(); break;
    }

    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    QString id = ui->tableView->model()->index(row, 0).data().toString();
    QString nom = ui->tableView->model()->index(row, 1).data().toString();
    QString consommation = ui->tableView->model()->index(row, 2).data().toString();
    QString idEmp = ui->tableView->model()->index(row, 3).data().toString();
    QString type = ui->tableView->model()->index(row, 4).data().toString();
    QString zone = ui->tableView->model()->index(row, 5).data().toString();
    QString etat = ui->tableView->model()->index(row, 6).data().toString();

    // Remplir champs ajout
    ui->lineEdit_30->setText(id);
    ui->lineEdit_31->setText(nom);
    ui->lineEdit_36->setText(consommation);
    ui->lineEdit_32->setText(idEmp);
    ui->lineEdit_33->setText(type);
    ui->lineEdit_34->setText(zone);
    ui->lineEdit_35->setText(etat);

    // Remplir champs modification
    ui->lineEdit_37->setText(id);
    ui->lineEdit_42->setText(nom);
    ui->lineEdit_39->setText(consommation);
    ui->lineEdit_38->setText(idEmp);
    ui->lineEdit_40->setText(type);
    ui->lineEdit_41->setText(zone);
}
//dechet********************
void MainWindow::on_ajouter_dechet_btn_clicked()
{
    int cin = ui->cin_dechet->text().toInt();
    QDate date = ui->date_dechet->date();
    QString lieu = ui->lieu_dechet->text();
    float volume = ui->volume_dechet->text().toFloat();
    QString etat = ui->recycle->isChecked() ? "recyclé" : "non recyclé";

    Dechet D(cin, date, lieu, volume, etat);

    if (D.ajouter()) {
        QMessageBox::information(this, "Ajout", "Déchet ajouté avec succès !");
        ui->tableView_dechet->setModel(tmpDechet.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout échoué !");
    }

}
void MainWindow::on_modifier_dechet_btn_clicked()
{
    int cin = ui->cin_dechet_modifier->text().toInt();
    QDate date = ui->date_dechet_modifier->date();
    QString lieu = ui->lieu_dechet_modifier->text();
    float volume = ui->volume_dechet_modifier->text().toFloat();
    QString etat = ui->recycle_modifier->isChecked() ? "recyclé" : "non recyclé";

    Dechet D(cin, date, lieu, volume, etat);

    if (D.modifier()) {
        ui->tableView_dechet->setModel(D.afficher());
    }
}

void MainWindow::on_supprimer_dechet_btn_clicked()
{
    int cin = ui->lineEdit_supprimer_dechet->text().toInt();

    if (tmpDechet.supprimer(cin)) {
        QMessageBox::information(this, "Suppression", "Déchet supprimé !");
        ui->tableView_dechet->setModel(tmpDechet.afficher()); // refresh
    } else {
        QMessageBox::critical(this, "Erreur", "Suppression échouée !");
    }
}

void MainWindow::on_afficher_dechet_btn_clicked()
{
    ui->tableView_dechet->setModel(tmpDechet.afficher());
}

//les metiers tri+rech
// Recherche
void MainWindow::on_rech_dechet_btn_clicked()
{
    QString critere = ui->comboBox_rech_dechet->currentText();
    QString valeur = ui->lineEdit_rech_dechet->text();   // AJOUT OBLIGATOIRE

    QString colonne;

    if (critere == "CIN") colonne = "CIN";
    if (critere == "Lieu") colonne = "LIEU_COLLECTE";

    ui->tableView_dechet->setModel(tmpDechet.rechercher(colonne, valeur));
}

// Tri
void MainWindow::on_trier_dechet_btn_clicked()
{
    QString critere = ui->comboBox_trier_dechet->currentText();
    QString colonne;

    if (critere == "date") colonne = "DATE_COLLECTE";
    if (critere == "lieu") colonne = "LIEU_COLLECTE";
    if (critere == "etat") colonne = "ETAT";

    ui->tableView_dechet->setModel(tmpDechet.trier(colonne));

}

void MainWindow::on_refresh_dechet_btn_clicked()
{
    ui->tableView_dechet->setModel(tmpDechet.afficher());
}
void MainWindow::on_pdf_dechet_btn_clicked()
{
    QPdfWriter pdf("dechets.pdf");
    QPainter painter(&pdf);

    painter.setFont(QFont("Helvetica", 12));
    painter.drawText(100, 100, "Liste des déchets");

    QSqlQuery query("SELECT * FROM DECHETS");
    int y = 300;

    while (query.next()) {
        QString line = "CIN : " + query.value(0).toString()
        + " | Date : " + query.value(1).toDate().toString("dd/MM/yyyy")
            + " | Lieu : " + query.value(2).toString()
            + " | Volume : " + query.value(3).toString()
            + " | Etat : " + query.value(4).toString();

        painter.drawText(100, y, line);
        y += 200;
    }

    painter.end();
    QMessageBox::information(this, "PDF", "PDF généré : dechets.pdf !");
}

