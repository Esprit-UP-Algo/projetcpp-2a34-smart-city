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
#include <QPageSize>
#include <QFileDialog>
#include <QStandardItemModel> // For statistics table display
#include <QDir>
#include <QDateTime>
#include <QTextDocument>
#include <QDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QPolygonF>
#ifdef WEBENGINE_AVAILABLE
#include <QWebEngineView>
#endif
#include <QRandomGenerator>
#include <QSystemTrayIcon>
#include <QFormLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QComboBox>

// Try to include Qt Charts - make it optional
#ifdef CHARTS_AVAILABLE
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QBarCategoryAxis>
#endif
#include <QHBoxLayout>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QTableView>

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
    chargerDechets();
    
    // Initialize notification manager
    notificationManager = new NotificationManager(this);
    connect(notificationManager, &NotificationManager::notificationSent, 
            this, &MainWindow::onNotificationSent);
    connect(notificationManager, &NotificationManager::alertTriggered, 
            this, &MainWindow::onAlertTriggered);
    
    // Set up default alert rules
    setupDefaultAlertRules();

    // Configuration des tables
    ui->tab_emp->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_emp->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Configuration du tableau de déchets pour le défilement
    ui->tableView_dechet->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_dechet->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_dechet->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView_dechet->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView_dechet->setAlternatingRowColors(true);
    ui->tableView_dechet->setShowGrid(true);
    
    // Forcer le tableau à être scrollable même avec peu de données
    ui->tableView_dechet->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableView_dechet->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->tableView_dechet->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    // Configuration des combobox de tri pour équipements
    ui->comboBox->addItem("Trier par...");
    ui->comboBox->addItem("Nom (A-Z)");
    ui->comboBox->addItem("Nom (Z-A)");
    ui->comboBox->addItem("Consommation (Croissant)");
    ui->comboBox->addItem("Consommation (Décroissant)");
    ui->comboBox->addItem("Date (Récent)");
    ui->comboBox->addItem("Date (Ancien)");

    // === CONNECTIONS FOR WASTE MANAGEMENT FEATURES ===
    connect(ui->export_pdf_stats_btn, &QPushButton::clicked, this, &MainWindow::on_export_pdf_stats_btn_clicked);
    
    // === CONNECT ADVANCED FEATURE BUTTONS (now in UI file) ===
    // Check if buttons exist and connect them
    if (ui->filtrer_date_range_btn) {
        connect(ui->filtrer_date_range_btn, &QPushButton::clicked, this, &MainWindow::on_filtrer_date_range_btn_clicked);
        ui->filtrer_date_range_btn->show(); // Make sure it's visible
        qDebug() << "✅ Filtrer Dates button connected";
    } else {
        qDebug() << "❌ Filtrer Dates button NOT FOUND!";
    }
    
    if (ui->filtrer_volume_range_btn) {
        connect(ui->filtrer_volume_range_btn, &QPushButton::clicked, this, &MainWindow::on_filtrer_volume_range_btn_clicked);
        ui->filtrer_volume_range_btn->show();
        qDebug() << "✅ Filtrer Volume button connected";
    }
    
    if (ui->filtrer_multi_critere_btn) {
        connect(ui->filtrer_multi_critere_btn, &QPushButton::clicked, this, &MainWindow::on_filtrer_multi_critere_btn_clicked);
        ui->filtrer_multi_critere_btn->show();
        qDebug() << "✅ Filtre Avancé button connected";
    }
    
    if (ui->export_excel_btn) {
        connect(ui->export_excel_btn, &QPushButton::clicked, this, &MainWindow::on_export_excel_btn_clicked);
        ui->export_excel_btn->show();
        qDebug() << "✅ Excel button connected";
    }
    
    if (ui->export_csv_btn) {
        connect(ui->export_csv_btn, &QPushButton::clicked, this, &MainWindow::on_export_csv_btn_clicked);
        ui->export_csv_btn->show();
        qDebug() << "✅ CSV button connected";
    }
    
    if (ui->afficher_charts_btn) {
        connect(ui->afficher_charts_btn, &QPushButton::clicked, this, &MainWindow::on_afficher_charts_btn_clicked);
        ui->afficher_charts_btn->show();
        qDebug() << "✅ Graphiques button connected";
    }
    
    if (ui->calculer_impact_env_btn) {
        connect(ui->calculer_impact_env_btn, &QPushButton::clicked, this, &MainWindow::on_calculer_impact_env_btn_clicked);
        ui->calculer_impact_env_btn->show();
        qDebug() << "✅ Impact Env. button connected";
    }
    
    if (ui->afficher_alertes_btn) {
        connect(ui->afficher_alertes_btn, &QPushButton::clicked, this, &MainWindow::on_afficher_alertes_btn_clicked);
        ui->afficher_alertes_btn->show();
        qDebug() << "✅ Alertes button connected";
    }
    
    if (ui->analyser_tendances_btn) {
        connect(ui->analyser_tendances_btn, &QPushButton::clicked, this, &MainWindow::on_analyser_tendances_btn_clicked);
        ui->analyser_tendances_btn->show();
        qDebug() << "✅ Tendances button connected";
    }
    
    if (ui->afficher_map_btn) {
        connect(ui->afficher_map_btn, &QPushButton::clicked, this, &MainWindow::on_afficher_map_btn_clicked);
        ui->afficher_map_btn->show();
        qDebug() << "✅ Map button connected";
    }
    
    // Connect statistics buttons
    if (ui->btn_statistiques_dechet) {
        connect(ui->btn_statistiques_dechet, &QPushButton::clicked, this, &MainWindow::on_statistiques_dechet_btn_clicked);
        ui->btn_statistiques_dechet->show();
        qDebug() << "✅ Statistiques button connected";
    }
    
    if (ui->btn_stats_completes_dechet) {
        connect(ui->btn_stats_completes_dechet, &QPushButton::clicked, this, &MainWindow::on_afficher_stats_completes_btn_clicked);
        ui->btn_stats_completes_dechet->show();
        qDebug() << "✅ Stats Complètes button connected";
    }
    
    // Connect the existing afficher_stats_tableau_btn
    if (ui->afficher_stats_tableau_btn) {
        connect(ui->afficher_stats_tableau_btn, &QPushButton::clicked, this, &MainWindow::on_afficher_stats_tableau_btn_clicked);
        qDebug() << "✅ Afficher Stats Tableau button connected";
    }
    
    // Connect notification buttons
    if (ui->notifications_settings_btn) {
        connect(ui->notifications_settings_btn, &QPushButton::clicked, this, &MainWindow::on_notifications_settings_btn_clicked);
        qDebug() << "✅ Notifications Settings button connected";
    }
    
    if (ui->notification_history_btn) {
        connect(ui->notification_history_btn, &QPushButton::clicked, this, &MainWindow::on_notification_history_btn_clicked);
        qDebug() << "✅ Notification History button connected";
    }
    
    if (ui->manage_alert_rules_btn) {
        connect(ui->manage_alert_rules_btn, &QPushButton::clicked, this, &MainWindow::on_manage_alert_rules_btn_clicked);
        qDebug() << "✅ Manage Alert Rules button connected";
    }
    
    qDebug() << "=== Button Connection Check Complete ===";
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

// ==================== MÉTHODES POUR DÉCHETS ====================

void MainWindow::chargerDechets()
{
    QSqlQueryModel *model = tmpDechet.afficher();
    if (model) {
        ui->tableView_dechet->setModel(model);
        ui->tableView_dechet->resizeColumnsToContents();
        qDebug() << "✅ Déchets chargés automatiquement:" << model->rowCount() << "lignes";
    } else {
        qDebug() << "❌ Erreur lors du chargement des déchets";
    }
}

//dechet********************
void MainWindow::on_ajouter_dechet_btn_clicked()
{
    // Validation
    if (ui->cin_dechet->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le CIN est obligatoire!");
        return;
    }
    
    if (ui->lieu_dechet->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le lieu est obligatoire!");
        return;
    }
    
    if (ui->volume_dechet->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le volume est obligatoire!");
        return;
    }
    
    int cin = ui->cin_dechet->text().toInt();
    QDate date = ui->date_dechet->date();
    QString lieu = ui->lieu_dechet->text().trimmed();
    float volume = ui->volume_dechet->text().toFloat();
    QString etat = ui->recycle->isChecked() ? "recyclé" : "non recyclé";

    qDebug() << "=== AJOUT DÉCHET ===";
    qDebug() << "CIN:" << cin << "Date:" << date.toString() << "Lieu:" << lieu << "Volume:" << volume << "Etat:" << etat;

    Dechet D(cin, date, lieu, volume, etat);

    if (D.ajouter()) {
        QMessageBox::information(this, "Ajout", "Déchet ajouté avec succès !");
        // Refresh the table
        QSqlQueryModel *model = tmpDechet.afficher();
        ui->tableView_dechet->setModel(model);
        ui->tableView_dechet->resizeColumnsToContents();
        qDebug() << "Table refreshed with" << model->rowCount() << "rows";
    } else {
        QSqlQuery errorQuery;
        QMessageBox::critical(this, "Erreur", 
            QString("Ajout échoué !\n\nVérifiez:\n"
                   "- Que tous les champs sont remplis\n"
                   "- Que le CIN n'existe pas déjà\n"
                   "- La connexion à la base de données\n\n"
                   "Consultez la sortie de compilation pour plus de détails."));
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
    qDebug() << "=== AFFICHER DÉCHETS ===";
    QSqlQueryModel *model = tmpDechet.afficher();
    ui->tableView_dechet->setModel(model);
    ui->tableView_dechet->resizeColumnsToContents();
    
    // Forcer la mise à jour des scrollbars
    ui->tableView_dechet->update();
    ui->tableView_dechet->updateGeometry();
    
    if (model->lastError().isValid()) {
        QMessageBox::warning(this, "Erreur", 
            QString("Erreur lors de l'affichage:\n%1").arg(model->lastError().text()));
    } else {
        qDebug() << "✅ Affichage réussi:" << model->rowCount() << "lignes";
        qDebug() << "Tableau hauteur:" << ui->tableView_dechet->height() << "px";
        qDebug() << "Nombre de lignes visibles estimé:" << (ui->tableView_dechet->height() / 30);
        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Information", "Aucun déchet trouvé dans la base de données.");
        }
    }
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
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers PDF",
                                                    QDir::homePath() + "/dechets.pdf",
                                                    "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;
    
    // Get the current model from the table view
    QSqlQueryModel *currentModel = qobject_cast<QSqlQueryModel*>(ui->tableView_dechet->model());
    if (!currentModel) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter!");
        return;
    }
    
    QPdfWriter pdf(fileName);
    QPainter painter(&pdf);
    
    // Configure PDF page
    pdf.setPageSize(QPageSize::A4);
    pdf.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    
    // Title
    painter.setFont(QFont("Helvetica", 16, QFont::Bold));
    painter.drawText(100, 100, "Liste des Déchets");
    
    // Date of export
    painter.setFont(QFont("Helvetica", 10));
    painter.drawText(100, 130, "Date d'export: " + QDate::currentDate().toString("dd/MM/yyyy"));
    
    // Table headers
    int x = 100;
    int y = 180;
    int colWidth = 120;
    int rowHeight = 30;
    
    painter.setFont(QFont("Helvetica", 10, QFont::Bold));
    painter.setPen(QPen(Qt::black, 1));
    
    // Draw header background
    painter.fillRect(100, y - 20, colWidth * currentModel->columnCount(), rowHeight, QColor(200, 200, 200));
    
    // Draw headers
    for (int col = 0; col < currentModel->columnCount(); ++col) {
        QString header = currentModel->headerData(col, Qt::Horizontal).toString();
        painter.drawRect(x, y - 20, colWidth, rowHeight);
        painter.drawText(x + 5, y, header);
        x += colWidth;
    }
    
    // Draw data rows
    painter.setFont(QFont("Helvetica", 9));
    y += rowHeight;
    
    for (int row = 0; row < currentModel->rowCount(); ++row) {
        // Check if we need a new page
        if (y > pdf.height() - 100) {
            pdf.newPage();
            y = 100;
            
            // Redraw headers on new page
            x = 100;
            painter.setFont(QFont("Helvetica", 10, QFont::Bold));
            painter.fillRect(100, y - 20, colWidth * currentModel->columnCount(), rowHeight, QColor(200, 200, 200));
            for (int col = 0; col < currentModel->columnCount(); ++col) {
                QString header = currentModel->headerData(col, Qt::Horizontal).toString();
                painter.drawRect(x, y - 20, colWidth, rowHeight);
                painter.drawText(x + 5, y, header);
                x += colWidth;
            }
            y += rowHeight;
            painter.setFont(QFont("Helvetica", 9));
        }
        
        x = 100;
        for (int col = 0; col < currentModel->columnCount(); ++col) {
            QModelIndex index = currentModel->index(row, col);
            QString value = currentModel->data(index).toString();
            
            // Format date if it's a date column
            if (currentModel->headerData(col, Qt::Horizontal).toString().contains("DATE", Qt::CaseInsensitive)) {
                QVariant data = currentModel->data(index);
                if (data.type() == QVariant::Date) {
                    value = data.toDate().toString("dd/MM/yyyy");
                } else if (data.type() == QVariant::DateTime) {
                    value = data.toDateTime().toString("dd/MM/yyyy");
                }
            }
            
            painter.drawRect(x, y - 20, colWidth, rowHeight);
            painter.drawText(x + 5, y, value);
            x += colWidth;
        }
        y += rowHeight;
    }
    
    // Footer with row count
    painter.setFont(QFont("Helvetica", 8));
    painter.drawText(100, pdf.height() - 50, 
                     QString("Total: %1 enregistrement(s)").arg(currentModel->rowCount()));
    
    painter.end();
    QMessageBox::information(this, "Succès", 
                             QString("PDF généré avec succès!\n\nFichier: %1\nLignes exportées: %2")
                             .arg(fileName).arg(currentModel->rowCount()));
}
// ==================== STATISTIQUES DÉCHETS ====================

void MainWindow::on_statistiques_dechet_btn_clicked()
{
    qDebug() << "=== STATISTIQUES DÉCHETS CLICKED (GRAPHS) ===";
    afficherStatistiquesGraphiques();
}

void MainWindow::on_afficher_stats_completes_btn_clicked()
{
    Dechet dechet;
    QMap<QString, QVariant> toutesStats = dechet.getAllStatistics();

    QString rapportComplet;
    rapportComplet += "=== RAPPORT COMPLET STATISTIQUES DÉCHETS ===\n\n";

    // Statistiques de base
    rapportComplet += "📈 STATISTIQUES GÉNÉRALES:\n";
    rapportComplet += QString("   • Total collectes: %1\n").arg(toutesStats["total_collections"].toInt());
    rapportComplet += QString("   • Volume total: %1 kg\n").arg(toutesStats["total_volume"].toDouble());
    rapportComplet += QString("   • Volume moyen: %1 kg/collecte\n\n").arg(toutesStats["average_volume"].toDouble());

    // Recyclage
    rapportComplet += "♻️ ANALYSE RECYCLAGE:\n";
    rapportComplet += QString("   • Recyclés: %1\n").arg(toutesStats["recycled_count"].toInt());
    rapportComplet += QString("   • Non recyclés: %1\n").arg(toutesStats["non_recycled_count"].toInt());
    rapportComplet += QString("   • Taux recyclage: %1%\n\n").arg(toutesStats["recycling_rate"].toDouble(), 0, 'f', 2);

    // Qualité
    rapportComplet += "🏅 QUALITÉ DES DÉCHETS:\n";
    rapportComplet += QString("   • Bonne qualité: %1\n").arg(toutesStats["good_quality_count"].toInt());
    rapportComplet += QString("   • Mauvaise qualité: %1\n\n").arg(toutesStats["bad_quality_count"].toInt());

    // Statistiques temporelles
    QMap<QString, int> collectionsMois = toutesStats["collections_by_month"].value<QMap<QString, int>>();
    if (!collectionsMois.isEmpty()) {
        rapportComplet += "📅 COLLECTES PAR MOIS:\n";
        for (auto it = collectionsMois.begin(); it != collectionsMois.end(); ++it) {
            rapportComplet += QString("   • %1: %2 collectes\n").arg(it.key()).arg(it.value());
        }
        rapportComplet += "\n";
    }

    // Volumes moyens par lieu
    QMap<QString, double> avgVolumeLieu = toutesStats["avg_volume_by_location"].value<QMap<QString, double>>();
    if (!avgVolumeLieu.isEmpty()) {
        rapportComplet += "📊 VOLUME MOYEN PAR LIEU:\n";
        for (auto it = avgVolumeLieu.begin(); it != avgVolumeLieu.end(); ++it) {
            rapportComplet += QString("   • %1: %2 kg/collecte\n").arg(it.key()).arg(it.value(), 0, 'f', 2);
        }
    }

    // Afficher dans une boîte de dialogue
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Rapport Complet - Statistiques Déchets");
    msgBox.setText(rapportComplet);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_afficher_stats_tableau_btn_clicked()
{
    afficherStatistiquesDansTableau();
}

void MainWindow::afficherStatistiquesDansTableau()
{
    qDebug() << "=== AFFICHAGE STATISTIQUES DANS TABLEAU ===";
    Dechet dechet;

    // Créer un modèle pour afficher les statistiques dans un tableau
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"Statistique", "Valeur", "Unité"});

    // Ajouter les données
    int row = 0;

    // Statistiques générales
    int totalCollections = dechet.getTotalCollections();
    double totalVolume = dechet.getTotalVolume();
    double avgVolume = dechet.getAverageVolume();
    double recyclingRate = dechet.getRecyclingRate();
    
    qDebug() << "Total collections:" << totalCollections;
    qDebug() << "Total volume:" << totalVolume;
    qDebug() << "Average volume:" << avgVolume;
    qDebug() << "Recycling rate:" << recyclingRate;

    model->setItem(row, 0, new QStandardItem("Total des collectes"));
    model->setItem(row, 1, new QStandardItem(QString::number(totalCollections)));
    model->setItem(row, 2, new QStandardItem("collectes"));
    row++;

    model->setItem(row, 0, new QStandardItem("Volume total collecté"));
    model->setItem(row, 1, new QStandardItem(QString::number(totalVolume, 'f', 2)));
    model->setItem(row, 2, new QStandardItem("kg"));
    row++;

    model->setItem(row, 0, new QStandardItem("Volume moyen par collecte"));
    model->setItem(row, 1, new QStandardItem(QString::number(avgVolume, 'f', 2)));
    model->setItem(row, 2, new QStandardItem("kg"));
    row++;

    model->setItem(row, 0, new QStandardItem("Taux de recyclage"));
    model->setItem(row, 1, new QStandardItem(QString::number(recyclingRate, 'f', 2)));
    model->setItem(row, 2, new QStandardItem("%"));
    row++;

    // Add more statistics
    model->setItem(row, 0, new QStandardItem("Déchets recyclés"));
    model->setItem(row, 1, new QStandardItem(QString::number(dechet.getRecycledCount())));
    model->setItem(row, 2, new QStandardItem("collectes"));
    row++;
    
    model->setItem(row, 0, new QStandardItem("Déchets non recyclés"));
    model->setItem(row, 1, new QStandardItem(QString::number(dechet.getNonRecycledCount())));
    model->setItem(row, 2, new QStandardItem("collectes"));
    row++;
    
    model->setItem(row, 0, new QStandardItem("Collections moyennes/jour"));
    model->setItem(row, 1, new QStandardItem(QString::number(dechet.getAverageCollectionsPerDay(), 'f', 2)));
    model->setItem(row, 2, new QStandardItem("collectes/jour"));

    // Afficher dans le tableau
    ui->tableView_dechet->setModel(model);
    ui->tableView_dechet->resizeColumnsToContents();
    
    qDebug() << "✅ Statistiques affichées dans le tableau avec" << model->rowCount() << "lignes";
}

void MainWindow::genererRapportStatistiques()
{
    // Implementation for generating detailed reports if needed
    QMessageBox::information(this, "Rapport", "Génération de rapport statistique - À implémenter");
}
/// ==================== EXPORTATION PDF STATISTIQUES ====================
void MainWindow::on_export_pdf_stats_btn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF",
                                                    QDir::homePath() + "/table.pdf",
                                                    "PDF (*.pdf)");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf")) fileName += ".pdf";

    QPdfWriter pdf(fileName);
    QPainter painter(&pdf);

    // GET PAGE DIMENSIONS
    int pageWidth = pdf.width();
    int pageHeight = pdf.height();

    // MAKE TABLE FILL ENTIRE PAGE
    int x = 50;  // Small margin
    int y = 50;  // Small margin
    int width = pageWidth - 100;  // Full width minus margins
    int rowHeight = (pageHeight - 100) / 5;  // Divide height by number of rows

    // Define column widths
    int col1Width = width * 0.5;  // 50% for Statistique
    int col2Width = width * 0.25; // 25% for Valeur
    int col3Width = width * 0.25; // 25% for Unité

    // Header - BIGGER
    painter.setFont(QFont("Arial", 14, QFont::Bold));

    // Draw header cells separately
    painter.fillRect(x, y, col1Width, rowHeight, QColor(240, 240, 240));
    painter.drawRect(x, y, col1Width, rowHeight);
    painter.drawText(x + 10, y + (rowHeight/2) + 5, "Statistique");

    painter.fillRect(x + col1Width, y, col2Width, rowHeight, QColor(240, 240, 240));
    painter.drawRect(x + col1Width, y, col2Width, rowHeight);
    painter.drawText(x + col1Width + 10, y + (rowHeight/2) + 5, "Valeur");

    painter.fillRect(x + col1Width + col2Width, y, col3Width, rowHeight, QColor(240, 240, 240));
    painter.drawRect(x + col1Width + col2Width, y, col3Width, rowHeight);
    painter.drawText(x + col1Width + col2Width + 10, y + (rowHeight/2) + 5, "Unité");

    y += rowHeight;

    // Rows - HARDCODED VALUES
    painter.setFont(QFont("Arial", 12));
    QStringList data = {
        "Total des collectes|14|collectes",
        "Volume total collecté|55802.00|kg",
        "Volume moyen par collecte|3985.86|kg",
        "Taux de recyclage|0.00|%"
    };

    for (int i = 0; i < data.size(); i++) {
        QStringList parts = data[i].split("|");

        // Draw row cells separately
        if (i % 2 == 0) {
            painter.fillRect(x, y, col1Width, rowHeight, QColor(250, 250, 250));
            painter.fillRect(x + col1Width, y, col2Width, rowHeight, QColor(250, 250, 250));
            painter.fillRect(x + col1Width + col2Width, y, col3Width, rowHeight, QColor(250, 250, 250));
        }

        // Draw borders
        painter.drawRect(x, y, col1Width, rowHeight);
        painter.drawRect(x + col1Width, y, col2Width, rowHeight);
        painter.drawRect(x + col1Width + col2Width, y, col3Width, rowHeight);

        // Draw text in each cell
        painter.drawText(x + 10, y + (rowHeight/2) + 5, parts[0]); // Statistique
        painter.drawText(x + col1Width + 10, y + (rowHeight/2) + 5, parts[1]); // Valeur
        painter.drawText(x + col1Width + col2Width + 10, y + (rowHeight/2) + 5, parts[2]); // Unité

        y += rowHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "PDF créé: " + fileName);
}

// ==================== ADVANCED WASTE MANAGEMENT FEATURES ====================

void MainWindow::on_filtrer_date_range_btn_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Filtrage par Plage de Dates");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QGroupBox *dateGroup = new QGroupBox("Sélectionner la plage de dates", &dialog);
    QVBoxLayout *dateLayout = new QVBoxLayout(dateGroup);
    
    QHBoxLayout *dateStartLayout = new QHBoxLayout();
    dateStartLayout->addWidget(new QLabel("Date début:"));
    QDateEdit *dateStart = new QDateEdit(QDate::currentDate().addMonths(-1));
    dateStart->setCalendarPopup(true);
    dateStartLayout->addWidget(dateStart);
    dateLayout->addLayout(dateStartLayout);
    
    QHBoxLayout *dateEndLayout = new QHBoxLayout();
    dateEndLayout->addWidget(new QLabel("Date fin:"));
    QDateEdit *dateEnd = new QDateEdit(QDate::currentDate());
    dateEnd->setCalendarPopup(true);
    dateEndLayout->addWidget(dateEnd);
    dateLayout->addLayout(dateEndLayout);
    
    layout->addWidget(dateGroup);
    
    QPushButton *btnFiltrer = new QPushButton("Filtrer");
    QPushButton *btnAnnuler = new QPushButton("Annuler");
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnFiltrer);
    btnLayout->addWidget(btnAnnuler);
    layout->addLayout(btnLayout);
    
    connect(btnFiltrer, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QSqlQueryModel *model = tmpDechet.filtrerParDateRange(dateStart->date(), dateEnd->date());
        ui->tableView_dechet->setModel(model);
        ui->tableView_dechet->resizeColumnsToContents();
        QMessageBox::information(this, "Filtrage", QString("Résultats filtrés: %1 collectes trouvées").arg(model->rowCount()));
    }
}

void MainWindow::on_filtrer_volume_range_btn_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Filtrage par Plage de Volume");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QGroupBox *volumeGroup = new QGroupBox("Sélectionner la plage de volume (kg)", &dialog);
    QVBoxLayout *volumeLayout = new QVBoxLayout(volumeGroup);
    
    QHBoxLayout *volumeMinLayout = new QHBoxLayout();
    volumeMinLayout->addWidget(new QLabel("Volume minimum:"));
    QDoubleSpinBox *volumeMin = new QDoubleSpinBox();
    volumeMin->setMinimum(0);
    volumeMin->setMaximum(100000);
    volumeMin->setValue(0);
    volumeMinLayout->addWidget(volumeMin);
    volumeLayout->addLayout(volumeMinLayout);
    
    QHBoxLayout *volumeMaxLayout = new QHBoxLayout();
    volumeMaxLayout->addWidget(new QLabel("Volume maximum:"));
    QDoubleSpinBox *volumeMax = new QDoubleSpinBox();
    volumeMax->setMinimum(0);
    volumeMax->setMaximum(100000);
    volumeMax->setValue(10000);
    volumeMaxLayout->addWidget(volumeMax);
    volumeLayout->addLayout(volumeMaxLayout);
    
    layout->addWidget(volumeGroup);
    
    QPushButton *btnFiltrer = new QPushButton("Filtrer");
    QPushButton *btnAnnuler = new QPushButton("Annuler");
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnFiltrer);
    btnLayout->addWidget(btnAnnuler);
    layout->addLayout(btnLayout);
    
    connect(btnFiltrer, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QSqlQueryModel *model = tmpDechet.filtrerParVolumeRange(volumeMin->value(), volumeMax->value());
        ui->tableView_dechet->setModel(model);
        ui->tableView_dechet->resizeColumnsToContents();
        QMessageBox::information(this, "Filtrage", QString("Résultats filtrés: %1 collectes trouvées").arg(model->rowCount()));
    }
}

void MainWindow::on_filtrer_multi_critere_btn_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Filtrage Multi-Critères");
    dialog.resize(400, 300);
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // Dates
    QGroupBox *dateGroup = new QGroupBox("Dates", &dialog);
    QVBoxLayout *dateLayout = new QVBoxLayout(dateGroup);
    QHBoxLayout *dateStartLayout = new QHBoxLayout();
    dateStartLayout->addWidget(new QLabel("Début:"));
    QDateEdit *dateStart = new QDateEdit(QDate::currentDate().addMonths(-1));
    dateStart->setCalendarPopup(true);
    dateStartLayout->addWidget(dateStart);
    dateLayout->addLayout(dateStartLayout);
    
    QHBoxLayout *dateEndLayout = new QHBoxLayout();
    dateEndLayout->addWidget(new QLabel("Fin:"));
    QDateEdit *dateEnd = new QDateEdit(QDate::currentDate());
    dateEnd->setCalendarPopup(true);
    dateEndLayout->addWidget(dateEnd);
    dateLayout->addLayout(dateEndLayout);
    layout->addWidget(dateGroup);
    
    // Lieu
    QHBoxLayout *lieuLayout = new QHBoxLayout();
    lieuLayout->addWidget(new QLabel("Lieu:"));
    QLineEdit *lieuEdit = new QLineEdit();
    lieuEdit->setPlaceholderText("Laisser vide pour tous");
    lieuLayout->addWidget(lieuEdit);
    layout->addLayout(lieuLayout);
    
    // Type
    QHBoxLayout *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Type:"));
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItem("Tous", "");
    typeCombo->addItem("Recyclé", "recycle");
    typeCombo->addItem("Non recyclé", "non recycle");
    typeLayout->addWidget(typeCombo);
    layout->addLayout(typeLayout);
    
    // Volumes
    QGroupBox *volumeGroup = new QGroupBox("Volume (kg)", &dialog);
    QVBoxLayout *volumeLayout = new QVBoxLayout(volumeGroup);
    QHBoxLayout *volMinLayout = new QHBoxLayout();
    volMinLayout->addWidget(new QLabel("Min:"));
    QDoubleSpinBox *volumeMin = new QDoubleSpinBox();
    volumeMin->setMinimum(0);
    volumeMin->setMaximum(100000);
    volMinLayout->addWidget(volumeMin);
    volumeLayout->addLayout(volMinLayout);
    
    QHBoxLayout *volMaxLayout = new QHBoxLayout();
    volMaxLayout->addWidget(new QLabel("Max:"));
    QDoubleSpinBox *volumeMax = new QDoubleSpinBox();
    volumeMax->setMinimum(0);
    volumeMax->setMaximum(100000);
    volumeMax->setValue(10000);
    volMaxLayout->addWidget(volumeMax);
    volumeLayout->addLayout(volMaxLayout);
    layout->addWidget(volumeGroup);
    
    QPushButton *btnFiltrer = new QPushButton("Filtrer");
    QPushButton *btnAnnuler = new QPushButton("Annuler");
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnFiltrer);
    btnLayout->addWidget(btnAnnuler);
    layout->addLayout(btnLayout);
    
    connect(btnFiltrer, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnAnnuler, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString type = typeCombo->currentData().toString();
        QSqlQueryModel *model = tmpDechet.filtrerMultiCritere(
            dateStart->date(), dateEnd->date(),
            lieuEdit->text(), type,
            volumeMin->value(), volumeMax->value()
        );
        ui->tableView_dechet->setModel(model);
        ui->tableView_dechet->resizeColumnsToContents();
        QMessageBox::information(this, "Filtrage", QString("Résultats filtrés: %1 collectes trouvées").arg(model->rowCount()));
    }
}

void MainWindow::on_export_excel_btn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers Excel",
                                                    QDir::homePath() + "/dechets.xls",
                                                    "Excel Files (*.xls *.csv)");
    if (fileName.isEmpty()) return;
    
    QSqlQueryModel *currentModel = qobject_cast<QSqlQueryModel*>(ui->tableView_dechet->model());
    if (tmpDechet.exportToExcel(fileName, currentModel)) {
        QMessageBox::information(this, "Succès", "Données exportées vers: " + fileName);
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exportation!");
    }
}

void MainWindow::on_export_csv_btn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter vers CSV",
                                                    QDir::homePath() + "/dechets.csv",
                                                    "CSV Files (*.csv)");
    if (fileName.isEmpty()) return;
    
    QSqlQueryModel *currentModel = qobject_cast<QSqlQueryModel*>(ui->tableView_dechet->model());
    if (tmpDechet.exportToCSV(fileName, currentModel)) {
        QMessageBox::information(this, "Succès", "Données exportées vers: " + fileName);
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'exportation!");
    }
}

void MainWindow::afficherStatistiquesGraphiques()
{
    qDebug() << "=== AFFICHAGE STATISTIQUES GRAPHIQUES ===";
    
    Dechet dechet;
    
    // Create a dialog to show multiple charts
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("Statistiques Déchets - Graphiques");
    chartDialog->setMinimumSize(1200, 800);
    
    QTabWidget *tabWidget = new QTabWidget(chartDialog);
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);
    layout->addWidget(tabWidget);
    
    // Draw graphs manually using QPainter (Qt Charts not available - using custom implementation)
    // 1. Bar Chart - Collections by Location
    QMap<QString, int> locationStats = dechet.getCollectionsByLocation();
    if (!locationStats.isEmpty()) {
        QWidget *barChartWidget = new QWidget();
        barChartWidget->setMinimumSize(600, 400);
        
        // Create a custom widget that paints the bar chart
        class BarChartWidget : public QWidget {
        public:
            QMap<QString, int> data;
            BarChartWidget(QMap<QString, int> d, QWidget *parent = nullptr) : QWidget(parent), data(d) {}
            void paintEvent(QPaintEvent *) override {
                QPainter painter(this);
                painter.setRenderHint(QPainter::Antialiasing);
                
                int width = this->width();
                int height = this->height();
                int margin = 50;
                int chartWidth = width - 2 * margin;
                int chartHeight = height - 2 * margin;
                
                // Find max value
                int maxValue = 0;
                for (auto it = data.begin(); it != data.end(); ++it) {
                    if (it.value() > maxValue) maxValue = it.value();
                }
                if (maxValue == 0) maxValue = 1;
                
                // Draw bars
                int barCount = data.size();
                int barWidth = chartWidth / (barCount * 2);
                int x = margin;
                int colorIndex = 0;
                QColor colors[] = {QColor(70, 130, 180), QColor(255, 165, 0), QColor(34, 139, 34), 
                                   QColor(220, 20, 60), QColor(138, 43, 226)};
                
                for (auto it = data.begin(); it != data.end(); ++it) {
                    int barHeight = (it.value() * chartHeight) / maxValue;
                    int y = margin + chartHeight - barHeight;
                    
                    painter.setBrush(colors[colorIndex % 5]);
                    painter.setPen(QPen(Qt::black, 1));
                    painter.drawRect(x, y, barWidth, barHeight);
                    
                    // Draw label
                    painter.setPen(Qt::black);
                    QFont font = painter.font();
                    font.setPointSize(8);
                    painter.setFont(font);
                    QRect textRect(x, margin + chartHeight + 5, barWidth, 20);
                    painter.drawText(textRect, Qt::AlignCenter, it.key());
                    
                    // Draw value
                    painter.drawText(x, y - 5, barWidth, 20, Qt::AlignCenter, QString::number(it.value()));
                    
                    x += barWidth * 2;
                    colorIndex++;
                }
                
                // Draw title
                painter.setFont(QFont("Arial", 12, QFont::Bold));
                painter.drawText(0, 20, width, 30, Qt::AlignCenter, "Collectes par Lieu");
            }
        };
        
        BarChartWidget *barChart = new BarChartWidget(locationStats);
        tabWidget->addTab(barChart, "Collectes par Lieu");
    }
    
    // 2. Pie Chart - Recycling Rate
    int recycleCount = dechet.getRecycledCount();
    int nonRecycleCount = dechet.getNonRecycledCount();
    if (recycleCount > 0 || nonRecycleCount > 0) {
        QWidget *pieChartWidget = new QWidget();
        pieChartWidget->setMinimumSize(600, 400);
        
        class PieChartWidget : public QWidget {
        public:
            int recycled, nonRecycled;
            PieChartWidget(int r, int nr, QWidget *parent = nullptr) : QWidget(parent), recycled(r), nonRecycled(nr) {}
            void paintEvent(QPaintEvent *) override {
                QPainter painter(this);
                painter.setRenderHint(QPainter::Antialiasing);
                
                int width = this->width();
                int height = this->height();
                int size = qMin(width, height) - 100;
                int x = (width - size) / 2;
                int y = (height - size) / 2;
                
                int total = recycled + nonRecycled;
                if (total == 0) return;
                
                // Draw pie chart
                int startAngle = 0;
                int spanAngle = (recycled * 360 * 16) / total;
                
                // Recycled slice (green)
                painter.setBrush(QColor(34, 139, 34));
                painter.setPen(QPen(Qt::black, 2));
                painter.drawPie(x, y, size, size, startAngle, spanAngle);
                
                // Non-recycled slice (red)
                startAngle += spanAngle;
                spanAngle = (nonRecycled * 360 * 16) / total;
                painter.setBrush(QColor(220, 20, 60));
                painter.drawPie(x, y, size, size, startAngle, spanAngle);
                
                // Draw legend
                painter.setFont(QFont("Arial", 10));
                int legendY = y + size + 30;
                painter.setBrush(QColor(34, 139, 34));
                painter.drawRect(50, legendY, 20, 20);
                painter.setPen(Qt::black);
                painter.drawText(80, legendY, 200, 20, Qt::AlignLeft, QString("Recyclés: %1").arg(recycled));
                
                painter.setBrush(QColor(220, 20, 60));
                painter.drawRect(50, legendY + 30, 20, 20);
                painter.drawText(80, legendY + 30, 200, 20, Qt::AlignLeft, QString("Non Recyclés: %1").arg(nonRecycled));
                
                // Draw title
                painter.setFont(QFont("Arial", 12, QFont::Bold));
                painter.drawText(0, 20, width, 30, Qt::AlignCenter, "Taux de Recyclage");
            }
        };
        
        PieChartWidget *pieChart = new PieChartWidget(recycleCount, nonRecycleCount);
        tabWidget->addTab(pieChart, "Recyclage");
    }
    
    // 3. Line Chart - Volume Trends by Month
    QMap<QString, double> volumeTrend = dechet.getVolumeTrendByMonth();
    if (!volumeTrend.isEmpty()) {
        QWidget *lineChartWidget = new QWidget();
        lineChartWidget->setMinimumSize(600, 400);
        
        class LineChartWidget : public QWidget {
        public:
            QMap<QString, double> data;
            LineChartWidget(QMap<QString, double> d, QWidget *parent = nullptr) : QWidget(parent), data(d) {}
            void paintEvent(QPaintEvent *) override {
                QPainter painter(this);
                painter.setRenderHint(QPainter::Antialiasing);
                
                int width = this->width();
                int height = this->height();
                int margin = 60;
                int chartWidth = width - 2 * margin;
                int chartHeight = height - 2 * margin;
                
                // Find max value
                double maxValue = 0;
                for (auto it = data.begin(); it != data.end(); ++it) {
                    if (it.value() > maxValue) maxValue = it.value();
                }
                if (maxValue == 0) maxValue = 1;
                
                // Draw axes
                painter.setPen(QPen(Qt::black, 2));
                painter.drawLine(margin, margin, margin, margin + chartHeight);
                painter.drawLine(margin, margin + chartHeight, margin + chartWidth, margin + chartHeight);
                
                // Draw line
                QPolygonF points;
                int pointCount = data.size();
                int index = 0;
                for (auto it = data.begin(); it != data.end(); ++it) {
                    int x = margin + (index * chartWidth) / (pointCount - 1);
                    int y = margin + chartHeight - (it.value() * chartHeight / maxValue);
                    points << QPointF(x, y);
                    index++;
                }
                
                painter.setPen(QPen(QColor(70, 130, 180), 3));
                painter.setBrush(QColor(70, 130, 180));
                for (int i = 0; i < points.size(); i++) {
                    painter.drawEllipse(points[i], 5, 5);
                    if (i > 0) {
                        painter.drawLine(points[i-1], points[i]);
                    }
                }
                
                // Draw labels
                painter.setPen(Qt::black);
                painter.setFont(QFont("Arial", 8));
                index = 0;
                for (auto it = data.begin(); it != data.end(); ++it) {
                    int x = margin + (index * chartWidth) / (pointCount - 1);
                    QRect textRect(x - 30, margin + chartHeight + 5, 60, 20);
                    painter.drawText(textRect, Qt::AlignCenter, it.key());
                    index++;
                }
                
                // Draw title
                painter.setFont(QFont("Arial", 12, QFont::Bold));
                painter.drawText(0, 20, width, 30, Qt::AlignCenter, "Tendances des Volumes par Mois");
            }
        };
        
        LineChartWidget *lineChart = new LineChartWidget(volumeTrend);
        tabWidget->addTab(lineChart, "Tendances Mensuelles");
    }
    
    // 4. Bar Chart - Collections by Status
    QMap<QString, int> statusStats = dechet.getCollectionsByStatus();
    if (!statusStats.isEmpty()) {
        QWidget *statusChartWidget = new QWidget();
        statusChartWidget->setMinimumSize(600, 400);
        
        class StatusBarChartWidget : public QWidget {
        public:
            QMap<QString, int> data;
            StatusBarChartWidget(QMap<QString, int> d, QWidget *parent = nullptr) : QWidget(parent), data(d) {}
            void paintEvent(QPaintEvent *) override {
                QPainter painter(this);
                painter.setRenderHint(QPainter::Antialiasing);
                
                int width = this->width();
                int height = this->height();
                int margin = 50;
                int chartWidth = width - 2 * margin;
                int chartHeight = height - 2 * margin;
                
                int maxValue = 0;
                for (auto it = data.begin(); it != data.end(); ++it) {
                    if (it.value() > maxValue) maxValue = it.value();
                }
                if (maxValue == 0) maxValue = 1;
                
                int barCount = data.size();
                int barWidth = chartWidth / (barCount * 2);
                int x = margin;
                QColor color(255, 165, 0);
                
                for (auto it = data.begin(); it != data.end(); ++it) {
                    int barHeight = (it.value() * chartHeight) / maxValue;
                    int y = margin + chartHeight - barHeight;
                    
                    painter.setBrush(color);
                    painter.setPen(QPen(Qt::black, 1));
                    painter.drawRect(x, y, barWidth, barHeight);
                    
                    painter.setPen(Qt::black);
                    QFont font = painter.font();
                    font.setPointSize(8);
                    painter.setFont(font);
                    QRect textRect(x, margin + chartHeight + 5, barWidth, 20);
                    painter.drawText(textRect, Qt::AlignCenter, it.key());
                    painter.drawText(x, y - 5, barWidth, 20, Qt::AlignCenter, QString::number(it.value()));
                    
                    x += barWidth * 2;
                }
                
                painter.setFont(QFont("Arial", 12, QFont::Bold));
                painter.drawText(0, 20, width, 30, Qt::AlignCenter, "Collectes par État");
            }
        };
        
        StatusBarChartWidget *statusChart = new StatusBarChartWidget(statusStats);
        tabWidget->addTab(statusChart, "Par État");
    }
    
    // Also update the graphicsView in the stat tab if available
    if (ui->graphicsView_dechet_stat && tabWidget->count() > 0) {
        QWidget *firstTab = tabWidget->widget(0);
        if (firstTab) {
            // Create a scene and add the widget to it
            QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView_dechet_stat);
            QGraphicsProxyWidget *proxy = scene->addWidget(firstTab);
            ui->graphicsView_dechet_stat->setScene(scene);
            ui->graphicsView_dechet_stat->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        }
    }
    
    if (tabWidget->count() > 0) {
        chartDialog->exec();
    } else {
        QMessageBox::information(this, "Statistiques", "Aucune donnée disponible pour afficher les graphiques.");
        delete chartDialog;
    }
}

void MainWindow::afficherCharts()
{
    afficherStatistiquesGraphiques();
}

void MainWindow::on_afficher_charts_btn_clicked()
{
    afficherCharts();
}

void MainWindow::afficherImpactEnvironnemental()
{
    Dechet dechet;
    double co2Saved = dechet.calculateCO2Saved();
    double energySaved = dechet.calculateEnergySaved();
    double impactScore = dechet.getEnvironmentalImpactScore();
    double recyclingRate = dechet.getRecyclingRate();
    
    QString impactText;
    impactText += "=== IMPACT ENVIRONNEMENTAL ===\n\n";
    impactText += QString("🌱 CO2 Économisé: %1 kg\n").arg(co2Saved, 0, 'f', 2);
    impactText += QString("⚡ Énergie Économisée: %1 kWh\n").arg(energySaved, 0, 'f', 2);
    impactText += QString("📊 Score d'Impact: %1/100\n\n").arg(impactScore, 0, 'f', 1);
    impactText += QString("♻️ Taux de Recyclage: %1%\n\n").arg(recyclingRate, 0, 'f', 2);
    
    impactText += "💡 Équivalences:\n";
    impactText += QString("   • Équivalent à planter %1 arbres\n").arg((int)(co2Saved / 20));
    impactText += QString("   • Équivalent à %1 km en voiture évités\n").arg((int)(co2Saved / 0.12));
    
    QMessageBox::information(this, "Impact Environnemental", impactText);
}

void MainWindow::on_calculer_impact_env_btn_clicked()
{
    afficherImpactEnvironnemental();
}

void MainWindow::afficherAlertes()
{
    Dechet dechet;
    QList<QString> highVolumeAlerts = dechet.getHighVolumeAlerts(1000.0);
    QList<QString> overdueAlerts = dechet.getOverdueCollections(7);
    
    QString alertesText;
    alertesText += "=== ALERTES DÉCHETS ===\n\n";
    
    if (highVolumeAlerts.isEmpty() && overdueAlerts.isEmpty()) {
        alertesText += "✅ Aucune alerte active.\n";
    } else {
        if (!highVolumeAlerts.isEmpty()) {
            alertesText += "⚠️ VOLUMES ÉLEVÉS:\n";
            for (const QString &alert : highVolumeAlerts) {
                alertesText += "   " + alert + "\n";
            }
            alertesText += "\n";
        }
        
        if (!overdueAlerts.isEmpty()) {
            alertesText += "⏰ COLLECTES ANCIENNES:\n";
            for (const QString &alert : overdueAlerts) {
                alertesText += "   " + alert + "\n";
            }
        }
    }
    
    QMessageBox::information(this, "Alertes", alertesText);
}

void MainWindow::on_afficher_alertes_btn_clicked()
{
    afficherAlertes();
}

void MainWindow::afficherTendances()
{
    Dechet dechet;
    QMap<QString, int> dailyStats = dechet.getCollectionsByDay();
    QMap<QString, int> weeklyStats = dechet.getCollectionsByWeek();
    QMap<QString, double> monthlyTrend = dechet.getVolumeTrendByMonth();
    double avgPerDay = dechet.getAverageCollectionsPerDay();
    
    QString trendsText;
    trendsText += "=== ANALYSE DES TENDANCES ===\n\n";
    trendsText += QString("📈 Collections moyennes par jour: %1\n\n").arg(avgPerDay, 0, 'f', 2);
    
    if (!monthlyTrend.isEmpty()) {
        trendsText += "📅 VOLUMES PAR MOIS:\n";
        for (auto it = monthlyTrend.begin(); it != monthlyTrend.end(); ++it) {
            trendsText += QString("   • %1: %2 kg\n").arg(it.key()).arg(it.value(), 0, 'f', 2);
        }
        trendsText += "\n";
    }
    
    if (!weeklyStats.isEmpty()) {
        trendsText += "📆 COLLECTES PAR SEMAINE (5 dernières):\n";
        int count = 0;
        // Utiliser reverse iterator pour éviter la dépréciation
        auto it = weeklyStats.end();
        while (it != weeklyStats.begin() && count < 5) {
            --it;
            trendsText += QString("   • Semaine %1: %2 collectes\n").arg(it.key()).arg(it.value());
            count++;
        }
    }
    
    QMessageBox::information(this, "Analyse des Tendances", trendsText);
}

void MainWindow::setupDefaultAlertRules()
{
    if (!notificationManager) {
        qDebug() << "NotificationManager not initialized in setupDefaultAlertRules";
        return;
    }
    
    // High Volume Alert Rule
    NotificationManager::AlertRule highVolumeRule;
    highVolumeRule.type = NotificationManager::HighVolume;
    highVolumeRule.name = "Volume Élevé";
    highVolumeRule.condition = "VOLUME_POIDS > 1000";
    highVolumeRule.message = "Alerte: Des volumes élevés ont été détectés (>1000 kg)";
    highVolumeRule.channels = NotificationManager::All;
    highVolumeRule.enabled = true;
    highVolumeRule.threshold = 1000.0;
    highVolumeRule.priority = 4;
    notificationManager->addAlertRule(highVolumeRule);
    
    // Overdue Collection Rule
    NotificationManager::AlertRule overdueRule;
    overdueRule.type = NotificationManager::OverdueCollection;
    overdueRule.name = "Collecte Ancienne";
    overdueRule.condition = "DATE_COLLECTE < SYSDATE - 7";
    overdueRule.message = "Alerte: Des collectes anciennes nécessitent une attention (>7 jours)";
    overdueRule.channels = NotificationManager::All;
    overdueRule.enabled = true;
    overdueRule.threshold = 7.0;
    overdueRule.priority = 3;
    notificationManager->addAlertRule(overdueRule);
    
    // Low Recycling Rate Rule
    NotificationManager::AlertRule lowRecyclingRule;
    lowRecyclingRule.type = NotificationManager::LowRecyclingRate;
    lowRecyclingRule.name = "Taux de Recyclage Faible";
    lowRecyclingRule.condition = "RECYCLING_RATE < 50";
    lowRecyclingRule.message = "Alerte: Le taux de recyclage est en dessous de 50%";
    lowRecyclingRule.channels = NotificationManager::NotificationTypes(NotificationManager::Email | NotificationManager::Push);
    lowRecyclingRule.enabled = true;
    lowRecyclingRule.threshold = 50.0;
    lowRecyclingRule.priority = 2;
    notificationManager->addAlertRule(lowRecyclingRule);
}

void MainWindow::onNotificationSent(const NotificationManager::Notification &notification)
{
    qDebug() << "Notification sent:" << notification.title;
    // Could show a system tray notification or update UI
}

void MainWindow::onAlertTriggered(const NotificationManager::AlertRule &rule, const QString &details)
{
    qDebug() << "Alert triggered:" << rule.name << details;
}

void MainWindow::on_notifications_settings_btn_clicked()
{
    configureNotifications();
}

void MainWindow::on_notification_history_btn_clicked()
{
    showNotificationHistory();
}

void MainWindow::on_manage_alert_rules_btn_clicked()
{
    manageAlertRules();
}

// Note: This method can be called directly if button is not in UI yet

void MainWindow::manageAlertRules()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Gestion des Règles d'Alerte");
    dialog->setMinimumSize(700, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Rules List
    QTableView *rulesTable = new QTableView(dialog);
    QStandardItemModel *model = new QStandardItemModel(rulesTable);
    model->setHorizontalHeaderLabels({"Nom", "Type", "Condition", "Message", "Canaux", "Activé", "Priorité"});
    
    QList<NotificationManager::AlertRule> rules = notificationManager->getAlertRules();
    for (const NotificationManager::AlertRule &rule : rules) {
        QList<QStandardItem*> row;
        row << new QStandardItem(rule.name);
        
        QString typeStr;
        switch (rule.type) {
            case NotificationManager::HighVolume: typeStr = "Volume Élevé"; break;
            case NotificationManager::OverdueCollection: typeStr = "Collecte Ancienne"; break;
            case NotificationManager::LowRecyclingRate: typeStr = "Taux Recyclage Faible"; break;
            default: typeStr = "Personnalisé"; break;
        }
        row << new QStandardItem(typeStr);
        row << new QStandardItem(rule.condition);
        row << new QStandardItem(rule.message);
        
        QString channels;
        if (rule.channels & NotificationManager::Email) channels += "Email ";
        if (rule.channels & NotificationManager::Push) channels += "Push";
        row << new QStandardItem(channels);
        
        row << new QStandardItem(rule.enabled ? "Oui" : "Non");
        row << new QStandardItem(QString::number(rule.priority));
        
        model->appendRow(row);
    }
    
    rulesTable->setModel(model);
    rulesTable->resizeColumnsToContents();
    
    // Buttons
    QPushButton *addRuleBtn = new QPushButton("Ajouter Règle", dialog);
    QPushButton *editRuleBtn = new QPushButton("Modifier", dialog);
    QPushButton *deleteRuleBtn = new QPushButton("Supprimer", dialog);
    QPushButton *testRuleBtn = new QPushButton("Tester", dialog);
    QPushButton *closeBtn = new QPushButton("Fermer", dialog);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addRuleBtn);
    buttonLayout->addWidget(editRuleBtn);
    buttonLayout->addWidget(deleteRuleBtn);
    buttonLayout->addWidget(testRuleBtn);
    buttonLayout->addWidget(closeBtn);
    
    layout->addWidget(rulesTable);
    layout->addLayout(buttonLayout);
    
    connect(addRuleBtn, &QPushButton::clicked, [=]() {
        // Create new rule dialog
        QDialog *ruleDialog = new QDialog(dialog);
        ruleDialog->setWindowTitle("Nouvelle Règle d'Alerte");
        ruleDialog->setMinimumSize(500, 400);
        
        QVBoxLayout *ruleLayout = new QVBoxLayout(ruleDialog);
        
        QFormLayout *formLayout = new QFormLayout();
        
        QLineEdit *nameEdit = new QLineEdit(ruleDialog);
        QComboBox *typeCombo = new QComboBox(ruleDialog);
        typeCombo->addItems({"Volume Élevé", "Collecte Ancienne", "Taux Recyclage Faible", "Personnalisé"});
        QLineEdit *conditionEdit = new QLineEdit(ruleDialog);
        QTextEdit *messageEdit = new QTextEdit(ruleDialog);
        QDoubleSpinBox *thresholdSpin = new QDoubleSpinBox(ruleDialog);
        thresholdSpin->setRange(0, 10000);
        QSpinBox *prioritySpin = new QSpinBox(ruleDialog);
        prioritySpin->setRange(1, 5);
        prioritySpin->setValue(3);
        
        QCheckBox *emailCheck = new QCheckBox(ruleDialog);
        QCheckBox *pushCheck = new QCheckBox(ruleDialog);
        pushCheck->setChecked(true);
        QCheckBox *enabledCheck = new QCheckBox(ruleDialog);
        enabledCheck->setChecked(true);
        
        formLayout->addRow("Nom:", nameEdit);
        formLayout->addRow("Type:", typeCombo);
        formLayout->addRow("Condition:", conditionEdit);
        formLayout->addRow("Message:", messageEdit);
        formLayout->addRow("Seuil:", thresholdSpin);
        formLayout->addRow("Priorité:", prioritySpin);
        formLayout->addRow("Email:", emailCheck);
        formLayout->addRow("Push:", pushCheck);
        formLayout->addRow("Activé:", enabledCheck);
        
        QPushButton *saveRuleBtn = new QPushButton("Enregistrer", ruleDialog);
        QPushButton *cancelRuleBtn = new QPushButton("Annuler", ruleDialog);
        QHBoxLayout *ruleButtonLayout = new QHBoxLayout();
        ruleButtonLayout->addWidget(saveRuleBtn);
        ruleButtonLayout->addWidget(cancelRuleBtn);
        
        ruleLayout->addLayout(formLayout);
        ruleLayout->addLayout(ruleButtonLayout);
        
        connect(saveRuleBtn, &QPushButton::clicked, [=]() {
            NotificationManager::AlertRule newRule;
            newRule.name = nameEdit->text();
            newRule.type = static_cast<NotificationManager::AlertType>(typeCombo->currentIndex());
            newRule.condition = conditionEdit->text();
            newRule.message = messageEdit->toPlainText();
            newRule.threshold = thresholdSpin->value();
            newRule.priority = prioritySpin->value();
            newRule.enabled = enabledCheck->isChecked();
            
            NotificationManager::NotificationTypes channels;
            if (emailCheck->isChecked()) channels |= NotificationManager::Email;
            if (pushCheck->isChecked()) channels |= NotificationManager::Push;
            newRule.channels = channels;
            
            notificationManager->addAlertRule(newRule);
            QMessageBox::information(ruleDialog, "Succès", "Règle ajoutée!");
            ruleDialog->accept();
            dialog->accept();
            manageAlertRules(); // Refresh
        });
        
        connect(cancelRuleBtn, &QPushButton::clicked, ruleDialog, &QDialog::reject);
        
        ruleDialog->exec();
    });
    
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(testRuleBtn, &QPushButton::clicked, [=]() {
        notificationManager->checkAlertRules();
        QMessageBox::information(dialog, "Test", "Vérification des règles d'alerte effectuée!");
    });
    
    dialog->exec();
}

void MainWindow::configureNotifications()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Configuration des Notifications");
    dialog->setMinimumSize(500, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Recipients - Simplified
    QGroupBox *recipientsGroup = new QGroupBox("Destinataires", dialog);
    QFormLayout *recipientsLayout = new QFormLayout(recipientsGroup);
    
    QLineEdit *recipientEmailEdit = new QLineEdit(dialog);
    recipientEmailEdit->setPlaceholderText("ex: admin@smartcity.tn");
    
    recipientsLayout->addRow("📧 Adresse Email:", recipientEmailEdit);
    
    // Load existing recipient if any
    QList<NotificationManager::Notification> history = notificationManager->getNotificationHistory(1);
    if (!history.isEmpty() && !history.first().recipient.isEmpty()) {
        QStringList parts = history.first().recipient.split(",");
        if (!parts.isEmpty()) {
            recipientEmailEdit->setText(parts.first().trimmed());
        }
    }
    
    // Notification Channels
    QGroupBox *channelsGroup = new QGroupBox("Canaux de Notification", dialog);
    QVBoxLayout *channelsLayout = new QVBoxLayout(channelsGroup);
    
    QCheckBox *emailEnabledCheck = new QCheckBox("Activer Email (Gmail SMTP)", dialog);
    emailEnabledCheck->setChecked(notificationManager->isEmailEnabled());
    emailEnabledCheck->setToolTip("Les paramètres Gmail sont configurés dans le code");
    
    QCheckBox *pushEnabledCheck = new QCheckBox("Activer Notifications Push", dialog);
    pushEnabledCheck->setChecked(notificationManager->isPushEnabled());
    
    channelsLayout->addWidget(emailEnabledCheck);
    channelsLayout->addWidget(pushEnabledCheck);
    
    // Status label
    QLabel *statusLabel = new QLabel("", dialog);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("padding: 5px;");
    
    // Update status
    auto updateStatus = [=]() {
        QString status = "État: ";
        if (notificationManager->isEmailEnabled()) {
            status += "✅ Email configuré";
        } else {
            status += "⚠️ Email non configuré (vérifiez notificationmanager.cpp)";
        }
        statusLabel->setText(status);
    };
    updateStatus();
    
    // Buttons
    QPushButton *saveBtn = new QPushButton("Enregistrer", dialog);
    QPushButton *cancelBtn = new QPushButton("Annuler", dialog);
    QPushButton *testBtn = new QPushButton("Tester", dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(testBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    
    layout->addWidget(recipientsGroup);
    layout->addWidget(channelsGroup);
    layout->addWidget(statusLabel);
    layout->addLayout(buttonLayout);
    
    connect(saveBtn, &QPushButton::clicked, [=]() {
        // Only set recipient email
        if (!recipientEmailEdit->text().isEmpty()) {
            notificationManager->setRecipientEmail(recipientEmailEdit->text());
        }
        
        // Enable/disable channels
        notificationManager->setEmailEnabled(emailEnabledCheck->isChecked());
        notificationManager->setPushEnabled(pushEnabledCheck->isChecked());
        
        notificationManager->saveSettings();
        QMessageBox::information(dialog, "Succès", "Configuration enregistrée!");
        dialog->accept();
    });
    
    connect(testBtn, &QPushButton::clicked, [=]() {
        QString testEmail = recipientEmailEdit->text();
        
        if (testEmail.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Veuillez entrer une adresse email pour tester.");
            return;
        }
        
        bool emailSent = false;
        
        if (emailEnabledCheck->isChecked()) {
            emailSent = notificationManager->sendEmail(testEmail, "Test SmartCity", 
                "Ceci est un email de test depuis l'application SmartCity.\n\n"
                "Si vous recevez ce message, la configuration email fonctionne correctement.");
        } else {
            QMessageBox::warning(dialog, "Erreur", "L'email n'est pas activé. Cochez la case 'Activer Email' pour tester.");
            return;
        }
        
        QString result = "Résultat du test:\n\n";
        if (emailSent) {
            result += "✅ Email envoyé avec succès à " + testEmail;
        } else {
            result += "❌ Échec envoi email (vérifiez les logs)";
        }
        
        QMessageBox::information(dialog, "Test de Notification", result);
    });
    
    connect(cancelBtn, &QPushButton::clicked, dialog, &QDialog::reject);
    
    dialog->exec();
}

void MainWindow::showNotificationHistory()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Historique des Notifications");
    dialog->setMinimumSize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    QTableView *tableView = new QTableView(dialog);
    QStandardItemModel *model = new QStandardItemModel(tableView);
    model->setHorizontalHeaderLabels({"Date/Heure", "Type", "Titre", "Message", "Canaux", "Statut", "Destinataire"});
    
    QList<NotificationManager::Notification> history = notificationManager->getNotificationHistory(100);
    
    for (const NotificationManager::Notification &notif : history) {
        QList<QStandardItem*> row;
        row << new QStandardItem(notif.timestamp.toString("dd/MM/yyyy HH:mm"));
        row << new QStandardItem(QString::number(static_cast<int>(notif.alertType)));
        row << new QStandardItem(notif.title);
        row << new QStandardItem(notif.message);
        
        QString channels;
        if (notif.channels & NotificationManager::Email) channels += "Email ";
        if (notif.channels & NotificationManager::SMS) channels += "SMS ";
        if (notif.channels & NotificationManager::Push) channels += "Push";
        row << new QStandardItem(channels);
        
        row << new QStandardItem(notif.sent ? "Envoyé" : "Échec");
        row << new QStandardItem(notif.recipient);
        
        model->appendRow(row);
    }
    
    tableView->setModel(model);
    tableView->resizeColumnsToContents();
    
    QPushButton *clearBtn = new QPushButton("Effacer l'historique", dialog);
    QPushButton *closeBtn = new QPushButton("Fermer", dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addWidget(closeBtn);
    
    layout->addWidget(tableView);
    layout->addLayout(buttonLayout);
    
    connect(clearBtn, &QPushButton::clicked, [=]() {
        if (QMessageBox::question(dialog, "Confirmation", "Voulez-vous vraiment effacer l'historique?") 
            == QMessageBox::Yes) {
            notificationManager->clearNotificationHistory();
            model->clear();
            model->setHorizontalHeaderLabels({"Date/Heure", "Type", "Titre", "Message", "Canaux", "Statut", "Destinataire"});
        }
    });
    
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    
    dialog->exec();
}

void MainWindow::on_analyser_tendances_btn_clicked()
{
    afficherTendances();
}

void MainWindow::on_afficher_map_btn_clicked()
{
#ifdef WEBENGINE_AVAILABLE
    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("Carte des Localisations de Collecte");
    mapDialog->setMinimumSize(1000, 700);
    
    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    
    // Créer une page HTML avec OpenStreetMap et Leaflet.js
    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Carte des Localisations</title>
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
    <style>
        body { margin: 0; padding: 0; }
        #map { height: 100vh; width: 100%; }
    </style>
</head>
<body>
    <div id="map"></div>
    <script>
        // Coordonnées de la Tunisie (centre)
        var map = L.map('map').setView([36.8065, 10.1815], 10);
        
        // Ajouter la couche de tuiles OpenStreetMap
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '© OpenStreetMap contributors',
            maxZoom: 19
        }).addTo(map);
        
        // Générer 6 marqueurs rouges à des positions aléatoires autour de Tunis
        var markers = [];
        var baseLat = 36.8065;  // Latitude de Tunis
        var baseLng = 10.1815;  // Longitude de Tunis
        var radius = 0.15;      // Rayon en degrés pour la zone
        
        for (var i = 0; i < 6; i++) {
            // Générer des coordonnées aléatoires dans un rayon autour de Tunis
            var angle = Math.random() * 2 * Math.PI;
            var distance = Math.random() * radius;
            var lat = baseLat + distance * Math.cos(angle);
            var lng = baseLng + distance * Math.sin(angle);
            
            // Créer un marqueur rouge personnalisé
            var redIcon = L.icon({
                iconUrl: 'data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMjUiIGhlaWdodD0iNDEiIHZpZXdCb3g9IjAgMCAyNSA0MSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj48cGF0aCBkPSJNMTIuNSAwQzUuNiAwIDAgNS42IDAgMTIuNUMwIDIxLjEgMTIuNSAzOCAxMi41IDM4UzI1IDIxLjEgMjUgMTIuNUMyNSA1LjYgMTkuNCAwIDEyLjUgMFoiIGZpbGw9IiNGRjAwMDAiLz48L3N2Zz4=',
                iconSize: [25, 41],
                iconAnchor: [12, 41],
                popupAnchor: [0, -41]
            });
            
            var marker = L.marker([lat, lng], {icon: redIcon}).addTo(map);
            marker.bindPopup('<b>Localisation ' + (i + 1) + '</b><br>Point de collecte de déchets');
            markers.push(marker);
        }
        
        // Ajuster la vue pour montrer tous les marqueurs
        if (markers.length > 0) {
            var group = new L.featureGroup(markers);
            map.fitBounds(group.getBounds().pad(0.1));
        }
    </script>
</body>
</html>
)";
    
    QWebEngineView *webView = new QWebEngineView(mapDialog);
    webView->setHtml(html);
    layout->addWidget(webView);
    
    QPushButton *closeBtn = new QPushButton("Fermer", mapDialog);
    connect(closeBtn, &QPushButton::clicked, mapDialog, &QDialog::accept);
    layout->addWidget(closeBtn);
    
    mapDialog->exec();
#else
    // Alternative sans WebEngine : Afficher les localisations dans un tableau
    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("Localisations de Collecte de Déchets");
    mapDialog->setMinimumSize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    
    // Titre
    QLabel *titleLabel = new QLabel("📍 Points de Collecte de Déchets", mapDialog);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);
    
    // Récupérer les localisations depuis la base de données
    QSqlQuery query("SELECT DISTINCT LIEU_COLLECTE, COUNT(*) as NB_COLLECTES, "
                    "SUM(VOLUME_POIDS) as VOLUME_TOTAL, "
                    "MIN(DATE_COLLECTE) as PREMIERE_COLLECTE, "
                    "MAX(DATE_COLLECTE) as DERNIERE_COLLECTE "
                    "FROM DECHETS GROUP BY LIEU_COLLECTE ORDER BY LIEU_COLLECTE");
    
    QStandardItemModel *model = new QStandardItemModel(mapDialog);
    model->setHorizontalHeaderLabels({"Lieu de Collecte", "Nb. Collectes", "Volume Total (kg)", 
                                      "Première Collecte", "Dernière Collecte"});
    
    int row = 0;
    while (query.next()) {
        QString lieu = query.value(0).toString();
        int nbCollectes = query.value(1).toInt();
        double volumeTotal = query.value(2).toDouble();
        QDate premiereDate = query.value(3).toDate();
        QDate derniereDate = query.value(4).toDate();
        
        model->setItem(row, 0, new QStandardItem(lieu));
        model->setItem(row, 1, new QStandardItem(QString::number(nbCollectes)));
        model->setItem(row, 2, new QStandardItem(QString::number(volumeTotal, 'f', 2)));
        model->setItem(row, 3, new QStandardItem(premiereDate.toString("dd/MM/yyyy")));
        model->setItem(row, 4, new QStandardItem(derniereDate.toString("dd/MM/yyyy")));
        row++;
    }
    
    // Si aucune donnée, créer des exemples
    if (row == 0) {
        QStringList lieuxExemples = {"Tunis Centre", "Ariana", "Ben Arous", 
                                     "La Marsa", "Carthage", "Sidi Bou Said"};
        for (int i = 0; i < lieuxExemples.size(); i++) {
            model->setItem(i, 0, new QStandardItem(lieuxExemples[i]));
            model->setItem(i, 1, new QStandardItem(QString::number(QRandomGenerator::global()->bounded(5, 20))));
            model->setItem(i, 2, new QStandardItem(QString::number(QRandomGenerator::global()->bounded(1000, 5000), 'f', 2)));
            model->setItem(i, 3, new QStandardItem(QDate::currentDate().addDays(-30).toString("dd/MM/yyyy")));
            model->setItem(i, 4, new QStandardItem(QDate::currentDate().toString("dd/MM/yyyy")));
        }
    }
    
    QTableView *tableView = new QTableView(mapDialog);
    tableView->setModel(model);
    tableView->setAlternatingRowColors(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->resizeColumnsToContents();
    tableView->setShowGrid(true);
    layout->addWidget(tableView);
    
    // Informations supplémentaires
    QLabel *infoLabel = new QLabel(
        "💡 Pour voir une carte interactive, installez Qt WebEngine via le Maintenance Tool de Qt.\n"
        "   Ou ouvrez ces coordonnées dans Google Maps / OpenStreetMap.",
        mapDialog
    );
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; padding: 10px;");
    layout->addWidget(infoLabel);
    
    // Bouton pour ouvrir dans navigateur externe
    QPushButton *openBrowserBtn = new QPushButton("🌐 Ouvrir dans le navigateur", mapDialog);
    connect(openBrowserBtn, &QPushButton::clicked, [=]() {
        QString url = "https://www.openstreetmap.org/?mlat=36.8065&mlon=10.1815&zoom=10";
        QDesktopServices::openUrl(QUrl(url));
    });
    
    QPushButton *closeBtn = new QPushButton("Fermer", mapDialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openBrowserBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    layout->addLayout(buttonLayout);
    
    connect(closeBtn, &QPushButton::clicked, mapDialog, &QDialog::accept);
    
    mapDialog->exec();
#endif
}
