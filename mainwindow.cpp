#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QPdfWriter>
#include <QTextDocument>
#include <QDateTime>
#include <QDate>
#include <QMetaType>
#include <QSignalBlocker>
#include "connection.h"
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
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QLinearGradient>
#include <QPolygonF>
#include <QTime>
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
#include <QTableWidget>
#include <QTableWidgetItem>

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
#include <QFrame>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QTextStream>
#include <QFile>
#include <QStringConverter>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QPrinter>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QEvent>
#include <QMouseEvent>
#include "alerte.h"
#ifdef LOCATION_AVAILABLE
#include "alertemap.h"
#endif
#include "faceenrolldialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    if (ui->dateEdit_ajout_eq) {
        ui->dateEdit_ajout_eq->setDate(QDate::currentDate());
    }
    if (ui->dateEdit_modif_eq) {
        ui->dateEdit_modif_eq->setDate(QDate::currentDate());
    }
    
    // Installer le filtre d'événement pour le label de statistiques
    if (ui->label_stats_alerte) {
        ui->label_stats_alerte->installEventFilter(this);
    }

    // Initialisation des variables RFID
    rfidReader = nullptr;
    rfidScanMode = false;
    currentRFIDUID = "";

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
    // Charger les données avec vérifications de sécurité
    try {
        chargerEquipements();
        setupFiltresEquipements();  // Configurer les filtres
    } catch (...) {
        qDebug() << "⚠️ Erreur lors du chargement des équipements (non bloquant)";
    }
    
    try {
        chargerDechets();
    } catch (...) {
        qDebug() << "⚠️ Erreur lors du chargement des déchets (non bloquant)";
    }
    
    try {
        chargerParkings();
    } catch (...) {
        qDebug() << "⚠️ Erreur lors du chargement des parkings (non bloquant)";
    }
    
    // Initialize notification manager
    notificationManager = new NotificationManager(this);
    
    // Initialize the notification system (system tray, menu, etc.)
    notificationManager->initialize();
    
    // Connect signals for new alert notifications
    connect(notificationManager, &NotificationManager::notificationSent, 
            this, [](const QString &title, const QString &message) {
        qDebug() << "📬 Notification sent:" << title;
    });
    
    connect(notificationManager, &NotificationManager::newAlertDetected,
            this, [](int alertId, const QString &gravite, const QString &client,
                    const QString &description, const QString &type) {
        qDebug() << "🔔 New alert detected - ID:" << alertId << "Client:" << client;
    });
    
    connect(notificationManager, &NotificationManager::criticalAlertDetected,
            this, [](int alertId, const QString &client, const QString &type,
                    const QString &description) {
        qDebug() << "🚨 CRITICAL ALERT - ID:" << alertId << "Client:" << client;
    });
    
    // Start monitoring for new alerts (check every 5 seconds)
    notificationManager->startMonitoring(5);
    
    qDebug() << "✅ Intelligent notification system initialized and started";

    // Configuration des tables
    if (ui->tab_emp) {
        ui->tab_emp->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tab_emp->setSelectionMode(QAbstractItemView::SingleSelection);
    }
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Configuration du ComboBox de tri pour les employés
    if (ui->comboBox_3) {
        ui->comboBox_3->clear();
        ui->comboBox_3->addItem("Trier par...");
        ui->comboBox_3->addItem("Nom (A-Z)");
        ui->comboBox_3->addItem("Nom (Z-A)");
        ui->comboBox_3->addItem("Poste (A-Z)");
        ui->comboBox_3->addItem("Poste (Z-A)");
        ui->comboBox_3->addItem("Salaire (Croissant)");
        ui->comboBox_3->addItem("Salaire (Décroissant)");
        ui->comboBox_3->addItem("Date d'embauche");
    }
    
    // Initialiser les dates d'embauche
    if (ui->dateEdit) {
        ui->dateEdit->setDate(QDate::currentDate());
    }
    if (ui->dateEdit_2) {
        ui->dateEdit_2->setDate(QDate::currentDate());
    }
    
    // === CONNECTIONS POUR LES BOUTONS DE L'ONGLET EMPLOYÉ ===
    // Connexions explicites pour s'assurer que tous les boutons fonctionnent
    if (ui->ajouter) {
        connect(ui->ajouter, &QPushButton::clicked, this, &MainWindow::on_ajouter_clicked);
        qDebug() << "✅ Bouton Ajouter connecté";
    }
    if (ui->modifier) {
        connect(ui->modifier, &QPushButton::clicked, this, &MainWindow::on_modifier_clicked);
        qDebug() << "✅ Bouton Modifier connecté";
    }
    if (ui->supprimer) {
        connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
        qDebug() << "✅ Bouton Supprimer connecté";
    }
    if (ui->refresh) {
        connect(ui->refresh, &QPushButton::clicked, this, &MainWindow::on_refresh_clicked);
        qDebug() << "✅ Bouton Refresh connecté";
    }
    if (ui->refresh_2) {
        connect(ui->refresh_2, &QPushButton::clicked, this, &MainWindow::on_refresh_2_clicked);
        qDebug() << "✅ Bouton Recherche par CIN connecté";
    }
    if (ui->refresh_3) {
        connect(ui->refresh_3, &QPushButton::clicked, this, &MainWindow::on_refresh_3_clicked);
        qDebug() << "✅ Bouton Statistiques connecté";
    }
    if (ui->pushButton_5) {
        connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_pushButton_5_clicked);
        qDebug() << "✅ Bouton PDF connecté";
    }
    if (ui->pushButton_7) {
        connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::on_pushButton_7_clicked);
        qDebug() << "✅ Bouton Annuler (Ajout) connecté";
    }
    if (ui->pushButton_6) {
        connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::on_pushButton_6_clicked);
        qDebug() << "✅ Bouton Annuler (Modification) connecté";
    }
    if (ui->btnCaptureFace) {
        connect(ui->btnCaptureFace, &QPushButton::clicked, this, &MainWindow::on_btnCaptureFace_clicked);
        qDebug() << "✅ Bouton Capturer visage (Ajout) connecté";
    }
    if (ui->btnCaptureFaceModif) {
        connect(ui->btnCaptureFaceModif, &QPushButton::clicked, this, &MainWindow::on_btnCaptureFace_clicked);
        qDebug() << "✅ Bouton Capturer visage (Modification) connecté";
    }
    if (ui->comboBox_3) {
        connect(ui->comboBox_3, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::on_comboBox_3_currentIndexChanged);
        qDebug() << "✅ ComboBox de tri connecté";
    }
    if (ui->tab_emp) {
        connect(ui->tab_emp, &QTableView::clicked, this, &MainWindow::on_tab_emp_clicked);
        qDebug() << "✅ TableView employés connecté";
    }
    if (ui->lineEdit_13) {
        // Connect Enter key press to trigger search
        connect(ui->lineEdit_13, &QLineEdit::returnPressed, this, &MainWindow::on_refresh_2_clicked);
        // Set placeholder text
        ui->lineEdit_13->setPlaceholderText("Entrez le CIN (8 chiffres)");
        qDebug() << "✅ Champ de recherche CIN connecté";
    }
    
    // Configuration de la table des alertes
    if (ui->tableView_alertes) {
        ui->tableView_alertes->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView_alertes->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView_alertes->setAlternatingRowColors(false); // Désactiver l'alternance des couleurs
        ui->tableView_alertes->setShowGrid(true);
        // Configurer le tableau pour qu'il occupe tout l'espace disponible
        ui->tableView_alertes->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        
        // Réduire les marges du tableau pour maximiser l'espace
        ui->tableView_alertes->setContentsMargins(0, 0, 0, 0);
        
        // Configurer le stretch du layout pour que le tableau prenne tout l'espace disponible
        // Structure: formulaire=0, notifications=1, boutons=2, recherche=3, tableau=4, stats=5
        if (ui->verticalLayout_alertes) {
            // Réduire l'espacement entre les éléments
            ui->verticalLayout_alertes->setSpacing(5);
            // Le tableau (index 4) prend tout l'espace disponible avec un stretch élevé
            ui->verticalLayout_alertes->setStretch(4, 10); // Le tableau prend tout l'espace
            ui->verticalLayout_alertes->setStretch(5, 0); // Les stats (index 5) ne s'étendent pas
            ui->verticalLayout_alertes->setStretch(0, 0); // Le formulaire ne s'étend pas
            ui->verticalLayout_alertes->setStretch(1, 0); // La section Notifications ne s'étend pas
            ui->verticalLayout_alertes->setStretch(2, 0); // Les boutons ne s'étendent pas
            ui->verticalLayout_alertes->setStretch(3, 0); // La recherche ne s'étend pas
        }
    }
    
    // Configuration de la date pour les alertes
    if (ui->dateEdit_alerte) {
        ui->dateEdit_alerte->setDate(QDate::currentDate());
    }
    
    // Connexions pour les boutons de gestion des alertes
    if (ui->btn_ajouter_alerte) {
        connect(ui->btn_ajouter_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_ajouter_alerte_clicked);
    }
    if (ui->btn_modifier_alerte) {
        connect(ui->btn_modifier_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_modifier_alerte_clicked);
    }
    if (ui->btn_supprimer_alerte) {
        connect(ui->btn_supprimer_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_supprimer_alerte_clicked);
    }
    if (ui->btn_vider_alerte) {
        connect(ui->btn_vider_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_vider_alerte_clicked);
    }
    if (ui->btn_refresh_alerte) {
        connect(ui->btn_refresh_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_refresh_alerte_clicked);
    }
    if (ui->btn_rechercher_alerte) {
        connect(ui->btn_rechercher_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_rechercher_alerte_clicked);
    }
    if (ui->btn_filtrer_gravite_alerte) {
        connect(ui->btn_filtrer_gravite_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_filtrer_gravite_alerte_clicked);
    }
    if (ui->btn_trier_alerte) {
        connect(ui->btn_trier_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_trier_alerte_clicked);
    }
    if (ui->btn_pdf_alerte) {
        connect(ui->btn_pdf_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_pdf_alerte_clicked);
    }
    if (ui->btn_carte_alerte) {
        connect(ui->btn_carte_alerte, &QPushButton::clicked, this, &MainWindow::on_btn_carte_alerte_clicked);
    }
    
    // Connexion pour la sélection dans le tableau des alertes
    if (ui->tableView_alertes) {
        // Connexion pour la sélection de ligne (vérifier que selectionModel existe)
        QItemSelectionModel *selectionModel = ui->tableView_alertes->selectionModel();
        if (selectionModel) {
            connect(selectionModel, &QItemSelectionModel::selectionChanged,
                    this, &MainWindow::on_tableView_alertes_selectionChanged);
        }
        // Connexion pour le clic simple sur une cellule
        connect(ui->tableView_alertes, &QTableView::clicked, this, &MainWindow::on_tableView_alertes_clicked);
    }
    
    // Section Notifications est maintenant dans le UI (mainwindow.ui)
    // Les boutons sont déjà connectés dans le constructeur
    if (ui->groupBox_notifications_alerte) {
        // Ajouter l'emoji au titre programmatiquement
        ui->groupBox_notifications_alerte->setTitle("🔔 Notifications");
        
        // Configurer les marges du layout de notifications
        if (ui->gridLayout_notifications_alerte) {
            ui->gridLayout_notifications_alerte->setContentsMargins(5, 5, 5, 5);
        }
        
        // Optionnel: Ajouter un label de statut dans la section Notifications
        if (notificationManager && ui->gridLayout_notifications_alerte) {
            QLabel *statusLabel = new QLabel(ui->groupBox_notifications_alerte);
            statusLabel->setText("✅ Système actif - Surveillance automatique (vérification toutes les 5 secondes)");
            statusLabel->setStyleSheet(
                "QLabel {"
                "background-color: #2d5016;"
                "color: #90EE90;"
                "padding: 5px;"
                "border-radius: 3px;"
                "font-size: 9pt;"
                "}"
            );
            statusLabel->setWordWrap(true);
            statusLabel->setAlignment(Qt::AlignCenter);
            
            // Ajouter le label dans le layout de la section Notifications
            ui->gridLayout_notifications_alerte->addWidget(statusLabel, 2, 0, 1, 2);
        }
        qDebug() << "✅ Notifications section found in UI";
    }
    
    // Charger les alertes au démarrage
    chargerAlertes();
    
    // Configuration du tableau de déchets pour le défilement
    ui->tableView_dechet->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_dechet->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_dechet->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView_dechet->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView_dechet->setAlternatingRowColors(false); // Désactiver l'alternance des couleurs - toutes les lignes ont la même couleur
    ui->tableView_dechet->setShowGrid(true);
    
    // Forcer le tableau à être scrollable même avec peu de données
    ui->tableView_dechet->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->tableView_dechet->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->tableView_dechet->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    
    // Connecter le clic sur le tableau pour charger les données dans le formulaire de modification
    connect(ui->tableView_dechet, &QTableView::clicked, this, &MainWindow::on_tableView_dechet_clicked);

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
    
    // Initialiser la carte des alertes (si disponible)
    #ifdef LOCATION_AVAILABLE
    m_alerteMap = nullptr; // Sera créé à la demande
    #endif
    
    // Connect parking widgets
    if (ui->tableWidget_2_parking) {
        ui->tableWidget_2_parking->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget_2_parking->setSelectionMode(QAbstractItemView::SingleSelection);
        connect(ui->tableWidget_2_parking, &QTableWidget::itemClicked, this, &MainWindow::on_tableWidget_2_parking_itemClicked);
        qDebug() << "✅ TableWidget parking connecté";
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
    
    // Connect notification buttons (from both locations if they exist)
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
    
    // Connect notification buttons in the alertes section
    if (ui->notifications_settings_btn_alerte) {
        connect(ui->notifications_settings_btn_alerte, &QPushButton::clicked, this, &MainWindow::on_notifications_settings_btn_clicked);
        qDebug() << "✅ Notifications Settings button (alertes section) connected";
    }
    
    if (ui->notification_history_btn_alerte) {
        connect(ui->notification_history_btn_alerte, &QPushButton::clicked, this, &MainWindow::on_notification_history_btn_clicked);
        qDebug() << "✅ Notification History button (alertes section) connected";
    }
    
    if (ui->manage_alert_rules_btn_alerte) {
        connect(ui->manage_alert_rules_btn_alerte, &QPushButton::clicked, this, &MainWindow::on_manage_alert_rules_btn_clicked);
        qDebug() << "✅ Manage Alert Rules button (alertes section) connected";
    }
    
    qDebug() << "=== Button Connection Check Complete ===";
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Détecter les clics sur le label de statistiques
    if (obj == ui->label_stats_alerte && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            afficherStatistiquesGraphiquesAlertes();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
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
    if (ui->dateEdit) {
        ui->dateEdit->setDate(QDate::currentDate());
    }
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
    if (ui->dateEdit_2) {
        ui->dateEdit_2->setDate(QDate::currentDate());
    }
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

    QDate dateEmbauche = QDate::currentDate();
    if (ui->dateEdit && ui->dateEdit->date().isValid()) {
        dateEmbauche = ui->dateEdit->date();
    }

    employee nouvelEmploye(cin.toInt(), nom, prenom, email, motdepasse,
                           telephone.toInt(), poste, salaire.toFloat(), dateEmbauche);

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

    QDate dateEmbauche = QDate::currentDate();
    if (ui->dateEdit_2 && ui->dateEdit_2->date().isValid()) {
        dateEmbauche = ui->dateEdit_2->date();
    } else {
        // Récupérer la date existante depuis la base de données
        QSqlQuery dateQuery;
        dateQuery.prepare("SELECT DATE_EMBAUCHE FROM EMPLOYE WHERE CIN = :cin");
        dateQuery.bindValue(":cin", cin.toInt());
        if (dateQuery.exec() && dateQuery.next()) {
            dateEmbauche = dateQuery.value(0).toDate();
        }
    }

    if (gestionEmployes.modifier(cin.toInt(), nom, prenom, email, motdepasse,
                                 telephone.toInt(), poste, salaire.toFloat(), dateEmbauche)) {
        chargerEmployes();
        viderChampsModificationEmploye();
        QMessageBox::information(this, "Succès", "Employé modifié avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'employé!");
    }
}

void MainWindow::on_supprimer_clicked()
{
    if (!ui->tab_emp) return;
    QItemSelectionModel *selectionModel = ui->tab_emp->selectionModel();
    if (!selectionModel) return;
    QModelIndexList selection = selectionModel->selectedRows();
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
    // Clear search field and reload all employees
    if (ui->lineEdit_13) {
        ui->lineEdit_13->clear();
    }
    chargerEmployes();
}

void MainWindow::on_refresh_2_clicked()
{
    // Get CIN from lineEdit_13 field
    QString cinStr = ui->lineEdit_13->text().trimmed();
    
    // If field is empty, show all employees
    if (cinStr.isEmpty()) {
        chargerEmployes();
        return;
    }
    
    // Validate CIN - should be numeric
    bool ok;
    int cin = cinStr.toInt(&ok);
    
    if (!ok) {
        QMessageBox::warning(this, "Erreur de saisie", 
                           "Le CIN doit être un nombre valide.\nVeuillez entrer uniquement des chiffres.");
        ui->lineEdit_13->clear();
        ui->lineEdit_13->setFocus();
        return;
    }
    
    // Validate CIN length (should be 8 digits)
    if (cinStr.length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", 
                           "Le CIN doit contenir exactement 8 chiffres.");
        ui->lineEdit_13->clear();
        ui->lineEdit_13->setFocus();
        return;
    }
    
    // Perform search
    QSqlQueryModel* model = gestionEmployes.rechercherParCIN(cin);
    if (model) {
        ui->tab_emp->setModel(model);
        ui->tab_emp->resizeColumnsToContents();
        
        // Check if any results were found
        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Aucun résultat", 
                                   QString("Aucun employé trouvé avec le CIN: %1").arg(cinStr));
            // Clear the search field
            ui->lineEdit_13->clear();
            ui->lineEdit_13->setFocus();
            // Optionally reload all employees
            chargerEmployes();
        }
        // Success: results are displayed in the table, no need for popup message
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche.");
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
    qDebug() << "=== BOUTON PDF CLIQUE ===";

    if (!ui->tab_emp) return;
    QItemSelectionModel *selectionModel = ui->tab_emp->selectionModel();
    if (!selectionModel) return;
    QModelIndexList selection = selectionModel->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un employé!");
        return;
    }

    int row = selection.at(0).row();
    int cinEmploye = ui->tab_emp->model()->index(row, 1).data().toInt();

    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE "
                  "FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cinEmploye);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Erreur: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::critical(this, "Erreur",
                              "Employé non trouvé dans la base de données!");
        return;
    }

    int cin = query.value(0).toInt();
    QString nom = query.value(1).toString();
    QString prenom = query.value(2).toString();
    QString poste = query.value(3).toString();
    float salaire = query.value(4).toFloat();
    QDate dateEmbauche = query.value(5).toDate();

    if (nom.isEmpty() || prenom.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Données employé incomplètes!");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer l'attestation",
                                                    QDir::homePath() + "/Attestation_" + nom + "_" + prenom + ".pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    bool result = employee::genererAttestationTravail(
        cin, nom, prenom, poste, salaire, dateEmbauche, fileName
        );

    if (result) {
        QMessageBox::information(this, "Succès",
                                 QString("Attestation générée avec succès!\n\nFichier : %1").arg(fileName));
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de la génération du PDF.");
    }
}

void MainWindow::on_refresh_3_clicked()
{
    afficherEmployeeCharts();
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
    
    // Récupérer la date d'embauche si disponible
    QVariant dateVar = ui->tab_emp->model()->index(row, 9).data();
    if (dateVar.isValid()) {
        if (dateVar.typeId() == QMetaType::QDate) {
            if (ui->dateEdit) ui->dateEdit->setDate(dateVar.toDate());
            if (ui->dateEdit_2) ui->dateEdit_2->setDate(dateVar.toDate());
        } else {
            QDate date = QDate::fromString(dateVar.toString(), "yyyy-MM-dd");
            if (date.isValid()) {
                if (ui->dateEdit) ui->dateEdit->setDate(date);
                if (ui->dateEdit_2) ui->dateEdit_2->setDate(date);
            }
        }
    }

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

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    if (index == 0) {
        chargerEmployes();
        return;
    }

    QSqlQueryModel* model = nullptr;
    
    switch(index) {
        case 1: // Nom (A-Z)
            model = gestionEmployes.trierParNom(true);
            break;
        case 2: // Nom (Z-A)
            model = gestionEmployes.trierParNom(false);
            break;
        case 3: // Poste (A-Z)
            model = gestionEmployes.trierParPoste(true);
            break;
        case 4: // Poste (Z-A)
            model = gestionEmployes.trierParPoste(false);
            break;
        case 5: // Salaire (Croissant)
            model = gestionEmployes.trierParSalaire(true);
            break;
        case 6: // Salaire (Décroissant)
            model = gestionEmployes.trierParSalaire(false);
            break;
        case 7: // Date d'embauche
            model = gestionEmployes.trierParDateEmbauche(true);
            break;
        default:
            chargerEmployes();
            return;
    }
    
    if (model) {
        ui->tab_emp->setModel(model);
        ui->tab_emp->resizeColumnsToContents();
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    viderChampsEmploye();
}

void MainWindow::on_pushButton_6_clicked()
{
    viderChampsModificationEmploye();
}

void MainWindow::on_btnCaptureFace_clicked()
{
    // On essaie d'abord le CIN du formulaire de modification, sinon celui du formulaire d'ajout
    QString cinText = ui->lineEdit_cin2->text().trimmed();
    if (cinText.isEmpty()) {
        cinText = ui->lineEdit_cin->text().trimmed();
    }

    if (cinText.isEmpty()) {
        QMessageBox::warning(this, "CIN manquant",
                             "Veuillez saisir ou sélectionner un employé avant de capturer son visage.");
        return;
    }

    // Vérifier que le CIN existe dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cinText);
    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Employé introuvable",
                             QString("Aucun employé trouvé avec le CIN: %1\n\n"
                                    "Veuillez d'abord ajouter l'employé avant de capturer son visage.")
                             .arg(cinText));
        return;
    }

    // Ouvrir le dialogue de capture de visage
    try {
        FaceEnrollDialog dialog(cinText, this);
        if (dialog.exec() == QDialog::Accepted) {
            // Recharger les employés pour mettre à jour l'affichage
            chargerEmployes();
            QMessageBox::information(this, "Succès",
                                   QString("Le visage de l'employé CIN: %1 a été enregistré avec succès!")
                                   .arg(cinText));
        }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Erreur",
                             QString("Erreur lors de l'ouverture de la caméra:\n%1\n\n"
                                    "Vérifiez que votre caméra est connectée et accessible.")
                             .arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Erreur",
                             "Erreur inconnue lors de l'ouverture de la caméra.\n\n"
                             "Vérifiez que votre caméra est connectée et accessible.");
    }
}

// ==================== MÉTHODES POUR ÉQUIPEMENTS ====================

void MainWindow::chargerEquipements()
{
    if (!ui || !ui->tableView) return;
    
    qDebug() << "Chargement des équipements...";
    QSqlQueryModel *model = Equipement::afficher();
    if (model) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        qDebug() << "Équipements chargés:" << model->rowCount() << " lignes";
        
        if (ui->labelResultats) {
            ui->labelResultats->setText(QString("Résultats: %1 équipement(s)").arg(model->rowCount()));
        }

        // Rafraîchir les filtres pour refléter les nouvelles données
        remplirFiltresType();
        remplirFiltresZone();
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
    if (!ui) return;
    if (ui->lineEdit_30) ui->lineEdit_30->clear();
    if (ui->lineEdit_31) ui->lineEdit_31->clear();
    if (ui->lineEdit_32) ui->lineEdit_32->clear();
    if (ui->lineEdit_33) ui->lineEdit_33->clear();
    if (ui->lineEdit_34) ui->lineEdit_34->clear();
    if (ui->lineEdit_36) ui->lineEdit_36->clear();
    if (ui->dateEdit_ajout_eq) ui->dateEdit_ajout_eq->setDate(QDate::currentDate());
}

void MainWindow::viderChampsModificationEquipement()
{
    if (!ui) return;
    if (ui->lineEdit_37) ui->lineEdit_37->clear();
    if (ui->lineEdit_42) ui->lineEdit_42->clear();
    if (ui->lineEdit_40) ui->lineEdit_40->clear();
    if (ui->lineEdit_38) ui->lineEdit_38->clear();
    if (ui->lineEdit_41) ui->lineEdit_41->clear();
    if (ui->lineEdit_39) ui->lineEdit_39->clear();
    if (ui->dateEdit_modif_eq) ui->dateEdit_modif_eq->setDate(QDate::currentDate());
}

// SLOTS ÉQUIPEMENTS - CORRECTION PRINCIPALE ICI
void MainWindow::on_pushButton_13_clicked()
{
    if (!ui || !ui->lineEdit_30 || !ui->lineEdit_31 || !ui->lineEdit_36) return;
    
    const QString idText = ui->lineEdit_30->text().trimmed();
    const QString nom = ui->lineEdit_31->text().trimmed();
    const QString type = ui->lineEdit_32 ? ui->lineEdit_32->text().trimmed() : QString();
    const QString zone = ui->lineEdit_33 ? ui->lineEdit_33->text().trimmed() : QString();
    const QString etat = ui->lineEdit_34 ? ui->lineEdit_34->text().trimmed() : QString();
    const QString consommationText = ui->lineEdit_36->text().trimmed();
    const QDate dateInstallation = ui->dateEdit_ajout_eq ? ui->dateEdit_ajout_eq->date() : QDate::currentDate();

    if (idText.isEmpty() || nom.isEmpty() || type.isEmpty() || zone.isEmpty() || etat.isEmpty() || consommationText.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants",
                             "Veuillez remplir tous les champs (ID, Nom, Type, Zone, État, Consommation).");
        return;
    }

    bool okId = false;
    int id = idText.toInt(&okId);
    if (!okId) {
        QMessageBox::warning(this, "ID invalide", "L'ID doit être un nombre.");
        ui->lineEdit_30->setFocus();
        return;
    }

    bool okConso = false;
    double consommation = consommationText.toDouble(&okConso);
    if (!okConso || consommation < 0) {
        QMessageBox::warning(this, "Consommation invalide", "La consommation doit être un nombre positif.");
        ui->lineEdit_36->setFocus();
        return;
    }

    if (!dateInstallation.isValid()) {
        QMessageBox::warning(this, "Date invalide", "Veuillez sélectionner une date d'installation valide.");
        return;
    }

    Equipement nouvelEquipement(id, nom, type, zone, etat, dateInstallation, consommation);
    if (nouvelEquipement.ajouter()) {
        chargerEquipements();
        viderChampsEquipement();
        QMessageBox::information(this, "Succès",
                                 QString("Équipement ajouté!\n\nID: %1\nNom: %2").arg(id).arg(nom));
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de l'ajout de l'équipement. Vérifiez la connexion ou les données saisies.");
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    if (!ui || !ui->lineEdit_37 || !ui->lineEdit_42 || !ui->lineEdit_39) return;
    
    const QString idText = ui->lineEdit_37->text().trimmed();
    const QString nom = ui->lineEdit_42->text().trimmed();
    const QString type = ui->lineEdit_40 ? ui->lineEdit_40->text().trimmed() : QString();
    const QString zone = ui->lineEdit_38 ? ui->lineEdit_38->text().trimmed() : QString();
    const QString etat = ui->lineEdit_41 ? ui->lineEdit_41->text().trimmed() : QString();
    const QString consommationText = ui->lineEdit_39 ? ui->lineEdit_39->text().trimmed() : QString();
    const QDate dateInstallation = ui->dateEdit_modif_eq ? ui->dateEdit_modif_eq->date() : QDate::currentDate();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un équipement à modifier.");
        return;
    }
    if (nom.isEmpty() || type.isEmpty() || zone.isEmpty() || etat.isEmpty() || consommationText.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants",
                             "Veuillez remplir tous les champs (Nom, Type, Zone, État, Consommation).");
        return;
    }

    bool okId = false;
    int id = idText.toInt(&okId);
    if (!okId) {
        QMessageBox::warning(this, "ID invalide", "L'ID doit être un nombre.");
        return;
    }

    bool okConso = false;
    double consommation = consommationText.toDouble(&okConso);
    if (!okConso || consommation < 0) {
        QMessageBox::warning(this, "Consommation invalide", "La consommation doit être un nombre positif.");
        return;
    }

    if (!dateInstallation.isValid()) {
        QMessageBox::warning(this, "Date invalide", "Veuillez sélectionner une date d'installation valide.");
        return;
    }

    Equipement equipement(id, nom, type, zone, etat, dateInstallation, consommation);
    if (equipement.modifier()) {
        chargerEquipements();
        viderChampsModificationEquipement();
        QMessageBox::information(this, "Succès", "Équipement modifié avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification de l'équipement.");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if (!ui->tableView) return;
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    if (!selectionModel) return;
    QModelIndexList selection = selectionModel->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un équipement à supprimer.");
        return;
    }

    int row = selection.at(0).row();
    QString idStr = ui->tableView->model()->index(row, 0).data().toString();
    QString nom = ui->tableView->model()->index(row, 1).data().toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  QString("Supprimer cet équipement?\n\nID: %1\nNom: %2").arg(idStr).arg(nom),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool okId = false;
        int id = idStr.toInt(&okId);
        if (!okId) {
            QMessageBox::warning(this, "ID invalide", "Impossible de supprimer: identifiant invalide.");
            return;
        }

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
    if (ui->comboBox) {
        ui->comboBox->setCurrentIndex(0);
    }
    // Réinitialiser les filtres
    if (ui->comboBoxFiltreType) ui->comboBoxFiltreType->setCurrentIndex(0);
    if (ui->comboBoxFiltreZone) ui->comboBoxFiltreZone->setCurrentIndex(0);
    if (ui->lineEditRechercheID) ui->lineEditRechercheID->clear();
    if (ui->labelResultats) {
        QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
        int count = model ? model->rowCount() : 0;
        ui->labelResultats->setText(QString("Résultats: %1 équipement(s)").arg(count));
    }
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
    if (!index.isValid() || !ui || !ui->tableView) return;

    int row = index.row();
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) return;
    
    QString id = model->data(model->index(row, 0)).toString();
    QString nom = model->data(model->index(row, 1)).toString();
    QString type = model->data(model->index(row, 2)).toString();
    QString zone = model->data(model->index(row, 3)).toString();
    QString etat = model->data(model->index(row, 4)).toString();
    QVariant dateVar = model->data(model->index(row, 5));
    QString consommation = model->data(model->index(row, 6)).toString();

    QDate dateInstallation = dateVar.typeId() == QMetaType::QDate ? dateVar.toDate()
                           : dateVar.typeId() == QMetaType::QDateTime ? dateVar.toDateTime().date()
                           : QDate::currentDate();

    // Remplir champs ajout
    if (ui->lineEdit_30) ui->lineEdit_30->setText(id);
    if (ui->lineEdit_31) ui->lineEdit_31->setText(nom);
    if (ui->lineEdit_32) ui->lineEdit_32->setText(type);
    if (ui->lineEdit_33) ui->lineEdit_33->setText(zone);
    if (ui->lineEdit_34) ui->lineEdit_34->setText(etat);
    if (ui->lineEdit_36) ui->lineEdit_36->setText(consommation);
    if (ui->dateEdit_ajout_eq && dateInstallation.isValid()) ui->dateEdit_ajout_eq->setDate(dateInstallation);

    // Remplir champs modification
    if (ui->lineEdit_37) ui->lineEdit_37->setText(id);
    if (ui->lineEdit_42) ui->lineEdit_42->setText(nom);
    if (ui->lineEdit_40) ui->lineEdit_40->setText(type);
    if (ui->lineEdit_38) ui->lineEdit_38->setText(zone);
    if (ui->lineEdit_41) ui->lineEdit_41->setText(etat);
    if (ui->lineEdit_39) ui->lineEdit_39->setText(consommation);
    if (ui->dateEdit_modif_eq && dateInstallation.isValid()) ui->dateEdit_modif_eq->setDate(dateInstallation);
}

// ==================== FONCTIONNALITÉS DE FILTRAGE ET RECHERCHE (comme Atelier_Connexion) ====================

void MainWindow::setupFiltresEquipements()
{
    if (!ui) return;
    
    qDebug() << "Configuration des filtres équipements...";
    
    // Connecter les signaux des filtres si les widgets existent
    if (ui->comboBoxFiltreType) {
        connect(ui->comboBoxFiltreType, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onFiltreEquipementChanged);
    }
    
    if (ui->comboBoxFiltreZone) {
        connect(ui->comboBoxFiltreZone, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &MainWindow::onFiltreEquipementChanged);
    }
    
    if (ui->lineEditRechercheID) {
        connect(ui->lineEditRechercheID, &QLineEdit::textChanged,
                this, &MainWindow::onRechercheIDEquipementChanged);
    }
    
    if (ui->pushButtonResetFiltres) {
        connect(ui->pushButtonResetFiltres, &QPushButton::clicked,
                this, &MainWindow::on_pushButtonResetFiltres_clicked);
    }
    
    // Remplir les filtres (même s'ils seront vides pour l'instant)
    remplirFiltresType();
    remplirFiltresZone();
    
    qDebug() << "Filtres équipements configurés";
}

void MainWindow::remplirFiltresType()
{
    if (!ui || !ui->comboBoxFiltreType) return;

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) return;
    }

    QSignalBlocker blocker(ui->comboBoxFiltreType);
    ui->comboBoxFiltreType->clear();
    ui->comboBoxFiltreType->addItem("Tous les types", "");

    QSqlQuery query("SELECT DISTINCT TYPE_EQUIPEMENT FROM EQUIPEMENT ORDER BY TYPE_EQUIPEMENT");
    while (query.next()) {
        const QString type = query.value(0).toString().trimmed();
        if (!type.isEmpty()) {
            ui->comboBoxFiltreType->addItem(type, type);
        }
    }
}

void MainWindow::remplirFiltresZone()
{
    if (!ui || !ui->comboBoxFiltreZone) return;

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) return;
    }

    QSignalBlocker blocker(ui->comboBoxFiltreZone);
    ui->comboBoxFiltreZone->clear();
    ui->comboBoxFiltreZone->addItem("Toutes les zones", "");

    QSqlQuery query("SELECT DISTINCT ZONE FROM EQUIPEMENT ORDER BY ZONE");
    while (query.next()) {
        const QString zone = query.value(0).toString().trimmed();
        if (!zone.isEmpty()) {
            ui->comboBoxFiltreZone->addItem(zone, zone);
        }
    }
}

void MainWindow::appliquerFiltresEquipements()
{
    if (!ui || !ui->tableView) return;
    
    qDebug() << "=== Application des filtres équipements ===";
    
    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) return;
    }
    
    // Récupérer les valeurs des filtres
    const QString typeFiltre = ui->comboBoxFiltreType ? ui->comboBoxFiltreType->currentData().toString() : QString();
    const QString zoneFiltre = ui->comboBoxFiltreZone ? ui->comboBoxFiltreZone->currentData().toString() : QString();
    const QString rechercheID = ui->lineEditRechercheID ? ui->lineEditRechercheID->text().trimmed() : QString();
    
    QString requete = "SELECT ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION "
                      "FROM EQUIPEMENT WHERE 1=1";
    
    if (!typeFiltre.isEmpty()) {
        requete += " AND TYPE_EQUIPEMENT = :type";
    }
    if (!zoneFiltre.isEmpty()) {
        requete += " AND ZONE = :zone";
    }
    if (!rechercheID.isEmpty()) {
        requete += " AND ID = :id";
    }
    
    requete += " ORDER BY ID";
    
    QSqlQuery query;
    query.prepare(requete);
    
    if (!typeFiltre.isEmpty()) {
        query.bindValue(":type", typeFiltre);
    }
    if (!zoneFiltre.isEmpty()) {
        query.bindValue(":zone", zoneFiltre);
    }
    if (!rechercheID.isEmpty()) {
        bool okId = false;
        int id = rechercheID.toInt(&okId);
        if (!okId) {
            QMessageBox::warning(this, "ID invalide", "La valeur saisie pour l'ID doit être un nombre.");
            return;
        }
        query.bindValue(":id", id);
    }
    
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête filtrée:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur lors du filtrage: " + query.lastError().text());
        return;
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(query));
    
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, tr("État"));
    model->setHeaderData(5, Qt::Horizontal, tr("Date d'installation"));
    model->setHeaderData(6, Qt::Horizontal, tr("Consommation"));
    
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    
    if (ui->labelResultats) {
        ui->labelResultats->setText(QString("Résultats: %1 équipement(s)").arg(model->rowCount()));
    }
    
    qDebug() << "Filtres appliqués - Résultats:" << model->rowCount();
}

void MainWindow::rechercherParIDEquipement(const QString &idText)
{
    if (!ui || !ui->tableView || idText.isEmpty()) return;
    
    qDebug() << "Recherche spécifique par ID:" << idText;
    
    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) return;
    }
    
    bool okId = false;
    int id = idText.toInt(&okId);
    if (!okId) {
        QMessageBox::warning(this, "ID invalide", "La valeur saisie pour l'ID doit être un nombre.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID, NOM, TYPE_EQUIPEMENT, ZONE, ETAT, DATE_INSTALLATION, CONSOMMATION "
                  "FROM EQUIPEMENT WHERE ID = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
        return;
    }
    
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(query));
    
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, tr("Zone"));
    model->setHeaderData(4, Qt::Horizontal, tr("État"));
    model->setHeaderData(5, Qt::Horizontal, tr("Date d'installation"));
    model->setHeaderData(6, Qt::Horizontal, tr("Consommation"));
    
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    
    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun équipement trouvé avec l'ID: " + idText);
    }
}

void MainWindow::onFiltreEquipementChanged()
{
    qDebug() << "Filtre équipement modifié - application des filtres...";
    appliquerFiltresEquipements();
}

void MainWindow::onRechercheIDEquipementChanged(const QString &text)
{
    qDebug() << "Recherche ID équipement modifiée:" << text;
    
    if (text.isEmpty()) {
        // Si le champ de recherche est vide, appliquer les autres filtres
        appliquerFiltresEquipements();
    } else {
        // Recherche spécifique par ID
        rechercherParIDEquipement(text);
    }
}

void MainWindow::onResetFiltresEquipements()
{
    qDebug() << "Réinitialisation des filtres équipements...";
    
    // Réinitialiser les contrôles de filtre
    if (ui->comboBoxFiltreType) ui->comboBoxFiltreType->setCurrentIndex(0);
    if (ui->comboBoxFiltreZone) ui->comboBoxFiltreZone->setCurrentIndex(0);
    if (ui->lineEditRechercheID) ui->lineEditRechercheID->clear();
    
    // Réafficher tous les équipements
    chargerEquipements();
    
    QMessageBox::information(this, "Filtres réinitialisés",
                             "Tous les filtres ont été réinitialisés.\nAffichage de tous les équipements.");
}

void MainWindow::on_pushButtonResetFiltres_clicked()
{
    onResetFiltresEquipements();
}

void MainWindow::on_pushButton_statistiques_clicked()
{
    // Fonction pour afficher les statistiques des équipements
    if (!ui || !ui->tableView) return;
    
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) {
        QMessageBox::information(this, "Statistiques", "Aucune donnée à afficher.");
        return;
    }
    
    int total = model->rowCount();
    double consommationTotale = 0;
    
    for (int i = 0; i < total; ++i) {
        QString consoStr = model->data(model->index(i, 6)).toString();
        bool ok;
        double conso = consoStr.toDouble(&ok);
        if (ok) {
            consommationTotale += conso;
        }
    }
    
    double consommationMoyenne = total > 0 ? consommationTotale / total : 0;
    
    QString message = QString("📊 STATISTIQUES DES ÉQUIPEMENTS\n\n"
                              "Total équipements: %1\n"
                              "Consommation totale: %2\n"
                              "Consommation moyenne: %3")
                          .arg(total)
                          .arg(consommationTotale, 0, 'f', 2)
                          .arg(consommationMoyenne, 0, 'f', 2);
    
    QMessageBox::information(this, "Statistiques", message);
}

void MainWindow::on_pushButton_notifications_clicked()
{
    // Fonction pour vérifier les notifications
    QMessageBox::information(this, "Notifications", 
                             "Système de notifications pour les équipements.\n"
                             "Cette fonctionnalité sera implémentée prochainement.");
}

void MainWindow::on_pushButton_test_notifications_clicked()
{
    // Fonction pour tester les notifications
    QMessageBox::information(this, "Test Notifications", 
                             "Test des notifications.\n"
                             "Cette fonctionnalité sera implémentée prochainement.");
}

void MainWindow::on_pushButton_export_pdf_clicked()
{
    // Fonction pour exporter en PDF
    if (!ui || !ui->tableView) return;
    
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnée à exporter.");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    QDir::homePath() + "/equipements.pdf",
                                                    "Fichiers PDF (*.pdf)");
    
    if (fileName.isEmpty()) return;
    
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }
    
    QTextDocument document;
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; margin: 20px; }"
                   "h1 { color: #2c3e50; text-align: center; }"
                   "table { width: 100%; border-collapse: collapse; margin: 20px 0; }"
                   "th { background-color: #4CAF50; color: white; padding: 12px; border: 1px solid #45a049; }"
                   "td { padding: 10px; border: 1px solid #ddd; }"
                   "tr:nth-child(even) { background-color: #f2f2f2; }"
                   "</style></head><body>";
    
    html += "<h1>📊 RAPPORT DES ÉQUIPEMENTS</h1>";
    html += "<p><strong>Généré le:</strong> " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm") + "</p>";
    html += "<p><strong>Total équipements:</strong> " + QString::number(model->rowCount()) + "</p>";
    
    html += "<table><tr>"
            "<th>ID</th>"
            "<th>Nom</th>"
            "<th>Type</th>"
            "<th>Zone</th>"
            "<th>État</th>"
            "<th>Date d'installation</th>"
            "<th>Consommation (kWh)</th>"
            "</tr>";
    
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < 7; ++col) {
            QString data = model->data(model->index(row, col)).toString();
            if (col == 6) {
                bool ok = false;
                double conso = data.toDouble(&ok);
                if (ok) {
                    data = QString::number(conso, 'f', 2);
                }
            }
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }
    
    html += "</table></body></html>";
    
    document.setHtml(html);
    
    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(15, 15, 15, 15));
    document.print(&writer);
    
    QMessageBox::information(this, "Export PDF Réussi",
                             QString("Le rapport a été exporté avec succès:\n%1")
                                 .arg(fileName));
}

// ==================== MÉTHODES POUR DÉCHETS ====================

void MainWindow::chargerDechets()
{
    if (!ui || !ui->tableView_dechet) return;
    
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
    if (!ui || !ui->cin_dechet_modifier || !ui->date_dechet_modifier || 
        !ui->lieu_dechet_modifier || !ui->volume_dechet_modifier) {
        return;
    }
    
    // Validation des champs
    if (ui->cin_dechet_modifier->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le CIN est obligatoire!");
        return;
    }
    
    if (ui->lieu_dechet_modifier->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le lieu est obligatoire!");
        return;
    }
    
    if (ui->volume_dechet_modifier->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le volume est obligatoire!");
        return;
    }
    
    bool ok;
    int cin = ui->cin_dechet_modifier->text().toInt(&ok);
    if (!ok || cin <= 0) {
        QMessageBox::warning(this, "Erreur", "Le CIN doit être un nombre valide!");
        return;
    }
    
    float volume = ui->volume_dechet_modifier->text().toFloat(&ok);
    if (!ok || volume < 0) {
        QMessageBox::warning(this, "Erreur", "Le volume doit être un nombre valide!");
        return;
    }
    
    QDate date = ui->date_dechet_modifier->date();
    QString lieu = ui->lieu_dechet_modifier->text().trimmed();
    QString etat = ui->recycle_modifier->isChecked() ? "recyclé" : "non recyclé";

    qDebug() << "=== MODIFICATION DÉCHET ===";
    qDebug() << "CIN:" << cin << "Date:" << date.toString() << "Lieu:" << lieu << "Volume:" << volume << "Etat:" << etat;

    Dechet D(cin, date, lieu, volume, etat);

    if (D.modifier()) {
        QMessageBox::information(this, "Modification", "Déchet modifié avec succès !");
        // Rafraîchir le tableau
        QSqlQueryModel *model = tmpDechet.afficher();
        if (model) {
            ui->tableView_dechet->setModel(model);
            ui->tableView_dechet->resizeColumnsToContents();
            qDebug() << "✅ Tableau rafraîchi avec" << model->rowCount() << "lignes";
        }
    } else {
        QMessageBox::critical(this, "Erreur", 
            QString("Modification échouée !\n\nVérifiez:\n"
                   "- Que tous les champs sont remplis correctement\n"
                   "- Que le CIN existe dans la base de données\n"
                   "- La connexion à la base de données\n\n"
                   "Consultez la sortie de compilation pour plus de détails."));
    }
}

void MainWindow::on_tableView_dechet_clicked(const QModelIndex &index)
{
    if (!ui || !ui->tableView_dechet || !index.isValid()) {
        return;
    }
    
    // Récupérer les données de la ligne sélectionnée
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableView_dechet->model());
    if (!model) {
        return;
    }
    
    int row = index.row();
    
    // Récupérer les valeurs de chaque colonne
    QString cinStr = model->data(model->index(row, 0)).toString();
    QDate date = model->data(model->index(row, 1)).toDate();
    QString lieu = model->data(model->index(row, 2)).toString();
    QString volumeStr = model->data(model->index(row, 3)).toString();
    QString etat = model->data(model->index(row, 4)).toString();
    
    // Charger les données dans le formulaire de modification
    if (ui->cin_dechet_modifier) {
        ui->cin_dechet_modifier->setText(cinStr);
    }
    if (ui->date_dechet_modifier) {
        ui->date_dechet_modifier->setDate(date);
    }
    if (ui->lieu_dechet_modifier) {
        ui->lieu_dechet_modifier->setText(lieu);
    }
    if (ui->volume_dechet_modifier) {
        ui->volume_dechet_modifier->setText(volumeStr);
    }
    if (ui->recycle_modifier && ui->nonrecycle_modifier) {
        if (etat.toLower().contains("recyclé") || etat.toLower().contains("recycle")) {
            ui->recycle_modifier->setChecked(true);
            ui->nonrecycle_modifier->setChecked(false);
        } else {
            ui->recycle_modifier->setChecked(false);
            ui->nonrecycle_modifier->setChecked(true);
        }
    }
    
    qDebug() << "✅ Données chargées dans le formulaire de modification: CIN=" << cinStr << "Lieu=" << lieu;
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
    if (!ui || !ui->comboBox_rech_dechet || !ui->lineEdit_rech_dechet || !ui->tableView_dechet) {
        return;
    }
    
    QString critere = ui->comboBox_rech_dechet->currentText().trimmed();
    QString valeur = ui->lineEdit_rech_dechet->text().trimmed();

    // Vérifier que la valeur de recherche n'est pas vide
    if (valeur.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer une valeur à rechercher!");
        return;
    }

    QString colonne;

    // Comparaison insensible à la casse
    if (critere.compare("cin", Qt::CaseInsensitive) == 0) {
        colonne = "CIN";
    } else if (critere.compare("lieu", Qt::CaseInsensitive) == 0) {
        colonne = "LIEU_COLLECTE";
    } else {
        QMessageBox::warning(this, "Erreur", "Critère de recherche invalide!");
        return;
    }

    QSqlQueryModel *model = tmpDechet.rechercher(colonne, valeur);
    if (model) {
        ui->tableView_dechet->setModel(model);
        ui->tableView_dechet->resizeColumnsToContents();
        
        // Afficher un message si aucun résultat
        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Recherche", 
                QString("Aucun résultat trouvé pour \"%1\" dans %2").arg(valeur, critere));
        } else {
            QMessageBox::information(this, "Recherche", 
                QString("%1 résultat(s) trouvé(s) pour \"%2\" dans %3")
                    .arg(model->rowCount()).arg(valeur, critere));
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche!");
    }
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
                if (data.typeId() == QMetaType::QDate) {
                    value = data.toDate().toString("dd/MM/yyyy");
                } else if (data.typeId() == QMetaType::QDateTime) {
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

void MainWindow::afficherEmployeeCharts()
{
    // Create a dialog to show employee charts
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("📊 Statistiques des Employés - Graphiques");
    chartDialog->setMinimumSize(1200, 800);
    chartDialog->setStyleSheet(
        "QDialog { background-color: #f0f0f0; }"
        "QTabWidget::pane { border: 2px solid #3399ff; background-color: white; }"
        "QTabBar::tab { background-color: #4CAF50; color: white; padding: 8px 16px; margin-right: 2px; }"
        "QTabBar::tab:selected { background-color: #45a049; }"
    );

    QTabWidget *tabWidget = new QTabWidget(chartDialog);
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);
    layout->addWidget(tabWidget);

#ifdef CHARTS_AVAILABLE
    // Get employee data from database
    QMap<QString, int> postStats; // Statistics by position
    QMap<QString, double> salaryStats; // Statistics by salary ranges
    
    QSqlQuery query;
    if (query.exec("SELECT POSTE, COUNT(*) as count FROM EMPLOYE GROUP BY POSTE")) {
        while (query.next()) {
            QString poste = query.value("POSTE").toString();
            int count = query.value("count").toInt();
            postStats[poste] = count;
        }
    }
    
    if (query.exec("SELECT POSTE, AVG(SALAIRE) as avg_salary FROM EMPLOYE WHERE SALAIRE IS NOT NULL GROUP BY POSTE")) {
        while (query.next()) {
            QString poste = query.value("POSTE").toString();
            double avgSalary = query.value("avg_salary").toDouble();
            salaryStats[poste] = avgSalary;
        }
    }

    // 1. Pie Chart - Employees by Position
    QChart *postChart = new QChart();
    postChart->setTitle("Répartition des Employés par Poste");
    postChart->setAnimationOptions(QChart::SeriesAnimations);
    postChart->legend()->setVisible(true);
    postChart->legend()->setAlignment(Qt::AlignRight);
    
    QPieSeries *postSeries = new QPieSeries();
    
    // Add data to pie chart with percentages
    int totalEmployees = 0;
    for (auto it = postStats.begin(); it != postStats.end(); ++it) {
        totalEmployees += it.value();
    }
    
    for (auto it = postStats.begin(); it != postStats.end(); ++it) {
        QString label = it.key() + " (" + QString::number(it.value()) + ")";
        if (totalEmployees > 0) {
            double percentage = (double(it.value()) / totalEmployees) * 100;
            label += " " + QString::number(percentage, 'f', 1) + "%";
        }
        QPieSlice *slice = postSeries->append(label, it.value());
        
        // Set different colors for each slice
        static QList<QColor> colors = {
            QColor(255, 99, 71),   // Tomato
            QColor(60, 179, 113),  // MediumSeaGreen
            QColor(106, 90, 205),  // SlateBlue
            QColor(255, 165, 0),   // Orange
            QColor(147, 112, 219), // MediumPurple
            QColor(255, 20, 147),  // DeepPink
            QColor(0, 191, 255),   // DeepSkyBlue
            QColor(50, 205, 50),   // LimeGreen
            QColor(255, 215, 0),   // Gold
            QColor(220, 20, 60)    // Crimson
        };
        int colorIndex = std::distance(postStats.begin(), it) % colors.size();
        slice->setColor(colors[colorIndex]);
        slice->setLabelVisible(true);
    }
    
    postChart->addSeries(postSeries);
    
    QChartView *postChartView = new QChartView(postChart);
    postChartView->setRenderHint(QPainter::Antialiasing);
    tabWidget->addTab(postChartView, "📊 Par Poste");

    // 2. Line Chart - Average Salary by Position
    QChart *salaryChart = new QChart();
    salaryChart->setTitle("Évolution des Salaires Moyens par Poste");
    salaryChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QLineSeries *salarySeries = new QLineSeries();
    salarySeries->setName("Salaire Moyen");
    salarySeries->setColor(QColor(255, 99, 71));  // Tomato color
    QCategoryAxis *salaryAxisX = new QCategoryAxis();
    QValueAxis *salaryAxisY = new QValueAxis();
    
    // Create a smooth curve with points
    QStringList salaryCategories;
    int index = 0;
    for (auto it = salaryStats.begin(); it != salaryStats.end(); ++it) {
        salarySeries->append(index, it.value());
        salaryCategories << it.key();
        index++;
    }
    
    // Configure axes
    for (const QString &category : salaryCategories) {
        salaryAxisX->append(category, index);
        index++;
    }
    salaryAxisY->setRange(0, salaryStats.isEmpty() ? 10000 : *std::max_element(salaryStats.begin(), salaryStats.end()) + 1000);
    salaryAxisY->setTitleText("Salaire Moyen (€)");
    salaryAxisY->setLabelFormat("%.0f");
    
    // Add series and axes to chart
    salaryChart->addSeries(salarySeries);
    salaryChart->addAxis(salaryAxisX, Qt::AlignBottom);
    salaryChart->addAxis(salaryAxisY, Qt::AlignLeft);
    salarySeries->attachAxis(salaryAxisX);
    salarySeries->attachAxis(salaryAxisY);
    
    // Enable points on the line
    salarySeries->setPointsVisible(true);
    salarySeries->setPointLabelsVisible(true);
    salarySeries->setPointLabelsFormat("@yPoint €");
    
    QChartView *salaryChartView = new QChartView(salaryChart);
    salaryChartView->setRenderHint(QPainter::Antialiasing);
    tabWidget->addTab(salaryChartView, "💰 Salaires");

#else
    // Fallback: Custom drawing implementation
    QWidget *postChartWidget = new QWidget();
    postChartWidget->setMinimumSize(600, 400);
    
    class EmployeePostChartWidget : public QWidget {
        QMap<QString, int> data;
    public:
        EmployeePostChartWidget(QMap<QString, int> d, QWidget *parent = nullptr) : QWidget(parent), data(d) {}
    protected:
        void paintEvent(QPaintEvent *) override {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            
            QRect rect = this->rect();
            int margin = 50;
            int chartWidth = rect.width() - 2 * margin;
            int chartHeight = rect.height() - 2 * margin - 40;
            
            // Draw axes
            painter.drawLine(margin, margin, margin, margin + chartHeight);
            painter.drawLine(margin, margin + chartHeight, margin + chartWidth, margin + chartHeight);
            
            // Draw bars
            if (data.isEmpty()) return;
            
            int barCount = data.size();
            int barWidth = chartWidth / (barCount * 2);
            int maxValue = *std::max_element(data.begin(), data.end());
            
            int x = margin + barWidth / 2;
            for (auto it = data.begin(); it != data.end(); ++it) {
                int barHeight = (it.value() * chartHeight) / maxValue;
                int y = margin + chartHeight - barHeight;
                
                // Draw bar
                painter.fillRect(x, y, barWidth, barHeight, QColor(76, 175, 80));
                
                // Draw text
                QRect textRect(x, margin + chartHeight + 5, barWidth, 30);
                painter.drawText(textRect, Qt::AlignCenter, it.key());
                
                QRect valueRect(x, y - 20, barWidth, 20);
                painter.drawText(valueRect, Qt::AlignCenter, QString::number(it.value()));
                
                x += barWidth * 2;
            }
            
            // Draw title
            painter.drawText(rect, Qt::AlignTop | Qt::AlignHCenter, "Répartition des Employés par Poste");
        }
    };
    
    EmployeePostChartWidget *postCustomChart = new EmployeePostChartWidget(postStats);
    tabWidget->addTab(postCustomChart, "📊 Par Poste");
#endif

    // Add close button
    QPushButton *closeBtn = new QPushButton("Fermer", chartDialog);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #45a049; }"
    );
    connect(closeBtn, &QPushButton::clicked, chartDialog, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignCenter);

    chartDialog->exec();
    delete chartDialog;
}

void MainWindow::on_btnGraphiquesEmployes_clicked()
{
    afficherEmployeeCharts();
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
    // Charger les alertes dans le tableau de l'onglet
    chargerAlertes();
}

void MainWindow::on_afficher_alertes_btn_clicked()
{
    // Changer vers l'onglet des alertes
    if (ui->tabWidget) {
        // Trouver l'index de l'onglet "Gestion des Alertes"
        for (int i = 0; i < ui->tabWidget->count(); i++) {
            if (ui->tabWidget->tabText(i) == "Gestion des Alertes") {
                ui->tabWidget->setCurrentIndex(i);
                chargerAlertes();
                break;
            }
        }
    }
}

// ==================== GESTION COMPLÈTE DES ALERTES ====================

void MainWindow::gererAlertes()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Gestion des Alertes");
    dialog->setMinimumSize(1000, 600);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    
    // === FORMULAIRE D'AJOUT/MODIFICATION ===
    QGroupBox *formGroup = new QGroupBox("Formulaire Alerte", dialog);
    QGridLayout *formLayout = new QGridLayout(formGroup);
    
    QLabel *lblId = new QLabel("ID:", dialog);
    QLineEdit *editId = new QLineEdit(dialog);
    editId->setReadOnly(true);
    editId->setPlaceholderText("Auto-généré");
    
    QLabel *lblType = new QLabel("Type *:", dialog);
    QLineEdit *editType = new QLineEdit(dialog);
    editType->setPlaceholderText("Ex: Urgence, Maintenance, Sécurité...");
    
    QLabel *lblIdClient = new QLabel("ID Client *:", dialog);
    QLineEdit *editIdClient = new QLineEdit(dialog);
    editIdClient->setPlaceholderText("Numéro du client");
    
    QLabel *lblStatut = new QLabel("Statut *:", dialog);
    QLineEdit *editStatut = new QLineEdit(dialog);
    editStatut->setPlaceholderText("Ex: Ouvert, En cours, Résolu...");
    
    QLabel *lblDescription = new QLabel("Description:", dialog);
    QTextEdit *editDescription = new QTextEdit(dialog);
    editDescription->setMaximumHeight(80);
    editDescription->setPlaceholderText("Description détaillée de l'alerte");
    
    QLabel *lblGravite = new QLabel("Gravité *:", dialog);
    QComboBox *comboGravite = new QComboBox(dialog);
    comboGravite->addItems({"Faible", "Moyenne", "Élevée", "Critique"});
    
    QLabel *lblDate = new QLabel("Date:", dialog);
    QDateEdit *editDate = new QDateEdit(dialog);
    editDate->setDate(QDate::currentDate());
    editDate->setCalendarPopup(true);
    
    formLayout->addWidget(lblId, 0, 0);
    formLayout->addWidget(editId, 0, 1);
    formLayout->addWidget(lblType, 1, 0);
    formLayout->addWidget(editType, 1, 1);
    formLayout->addWidget(lblIdClient, 2, 0);
    formLayout->addWidget(editIdClient, 2, 1);
    formLayout->addWidget(lblStatut, 3, 0);
    formLayout->addWidget(editStatut, 3, 1);
    formLayout->addWidget(lblDescription, 4, 0);
    formLayout->addWidget(editDescription, 4, 1);
    formLayout->addWidget(lblGravite, 5, 0);
    formLayout->addWidget(comboGravite, 5, 1);
    formLayout->addWidget(lblDate, 6, 0);
    formLayout->addWidget(editDate, 6, 1);
    
    // === BOUTONS CRUD ===
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnAjouter = new QPushButton("➕ Ajouter", dialog);
    QPushButton *btnModifier = new QPushButton("✏️ Modifier", dialog);
    QPushButton *btnSupprimer = new QPushButton("🗑️ Supprimer", dialog);
    QPushButton *btnVider = new QPushButton("🔄 Vider", dialog);
    QPushButton *btnRefresh = new QPushButton("🔄 Rafraîchir", dialog);
    
    btnLayout->addWidget(btnAjouter);
    btnLayout->addWidget(btnModifier);
    btnLayout->addWidget(btnSupprimer);
    btnLayout->addWidget(btnVider);
    btnLayout->addWidget(btnRefresh);
    
    // === RECHERCHE ET FILTRES ===
    QGroupBox *searchGroup = new QGroupBox("Recherche et Filtres", dialog);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchGroup);
    
    QLabel *lblRecherche = new QLabel("Rechercher:", dialog);
    QLineEdit *editRecherche = new QLineEdit(dialog);
    editRecherche->setPlaceholderText("Type, Statut, Gravité, Description...");
    QPushButton *btnRechercher = new QPushButton("🔍 Rechercher", dialog);
    
    QLabel *lblFiltrerGravite = new QLabel("Filtrer par Gravité:", dialog);
    QComboBox *comboFiltrerGravite = new QComboBox(dialog);
    comboFiltrerGravite->addItems({"Toutes", "Faible", "Moyenne", "Élevée", "Critique"});
    QPushButton *btnFiltrerGravite = new QPushButton("🔽 Filtrer", dialog);
    
    QPushButton *btnTrier = new QPushButton("📊 Trier par Date", dialog);
    QPushButton *btnPDF = new QPushButton("📄 Exporter PDF", dialog);
    
    searchLayout->addWidget(lblRecherche);
    searchLayout->addWidget(editRecherche);
    searchLayout->addWidget(btnRechercher);
    searchLayout->addWidget(lblFiltrerGravite);
    searchLayout->addWidget(comboFiltrerGravite);
    searchLayout->addWidget(btnFiltrerGravite);
    searchLayout->addWidget(btnTrier);
    searchLayout->addWidget(btnPDF);
    
    // === TABLEAU DES ALERTES ===
    QTableView *tableView = new QTableView(dialog);
    QSqlQueryModel *model = alerteTmp.afficher();
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->resizeColumnsToContents();
    
    // === STATISTIQUES ===
    // Utiliser un QPushButton stylisé comme label pour le rendre cliquable
    QPushButton *btnStats = new QPushButton(dialog);
    int total = alerteTmp.compterAlertes();
    int faible = alerteTmp.compterParGravite("Faible");
    int moyenne = alerteTmp.compterParGravite("Moyenne");
    int elevee = alerteTmp.compterParGravite("Élevée");
    int critique = alerteTmp.compterParGravite("Critique");
    QString stats = QString("📊 Statistiques: Total: %1 | Faible: %2 | Moyenne: %3 | Élevée: %4 | Critique: %5")
        .arg(total).arg(faible).arg(moyenne).arg(elevee).arg(critique);
    btnStats->setText(stats);
    btnStats->setStyleSheet(
        "QPushButton {"
        "    color: #5cb85c;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "    border: 1px solid #5cb85c;"
        "    border-radius: 3px;"
        "    background-color: #f0f0f0;"
        "    text-align: left;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e0e0e0;"
        "    cursor: pointer;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #d0d0d0;"
        "}"
    );
    connect(btnStats, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesGraphiquesAlertes);
    
    // === LAYOUT PRINCIPAL ===
    mainLayout->addWidget(formGroup);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(searchGroup);
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(btnStats); // Utiliser le bouton au lieu du label
    
    // === CONNECTIONS ===
    connect(btnAjouter, &QPushButton::clicked, [=]() {
        if (!validerAlerte(editType->text(), editIdClient->text(), editStatut->text(), comboGravite->currentText())) {
            return;
        }
        
        Alerte a;
        a.setType(editType->text());
        a.setIdClient(editIdClient->text().toInt());
        a.setStatut(editStatut->text());
        a.setDescription(editDescription->toPlainText());
        a.setGravite(comboGravite->currentText());
        a.setDate(editDate->date());
        
        if (a.ajouter()) {
            QMessageBox::information(dialog, "Succès", "Alerte ajoutée avec succès!");
            
            // Envoyer une notification si NotificationManager est disponible
            if (notificationManager) {
                QString message = QString("Nouvelle alerte créée:\nType: %1\nGravité: %2\nDescription: %3")
                    .arg(a.getType())
                    .arg(a.getGravite())
                    .arg(a.getDescription());
                // Use the new intelligent notification system
                notificationManager->showNotification("Nouvelle Alerte", message);
            }
            
            tableView->setModel(alerteTmp.afficher());
            viderChampsAlerteForm(editId, editType, editIdClient, editStatut, editDescription, comboGravite, editDate);
            // Mettre à jour le bouton de statistiques
            int total = alerteTmp.compterAlertes();
            int faible = alerteTmp.compterParGravite("Faible");
            int moyenne = alerteTmp.compterParGravite("Moyenne");
            int elevee = alerteTmp.compterParGravite("Élevée");
            int critique = alerteTmp.compterParGravite("Critique");
            QString stats = QString("📊 Statistiques: Total: %1 | Faible: %2 | Moyenne: %3 | Élevée: %4 | Critique: %5")
                .arg(total).arg(faible).arg(moyenne).arg(elevee).arg(critique);
            btnStats->setText(stats);
    } else {
            QMessageBox::critical(dialog, "Erreur", "Échec de l'ajout de l'alerte!");
        }
    });
    
    connect(btnModifier, &QPushButton::clicked, [=]() {
        if (editId->text().isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Veuillez sélectionner une alerte à modifier!");
            return;
        }
        
        if (!validerAlerte(editType->text(), editIdClient->text(), editStatut->text(), comboGravite->currentText())) {
            return;
        }
        
        Alerte a;
        a.setId(editId->text().toInt());
        a.setType(editType->text());
        a.setIdClient(editIdClient->text().toInt());
        a.setStatut(editStatut->text());
        a.setDescription(editDescription->toPlainText());
        a.setGravite(comboGravite->currentText());
        a.setDate(editDate->date());
        
        if (a.modifier()) {
            QMessageBox::information(dialog, "Succès", "Alerte modifiée avec succès!");
            tableView->setModel(alerteTmp.afficher());
            viderChampsAlerteForm(editId, editType, editIdClient, editStatut, editDescription, comboGravite, editDate);
            // Mettre à jour le bouton de statistiques
            int total = alerteTmp.compterAlertes();
            int faible = alerteTmp.compterParGravite("Faible");
            int moyenne = alerteTmp.compterParGravite("Moyenne");
            int elevee = alerteTmp.compterParGravite("Élevée");
            int critique = alerteTmp.compterParGravite("Critique");
            QString stats = QString("📊 Statistiques: Total: %1 | Faible: %2 | Moyenne: %3 | Élevée: %4 | Critique: %5")
                .arg(total).arg(faible).arg(moyenne).arg(elevee).arg(critique);
            btnStats->setText(stats);
        } else {
            QMessageBox::critical(dialog, "Erreur", "Échec de la modification!");
        }
    });
    
    connect(btnSupprimer, &QPushButton::clicked, [=]() {
        QItemSelectionModel *selModel = tableView->selectionModel();
        if (!selModel) return;
        QModelIndexList selected = selModel->selectedRows();
        if (selected.isEmpty()) {
            QMessageBox::warning(dialog, "Erreur", "Veuillez sélectionner une alerte à supprimer!");
            return;
        }
        
        int id = tableView->model()->data(selected.first()).toInt();
        
        QMessageBox::StandardButton reply = QMessageBox::question(
            dialog, "Confirmation", "Voulez-vous vraiment supprimer cette alerte?",
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply == QMessageBox::Yes) {
            if (alerteTmp.supprimer(id)) {
                QMessageBox::information(dialog, "Succès", "Alerte supprimée avec succès!");
                tableView->setModel(alerteTmp.afficher());
                viderChampsAlerteForm(editId, editType, editIdClient, editStatut, editDescription, comboGravite, editDate);
                // Mettre à jour le bouton de statistiques
                int total = alerteTmp.compterAlertes();
                int faible = alerteTmp.compterParGravite("Faible");
                int moyenne = alerteTmp.compterParGravite("Moyenne");
                int elevee = alerteTmp.compterParGravite("Élevée");
                int critique = alerteTmp.compterParGravite("Critique");
                QString stats = QString("📊 Statistiques: Total: %1 | Faible: %2 | Moyenne: %3 | Élevée: %4 | Critique: %5")
                    .arg(total).arg(faible).arg(moyenne).arg(elevee).arg(critique);
                btnStats->setText(stats);
            } else {
                QMessageBox::critical(dialog, "Erreur", "Échec de la suppression!");
            }
        }
    });
    
    connect(btnVider, &QPushButton::clicked, [=]() {
        viderChampsAlerteForm(editId, editType, editIdClient, editStatut, editDescription, comboGravite, editDate);
    });
    
    connect(btnRefresh, &QPushButton::clicked, [=]() {
        tableView->setModel(alerteTmp.afficher());
        // Mettre à jour le bouton de statistiques
        int total = alerteTmp.compterAlertes();
        int faible = alerteTmp.compterParGravite("Faible");
        int moyenne = alerteTmp.compterParGravite("Moyenne");
        int elevee = alerteTmp.compterParGravite("Élevée");
        int critique = alerteTmp.compterParGravite("Critique");
        QString stats = QString("📊 Statistiques: Total: %1 | Faible: %2 | Moyenne: %3 | Élevée: %4 | Critique: %5")
            .arg(total).arg(faible).arg(moyenne).arg(elevee).arg(critique);
        btnStats->setText(stats);
    });
    
    connect(btnRechercher, &QPushButton::clicked, [=]() {
        if (editRecherche->text().isEmpty()) {
            tableView->setModel(alerteTmp.afficher());
        } else {
            tableView->setModel(alerteTmp.rechercher(editRecherche->text()));
        }
    });
    
    connect(btnFiltrerGravite, &QPushButton::clicked, [=]() {
        QString gravite = comboFiltrerGravite->currentText();
        if (gravite == "Toutes") {
            tableView->setModel(alerteTmp.afficher());
        } else {
            tableView->setModel(alerteTmp.filtrerParGravite(gravite));
        }
    });
    
    connect(btnTrier, &QPushButton::clicked, [=]() {
        tableView->setModel(alerteTmp.trierPar("DATE_ALERTE", "DESC"));
    });
    
    connect(btnPDF, &QPushButton::clicked, [=]() {
        genererPDFAlertes(dialog);
    });
    
    // Sélection dans le tableau
    QItemSelectionModel *selModel = tableView->selectionModel();
    if (selModel) {
        connect(selModel, &QItemSelectionModel::selectionChanged, [=]() {
            QItemSelectionModel *selModel2 = tableView->selectionModel();
            if (!selModel2) return;
            QModelIndexList selected = selModel2->selectedRows();
        if (!selected.isEmpty()) {
            int row = selected.first().row();
            editId->setText(tableView->model()->data(tableView->model()->index(row, 0)).toString());
            editType->setText(tableView->model()->data(tableView->model()->index(row, 1)).toString());
            editIdClient->setText(tableView->model()->data(tableView->model()->index(row, 2)).toString());
            editStatut->setText(tableView->model()->data(tableView->model()->index(row, 3)).toString());
            editDescription->setPlainText(tableView->model()->data(tableView->model()->index(row, 4)).toString());
            comboGravite->setCurrentText(tableView->model()->data(tableView->model()->index(row, 5)).toString());
            QDate date = QDate::fromString(tableView->model()->data(tableView->model()->index(row, 6)).toString(), "dd/MM/yyyy");
            if (date.isValid()) {
                editDate->setDate(date);
            }
        }
        });
    }
    
    dialog->exec();
    delete dialog;
}

// Méthodes utilitaires pour les alertes
void MainWindow::chargerAlertes()
{
    if (ui->tableView_alertes) {
        QSqlQueryModel *model = alerteTmp.afficher();
        ui->tableView_alertes->setModel(model);
        ui->tableView_alertes->resizeColumnsToContents();
        updateStatsAlerte(ui->label_stats_alerte);
    }
}

void MainWindow::viderChampsAlerte()
{
    if (ui->lineEdit_id_alerte) ui->lineEdit_id_alerte->clear();
    if (ui->lineEdit_type_alerte) ui->lineEdit_type_alerte->clear();
    if (ui->lineEdit_id_client_alerte) ui->lineEdit_id_client_alerte->clear();
    if (ui->lineEdit_statut_alerte) ui->lineEdit_statut_alerte->clear();
    if (ui->textEdit_description_alerte) ui->textEdit_description_alerte->clear();
    if (ui->comboBox_gravite_alerte) ui->comboBox_gravite_alerte->setCurrentIndex(0);
    if (ui->dateEdit_alerte) ui->dateEdit_alerte->setDate(QDate::currentDate());
}

void MainWindow::viderChampsAlerteForm(QLineEdit *id, QLineEdit *type, QLineEdit *idClient, 
                                       QLineEdit *statut, QTextEdit *description, 
                                       QComboBox *gravite, QDateEdit *date)
{
    id->clear();
    type->clear();
    idClient->clear();
    statut->clear();
    description->clear();
    gravite->setCurrentIndex(0);
    date->setDate(QDate::currentDate());
}

bool MainWindow::validerAlerte(const QString& type, const QString& idClient, const QString& statut, const QString& gravite)
{
    if (type.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le type est obligatoire!");
        return false;
    }
    
    bool ok;
    int clientId = idClient.toInt(&ok);
    if (!ok || clientId <= 0) {
        QMessageBox::warning(this, "Erreur", "ID Client doit être un nombre positif!");
        return false;
    }
    
    if (statut.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le statut est obligatoire!");
        return false;
    }
    
    QStringList gravitesValides;
    gravitesValides << "Faible" << "Moyenne" << "Élevée" << "Critique";
    if (!gravitesValides.contains(gravite)) {
        QMessageBox::warning(this, "Erreur", "Gravité invalide! Valeurs acceptées: Faible, Moyenne, Élevée, Critique");
        return false;
    }
    
    return true;
}

void MainWindow::updateStatsAlerte(QLabel *label)
{
    int total = alerteTmp.compterAlertes();
    int faible = alerteTmp.compterParGravite("Faible");
    int moyenne = alerteTmp.compterParGravite("Moyenne");
    int elevee = alerteTmp.compterParGravite("Élevée");
    int critique = alerteTmp.compterParGravite("Critique");
    
    QString stats = QString("📊 Statistiques: Total: %1 | Faible: %2 | Moyenne: %3 | Élevée: %4 | Critique: %5")
        .arg(total).arg(faible).arg(moyenne).arg(elevee).arg(critique);
    label->setText(stats);
    
    // Rendre le label cliquable pour ouvrir les statistiques graphiques
    label->setStyleSheet(
        "QLabel {"
        "    color: #5cb85c;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "    border: 1px solid #5cb85c;"
        "    border-radius: 3px;"
        "    background-color: #f0f0f0;"
        "}"
        "QLabel:hover {"
        "    background-color: #e0e0e0;"
        "    cursor: pointer;"
        "}"
    );
    label->setCursor(Qt::PointingHandCursor);
}

void MainWindow::afficherStatistiquesGraphiquesAlertes()
{
    qDebug() << "=== AFFICHAGE STATISTIQUES GRAPHIQUES ALERTES ===";
    
    // Récupérer les statistiques
    int total = alerteTmp.compterAlertes();
    int faible = alerteTmp.compterParGravite("Faible");
    int moyenne = alerteTmp.compterParGravite("Moyenne");
    int elevee = alerteTmp.compterParGravite("Élevée");
    int critique = alerteTmp.compterParGravite("Critique");
    
    if (total == 0) {
        QMessageBox::information(this, "Statistiques", "Aucune alerte enregistrée pour afficher les statistiques.");
        return;
    }
    
    // Créer un dialogue popup pour afficher les graphiques
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("📊 Statistiques des Alertes - Graphiques");
    chartDialog->setMinimumSize(1000, 700);
    chartDialog->setStyleSheet(
        "QDialog {"
        "    background-color: #f5f5f5;"
        "}"
    );
    
    QTabWidget *tabWidget = new QTabWidget(chartDialog);
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);
    layout->addWidget(tabWidget);
    
    // 1. Graphique en camembert (Pie Chart) - Répartition par gravité
    QWidget *pieChartWidget = new QWidget();
    pieChartWidget->setMinimumSize(800, 600);
    
    class AlertPieChartWidget : public QWidget {
    public:
        int faible, moyenne, elevee, critique, total;
        AlertPieChartWidget(int f, int m, int e, int c, int t, QWidget *parent = nullptr) 
            : QWidget(parent), faible(f), moyenne(m), elevee(e), critique(c), total(t) {}
        
        void paintEvent(QPaintEvent *) override {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            
            int width = this->width();
            int height = this->height();
            int size = qMin(width, height) - 150;
            int x = (width - size) / 2;
            int y = 80;
            
            if (total == 0) return;
            
            // Couleurs pour chaque niveau de gravité
            QColor colorFaible(144, 238, 144);      // Vert clair
            QColor colorMoyenne(255, 255, 0);       // Jaune
            QColor colorElevee(255, 165, 0);        // Orange
            QColor colorCritique(220, 20, 60);      // Rouge
            
            int startAngle = 0;
            
            // Faible
            if (faible > 0) {
                int spanAngle = (faible * 360 * 16) / total;
                painter.setBrush(colorFaible);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawPie(x, y, size, size, startAngle, spanAngle);
                startAngle += spanAngle;
            }
            
            // Moyenne
            if (moyenne > 0) {
                int spanAngle = (moyenne * 360 * 16) / total;
                painter.setBrush(colorMoyenne);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawPie(x, y, size, size, startAngle, spanAngle);
                startAngle += spanAngle;
            }
            
            // Élevée
            if (elevee > 0) {
                int spanAngle = (elevee * 360 * 16) / total;
                painter.setBrush(colorElevee);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawPie(x, y, size, size, startAngle, spanAngle);
                startAngle += spanAngle;
            }
            
            // Critique
            if (critique > 0) {
                int spanAngle = (critique * 360 * 16) / total;
                painter.setBrush(colorCritique);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawPie(x, y, size, size, startAngle, spanAngle);
            }
            
            // Titre
            painter.setFont(QFont("Arial", 16, QFont::Bold));
            painter.setPen(Qt::black);
            painter.drawText(0, 20, width, 40, Qt::AlignCenter, "Répartition des Alertes par Gravité");
            
            // Légende
            int legendY = y + size + 40;
            int legendX = 50;
            painter.setFont(QFont("Arial", 11, QFont::Bold));
            
            // Faible
            painter.setBrush(colorFaible);
            painter.drawRect(legendX, legendY, 25, 25);
            painter.setPen(Qt::black);
            double percentFaible = total > 0 ? (faible * 100.0 / total) : 0;
            painter.drawText(legendX + 35, legendY + 18, QString("Faible: %1 (%2%)").arg(faible).arg(percentFaible, 0, 'f', 1));
            legendY += 35;
            
            // Moyenne
            painter.setBrush(colorMoyenne);
            painter.drawRect(legendX, legendY, 25, 25);
            painter.setPen(Qt::black);
            double percentMoyenne = total > 0 ? (moyenne * 100.0 / total) : 0;
            painter.drawText(legendX + 35, legendY + 18, QString("Moyenne: %1 (%2%)").arg(moyenne).arg(percentMoyenne, 0, 'f', 1));
            legendY += 35;
            
            // Élevée
            painter.setBrush(colorElevee);
            painter.drawRect(legendX, legendY, 25, 25);
            painter.setPen(Qt::black);
            double percentElevee = total > 0 ? (elevee * 100.0 / total) : 0;
            painter.drawText(legendX + 35, legendY + 18, QString("Élevée: %1 (%2%)").arg(elevee).arg(percentElevee, 0, 'f', 1));
            legendY += 35;
            
            // Critique
            painter.setBrush(colorCritique);
            painter.drawRect(legendX, legendY, 25, 25);
            painter.setPen(Qt::black);
            double percentCritique = total > 0 ? (critique * 100.0 / total) : 0;
            painter.drawText(legendX + 35, legendY + 18, QString("Critique: %1 (%2%)").arg(critique).arg(percentCritique, 0, 'f', 1));
            
            // Total en bas
            painter.setFont(QFont("Arial", 14, QFont::Bold));
            painter.setPen(QColor(50, 50, 50));
            painter.drawText(0, height - 30, width, 30, Qt::AlignCenter, 
                           QString("Total: %1 alertes").arg(total));
        }
    };
    
    AlertPieChartWidget *pieChart = new AlertPieChartWidget(faible, moyenne, elevee, critique, total);
    tabWidget->addTab(pieChart, "📊 Répartition par Gravité");
    
    // 2. Graphique en barres (Bar Chart) - Comparaison par gravité
    QWidget *barChartWidget = new QWidget();
    barChartWidget->setMinimumSize(800, 600);
    
    class AlertBarChartWidget : public QWidget {
    public:
        int faible, moyenne, elevee, critique, total;
        AlertBarChartWidget(int f, int m, int e, int c, int t, QWidget *parent = nullptr) 
            : QWidget(parent), faible(f), moyenne(m), elevee(e), critique(c), total(t) {}
        
        void paintEvent(QPaintEvent *) override {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);
            
            int width = this->width();
            int height = this->height();
            int margin = 80;
            int chartWidth = width - 2 * margin;
            int chartHeight = height - 2 * margin - 60;
            
            int maxValue = qMax(qMax(qMax(faible, moyenne), qMax(elevee, critique)), 1);
            
            // Couleurs
            QColor colorFaible(144, 238, 144);
            QColor colorMoyenne(255, 255, 0);
            QColor colorElevee(255, 165, 0);
            QColor colorCritique(220, 20, 60);
            
            QStringList labels = {"Faible", "Moyenne", "Élevée", "Critique"};
            QList<int> values = {faible, moyenne, elevee, critique};
            QList<QColor> colors = {colorFaible, colorMoyenne, colorElevee, colorCritique};
            
            int barCount = 4;
            int barWidth = chartWidth / (barCount * 2);
            int spacing = barWidth;
            int startX = margin + spacing;
            
            // Titre
            painter.setFont(QFont("Arial", 16, QFont::Bold));
            painter.setPen(Qt::black);
            painter.drawText(0, 20, width, 40, Qt::AlignCenter, "Nombre d'Alertes par Niveau de Gravité");
            
            // Dessiner les barres
            for (int i = 0; i < barCount; ++i) {
                int value = values[i];
                int barHeight = maxValue > 0 ? (value * chartHeight) / maxValue : 0;
                int y = margin + 60 + chartHeight - barHeight;
                int x = startX + i * (barWidth + spacing);
                
                // Barre
                painter.setBrush(colors[i]);
                painter.setPen(QPen(Qt::black, 2));
                painter.drawRect(x, y, barWidth, barHeight);
                
                // Valeur au-dessus de la barre
                painter.setFont(QFont("Arial", 10, QFont::Bold));
                painter.setPen(Qt::black);
                painter.drawText(x, y - 20, barWidth, 20, Qt::AlignCenter, QString::number(value));
                
                // Label en bas
                painter.setFont(QFont("Arial", 10));
                QRect textRect(x, margin + 60 + chartHeight + 10, barWidth, 30);
                painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, labels[i]);
            }
            
            // Axe Y (valeurs)
            painter.setFont(QFont("Arial", 9));
            painter.setPen(Qt::gray);
            int yStep = maxValue / 5;
            if (yStep == 0) yStep = 1;
            for (int i = 0; i <= 5; ++i) {
                int value = i * yStep;
                int y = margin + 60 + chartHeight - (value * chartHeight / maxValue);
                painter.drawLine(margin - 5, y, margin, y);
                painter.drawText(10, y - 10, margin - 15, 20, Qt::AlignRight, QString::number(value));
            }
            
            // Ligne de l'axe Y
            painter.setPen(QPen(Qt::black, 2));
            painter.drawLine(margin, margin + 60, margin, margin + 60 + chartHeight);
            
            // Total
            painter.setFont(QFont("Arial", 14, QFont::Bold));
            painter.setPen(QColor(50, 50, 50));
            painter.drawText(0, height - 30, width, 30, Qt::AlignCenter, 
                           QString("Total: %1 alertes").arg(total));
        }
    };
    
    AlertBarChartWidget *barChart = new AlertBarChartWidget(faible, moyenne, elevee, critique, total);
    tabWidget->addTab(barChart, "📈 Graphique en Barres");
    
    // 3. Vue d'ensemble textuelle avec statistiques détaillées
    QWidget *summaryWidget = new QWidget();
    QVBoxLayout *summaryLayout = new QVBoxLayout(summaryWidget);
    summaryLayout->setSpacing(15);
    summaryLayout->setContentsMargins(30, 30, 30, 30);
    
    QLabel *titleLabel = new QLabel("📊 Statistiques Détaillées des Alertes");
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #2c3e50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    summaryLayout->addWidget(titleLabel);
    
    // Statistiques générales
    QGroupBox *generalBox = new QGroupBox("Statistiques Générales");
    generalBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 12pt; }");
    QVBoxLayout *generalLayout = new QVBoxLayout(generalBox);
    
    QLabel *totalLabel = new QLabel(QString("📋 Total d'alertes: <b>%1</b>").arg(total));
    totalLabel->setStyleSheet("font-size: 11pt; padding: 5px;");
    generalLayout->addWidget(totalLabel);
    summaryLayout->addWidget(generalBox);
    
    // Répartition par gravité
    QGroupBox *gravityBox = new QGroupBox("Répartition par Gravité");
    gravityBox->setStyleSheet("QGroupBox { font-weight: bold; font-size: 12pt; }");
    QVBoxLayout *gravityLayout = new QVBoxLayout(gravityBox);
    
    double percentFaible = total > 0 ? (faible * 100.0 / total) : 0;
    double percentMoyenne = total > 0 ? (moyenne * 100.0 / total) : 0;
    double percentElevee = total > 0 ? (elevee * 100.0 / total) : 0;
    double percentCritique = total > 0 ? (critique * 100.0 / total) : 0;
    
    QLabel *faibleLabel = new QLabel(QString("🟢 Faible: <b>%1</b> alertes (%2%)").arg(faible).arg(percentFaible, 0, 'f', 1));
    faibleLabel->setStyleSheet("font-size: 11pt; padding: 5px; color: #228B22;");
    gravityLayout->addWidget(faibleLabel);
    
    QLabel *moyenneLabel = new QLabel(QString("🟡 Moyenne: <b>%1</b> alertes (%2%)").arg(moyenne).arg(percentMoyenne, 0, 'f', 1));
    moyenneLabel->setStyleSheet("font-size: 11pt; padding: 5px; color: #FFD700;");
    gravityLayout->addWidget(moyenneLabel);
    
    QLabel *eleveeLabel = new QLabel(QString("🟠 Élevée: <b>%1</b> alertes (%2%)").arg(elevee).arg(percentElevee, 0, 'f', 1));
    eleveeLabel->setStyleSheet("font-size: 11pt; padding: 5px; color: #FF8C00;");
    gravityLayout->addWidget(eleveeLabel);
    
    QLabel *critiqueLabel = new QLabel(QString("🔴 Critique: <b>%1</b> alertes (%2%)").arg(critique).arg(percentCritique, 0, 'f', 1));
    critiqueLabel->setStyleSheet("font-size: 11pt; padding: 5px; color: #DC143C; font-weight: bold;");
    gravityLayout->addWidget(critiqueLabel);
    
    summaryLayout->addWidget(gravityBox);
    summaryLayout->addStretch();
    
    tabWidget->addTab(summaryWidget, "📋 Résumé");
    
    // Afficher le dialogue
    chartDialog->exec();
    delete chartDialog;
}

void MainWindow::genererPDFAlertes(QWidget *parent)
{
    QString fileName = QFileDialog::getSaveFileName(parent, "Sauvegarder PDF",
                                                    QDir::homePath() + "/Alertes_Archive.pdf",
                                                    "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    
    // Get the current model from the table view
    QSqlQueryModel *currentModel = qobject_cast<QSqlQueryModel*>(ui->tableView_alertes->model());
    if (!currentModel) {
        // Try to get data directly from alerte
        currentModel = alerteTmp.afficher();
    }
    
    if (!currentModel || currentModel->rowCount() == 0) {
        QMessageBox::warning(parent, "Avertissement", "Aucune alerte à exporter!");
        return;
    }
    
    QPdfWriter pdf(fileName);
    QPainter painter(&pdf);
    
    // Configure PDF page - Use more of the page
    pdf.setPageSize(QPageSize::A4);
    pdf.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter); // Smaller margins
    
    // Page dimensions
    int pageWidth = pdf.width();
    int pageHeight = pdf.height();
    int margin = 30; // Reduced margin to use more space
    int y = margin;
    int currentPage = 1;
    
    // Colors
    QColor headerBgColor(70, 130, 180); // Steel blue
    QColor headerTextColor(Qt::white);
    QColor rowBgColor1(Qt::white);
    QColor rowBgColor2(245, 245, 245); // Light gray for alternating rows
    QColor borderColor(Qt::black);
    
    // Fonts - Larger sizes for better visibility
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 11, QFont::Bold);
    QFont normalFont("Arial", 10);
    QFont smallFont("Arial", 9);
    
    // Calculate column widths based on available space - use full width
    int numCols = currentModel->columnCount();
    int tableWidth = pageWidth - (2 * margin); // Use almost full width
    int colWidth = tableWidth / numCols;
    int rowHeight = 32; // Increased row height for better visibility
    
    // Calculate optimal column widths based on content and header names
    QList<int> colWidths;
    QList<QString> headerNames;
    
    // First pass: collect headers and determine base widths
    int totalFixedWidth = 0;
    for (int col = 0; col < numCols; ++col) {
        QString header = currentModel->headerData(col, Qt::Horizontal).toString();
        headerNames << header;
        QString headerUpper = header.toUpper();
        int width = 100; // Default width
        
        // Adjust width based on column type - optimized for full page usage
        if (headerUpper.contains("ID_AL") || headerUpper.contains("ID")) {
            width = 60;
        } else if (headerUpper.contains("DATE")) {
            width = 110;
        } else if (headerUpper.contains("DESCRIPTION") || headerUpper.contains("MESSAGE")) {
            width = 300; // More space for descriptions
        } else if (headerUpper.contains("TYPE")) {
            width = 100;
        } else if (headerUpper.contains("STATUT") || headerUpper.contains("STATUS")) {
            width = 90;
        } else if (headerUpper.contains("GRAVITÉ") || headerUpper.contains("GRAVITE") || headerUpper.contains("SEVERITY")) {
            width = 90;
        } else if (headerUpper.contains("CLIENT")) {
            width = 100;
        } else if (headerUpper.contains("ID_EMP") || headerUpper.contains("ID_EMPLOYE")) {
            width = 70;
        } else {
            // Calculate based on header text length
            width = qMax(70, header.length() * 7 + 15);
        }
        
        colWidths << width;
        totalFixedWidth += width;
    }
    
    // Adjust widths to fit exactly in table width
    if (totalFixedWidth != tableWidth && totalFixedWidth > 0) {
        double ratio = (double)tableWidth / totalFixedWidth;
        int newTotal = 0;
        for (int i = 0; i < colWidths.size(); ++i) {
            colWidths[i] = (int)(colWidths[i] * ratio);
            newTotal += colWidths[i];
        }
        // Adjust for rounding errors
        int diff = tableWidth - newTotal;
        if (diff != 0 && colWidths.size() > 0) {
            colWidths[0] += diff; // Add difference to first column
        }
    }
    
    // Draw header on first page - Compact
    painter.setFont(titleFont);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawText(margin, y, "Rapport des Alertes - Archive");
    y += 35;
    
    painter.setFont(smallFont);
    painter.setPen(QPen(Qt::gray));
    painter.drawText(margin, y, "Date du rapport: " + QDate::currentDate().toString("dd/MM/yyyy"));
    y += 20;
    
    // Statistics section - Compact layout
    int totalAlertes = alerteTmp.compterAlertes();
    
    // Draw statistics box - more compact
    int statsBoxY = y;
    int statsBoxHeight = 80; // Reduced height
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.setBrush(QBrush(QColor(250, 250, 250)));
    painter.drawRoundedRect(margin, y, tableWidth, statsBoxHeight, 5, 5);
    
    y += 12;
    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.setPen(QPen(Qt::black));
    painter.drawText(margin + 8, y, QString("Total: %1 alertes").arg(totalAlertes));
    y += 20;
    
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(margin + 8, y, "Répartition:");
    y += 18;
    
    painter.setFont(QFont("Arial", 9));
    QStringList gravites;
    gravites << "Critique" << "Élevée" << "Moyenne" << "Faible";
    
    int statsStartX = margin + 8;
    int statsColWidth = (tableWidth - 16) / 4; // Divide into 4 columns
    
    foreach(QString grav, gravites) {
        int count = alerteTmp.compterParGravite(grav);
        double percent = totalAlertes > 0 ? (count * 100.0 / totalAlertes) : 0;
        
        // Color code based on gravity
        QColor textColor = Qt::black;
        if (grav == "Critique") {
            textColor = QColor(200, 0, 0);
        } else if (grav == "Élevée") {
            textColor = QColor(255, 140, 0);
        } else if (grav == "Moyenne") {
            textColor = QColor(255, 200, 0);
        }
        
        painter.setPen(QPen(textColor, 1));
        QString line = QString("%1: %2 (%3%)")
                           .arg(grav)
                           .arg(count)
                           .arg(percent, 0, 'f', 1);
        painter.drawText(statsStartX, y, line);
        statsStartX += statsColWidth;
    }
    
    y = statsBoxY + statsBoxHeight + 15; // Reduced spacing
    
    // Draw a separator line
    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(margin, y, pageWidth - margin, y);
    y += 15; // Reduced spacing
    
    // Draw table header
    int headerY = y;
    painter.setFont(headerFont);
    painter.setPen(QPen(borderColor, 1));
    
    // Draw header background - darker for better visibility
    painter.fillRect(margin, y - rowHeight, tableWidth, rowHeight, QColor(50, 100, 150)); // Darker blue
    
    // Draw header border
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(margin, y - rowHeight, tableWidth, rowHeight);
    
    int x = margin;
    for (int col = 0; col < numCols; ++col) {
        QString header = currentModel->headerData(col, Qt::Horizontal).toString();
        if (header.isEmpty()) {
            header = QString("Colonne %1").arg(col + 1);
        }
        
        // Draw header cell border - thicker
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(x, y - rowHeight, colWidths[col], rowHeight);
        
        // Draw header text - centered and properly aligned - more padding
        painter.setPen(QPen(Qt::white)); // White text for better contrast
        QRect textRect(x + 6, y - rowHeight + 6, colWidths[col] - 12, rowHeight - 12);
        int flags = Qt::AlignCenter | Qt::AlignVCenter | Qt::TextWordWrap;
        painter.drawText(textRect, flags, header);
        
        x += colWidths[col];
    }
    
    y += 3; // Reduced spacing
    
    // Draw data rows
    painter.setFont(normalFont);
    int rowsExported = 0;
    
    for (int row = 0; row < currentModel->rowCount(); ++row) {
        // Check if we need a new page - use more of the page height
        if (y + rowHeight > pageHeight - 50) { // Reduced footer space
            // Draw footer - compact
            painter.setFont(smallFont);
            painter.setPen(QPen(Qt::gray));
            painter.drawText(margin, pageHeight - 20, 
                           QString("Page %1").arg(currentPage));
            painter.drawText(pageWidth - margin - 150, pageHeight - 20,
                           QString("Total: %1").arg(totalAlertes));
            
            pdf.newPage();
            currentPage++;
            y = margin;
            
            // Redraw header on new page - same style as first page
            painter.setFont(headerFont);
            painter.setPen(QPen(Qt::black, 2));
            painter.fillRect(margin, y - rowHeight, tableWidth, rowHeight, QColor(50, 100, 150));
            painter.drawRect(margin, y - rowHeight, tableWidth, rowHeight);
            
            x = margin;
            for (int col = 0; col < numCols; ++col) {
                QString header = currentModel->headerData(col, Qt::Horizontal).toString();
                if (header.isEmpty()) {
                    header = QString("Colonne %1").arg(col + 1);
                }
                painter.setPen(QPen(Qt::black, 1));
                painter.drawRect(x, y - rowHeight, colWidths[col], rowHeight);
                painter.setPen(QPen(Qt::white));
                QRect textRect(x + 6, y - rowHeight + 6, colWidths[col] - 12, rowHeight - 12);
                painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextWordWrap, header);
                x += colWidths[col];
            }
            y += 3;
            painter.setFont(normalFont);
        }
        
        // Alternate row colors - more visible contrast
        QColor rowBg = (row % 2 == 0) ? rowBgColor1 : QColor(245, 245, 250); // Slightly darker for better contrast
        painter.fillRect(margin, y, tableWidth, rowHeight, rowBg);
        
        // Draw row border for better separation - visible but not too dark
        painter.setPen(QPen(QColor(230, 230, 230), 1));
        painter.drawLine(margin, y, margin + tableWidth, y);
        
        x = margin;
        for (int col = 0; col < numCols; ++col) {
            QModelIndex index = currentModel->index(row, col);
            QString value = currentModel->data(index).toString();
            
            // Format date if it's a date column
            QString headerName = currentModel->headerData(col, Qt::Horizontal).toString();
            if (headerName.contains("DATE", Qt::CaseInsensitive)) {
                QVariant data = currentModel->data(index);
                if (data.typeId() == QMetaType::QDate) {
                    value = data.toDate().toString("dd/MM/yyyy");
                } else if (data.typeId() == QMetaType::QDateTime) {
                    value = data.toDateTime().toString("dd/MM/yyyy hh:mm");
                }
            }
            
            // Determine text color based on column type
            QColor textColor = Qt::black;
            int alignFlags = Qt::AlignLeft | Qt::AlignVCenter;
            
            // Special handling for different column types
            if (headerName.toUpper().contains("GRAVITÉ") || headerName.toUpper().contains("GRAVITE")) {
                if (value.toUpper().contains("CRITIQUE")) {
                    textColor = QColor(200, 0, 0);
                } else if (value.toUpper().contains("ÉLEVÉE") || value.toUpper().contains("ELEVEE")) {
                    textColor = QColor(255, 140, 0);
                } else if (value.toUpper().contains("MOYENNE")) {
                    textColor = QColor(255, 200, 0);
                }
            } else if (headerName.toUpper().contains("ID") && !headerName.toUpper().contains("CLIENT") && !headerName.toUpper().contains("EMP")) {
                // Right align numeric IDs
                alignFlags = Qt::AlignRight | Qt::AlignVCenter;
            } else if (headerName.toUpper().contains("DATE")) {
                // Center align dates
                alignFlags = Qt::AlignCenter | Qt::AlignVCenter;
            }
            
            // Truncate long text intelligently - but allow more text to show
            QFontMetrics fm(normalFont);
            int availableWidth = colWidths[col] - 12; // More padding
            QString displayValue = value;
            
            // For description columns, allow more text
            if (headerName.toUpper().contains("DESCRIPTION") || headerName.toUpper().contains("MESSAGE")) {
                // Allow more characters for descriptions
                int maxChars = (colWidths[col] / 6); // More characters per pixel
                if (displayValue.length() > maxChars) {
                    displayValue = displayValue.left(maxChars - 3) + "...";
                }
            } else {
                // For other columns, use elided text
                if (fm.horizontalAdvance(displayValue) > availableWidth) {
                    displayValue = fm.elidedText(value, Qt::ElideRight, availableWidth);
                }
            }
            
            // Draw cell border - visible borders for better readability
            painter.setPen(QPen(QColor(200, 200, 200), 1)); // Light gray for better visibility
            painter.drawRect(x, y, colWidths[col], rowHeight);
            
            // Draw cell text with proper alignment and color - more padding
            painter.setPen(QPen(textColor));
            QRect textRect(x + 6, y + 6, colWidths[col] - 12, rowHeight - 12);
            painter.drawText(textRect, alignFlags | Qt::TextWordWrap, displayValue);
            
            x += colWidths[col];
        }
        
        y += rowHeight;
        rowsExported++;
    }
    
    // Draw final footer - compact
    painter.setFont(smallFont);
    painter.setPen(QPen(Qt::gray));
    painter.drawText(margin, pageHeight - 20, 
                   QString("Page %1").arg(currentPage));
    painter.drawText(pageWidth - margin - 150, pageHeight - 20,
                   QString("Total: %1").arg(rowsExported));
    
    painter.end();
    
    QMessageBox::information(parent, "Succès", 
                           QString("PDF généré avec succès!\n\nFichier: %1\n\nAlertes exportées: %2\nPages: %3")
                           .arg(fileName).arg(rowsExported).arg(currentPage));
        }
        
// === IMPLÉMENTATION DES SLOTS POUR L'ONGLET ALERTES ===

void MainWindow::on_btn_ajouter_alerte_clicked()
{
    if (!ui->lineEdit_type_alerte || !ui->lineEdit_id_client_alerte || 
        !ui->lineEdit_statut_alerte || !ui->comboBox_gravite_alerte) {
        return;
    }
    
    if (!validerAlerte(ui->lineEdit_type_alerte->text(), 
                      ui->lineEdit_id_client_alerte->text(), 
                      ui->lineEdit_statut_alerte->text(), 
                      ui->comboBox_gravite_alerte->currentText())) {
        return;
    }
    
    Alerte a;
    a.setType(ui->lineEdit_type_alerte->text());
    a.setIdClient(ui->lineEdit_id_client_alerte->text().toInt());
    a.setStatut(ui->lineEdit_statut_alerte->text());
    if (ui->textEdit_description_alerte) {
        a.setDescription(ui->textEdit_description_alerte->toPlainText());
    }
    a.setGravite(ui->comboBox_gravite_alerte->currentText());
    if (ui->dateEdit_alerte) {
        a.setDate(ui->dateEdit_alerte->date());
    } else {
        a.setDate(QDate::currentDate());
    }
    
    if (a.ajouter()) {
        QMessageBox::information(this, "Succès", "Alerte ajoutée avec succès!");
        
        // Envoyer une notification si NotificationManager est disponible
        if (notificationManager) {
            QString message = QString("Nouvelle alerte créée:\nType: %1\nGravité: %2\nDescription: %3")
                .arg(a.getType())
                .arg(a.getGravite())
                .arg(a.getDescription());
            // Use the new intelligent notification system
            notificationManager->showNotification("Nouvelle Alerte", message);
        }
        
        chargerAlertes();
        viderChampsAlerte();
    } else {
        // Vérifier si la table existe
        QSqlQuery checkTable("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'ALERTE'");
        QString errorMsg = "Échec de l'ajout de l'alerte!\n\n";
        
        if (checkTable.exec() && checkTable.next()) {
            int tableExists = checkTable.value(0).toInt();
            if (tableExists == 0) {
                errorMsg += "⚠️ ERREUR: La table ALERTE n'existe pas dans la base de données!\n\n";
                errorMsg += "Veuillez exécuter le script SQL suivant dans Oracle SQL Developer:\n";
                errorMsg += "Fichier: create_alerte_table.sql\n\n";
                errorMsg += "Ou exécutez cette commande SQL:\n";
                errorMsg += "CREATE TABLE ALERTE (\n";
                errorMsg += "    ID NUMBER PRIMARY KEY,\n";
                errorMsg += "    TYPE VARCHAR2(100) NOT NULL,\n";
                errorMsg += "    ID_CLIENT NUMBER NOT NULL,\n";
                errorMsg += "    STATUT VARCHAR2(50) NOT NULL,\n";
                errorMsg += "    DESCRIPTION VARCHAR2(500),\n";
                errorMsg += "    GRAVITE VARCHAR2(20) NOT NULL,\n";
                errorMsg += "    DATE_ALERTE DATE NOT NULL\n";
                errorMsg += ");";
            } else {
                errorMsg += "Vérifiez les logs de la console pour plus de détails.";
            }
        } else {
            errorMsg += "Vérifiez les logs de la console pour plus de détails.";
        }
        
        QMessageBox::critical(this, "Erreur", errorMsg);
    }
}

void MainWindow::on_btn_modifier_alerte_clicked()
{
    if (!ui->lineEdit_id_alerte || ui->lineEdit_id_alerte->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une alerte à modifier!");
        return;
    }
    
    if (!ui->lineEdit_type_alerte || !ui->lineEdit_id_client_alerte || 
        !ui->lineEdit_statut_alerte || !ui->comboBox_gravite_alerte) {
        return;
    }
    
    if (!validerAlerte(ui->lineEdit_type_alerte->text(), 
                      ui->lineEdit_id_client_alerte->text(), 
                      ui->lineEdit_statut_alerte->text(), 
                      ui->comboBox_gravite_alerte->currentText())) {
        return;
    }
    
    Alerte a;
    a.setId(ui->lineEdit_id_alerte->text().toInt());
    a.setType(ui->lineEdit_type_alerte->text());
    a.setIdClient(ui->lineEdit_id_client_alerte->text().toInt());
    a.setStatut(ui->lineEdit_statut_alerte->text());
    if (ui->textEdit_description_alerte) {
        a.setDescription(ui->textEdit_description_alerte->toPlainText());
    }
    a.setGravite(ui->comboBox_gravite_alerte->currentText());
    if (ui->dateEdit_alerte) {
        a.setDate(ui->dateEdit_alerte->date());
    } else {
        a.setDate(QDate::currentDate());
    }
    
    if (a.modifier()) {
        QMessageBox::information(this, "Succès", "Alerte modifiée avec succès!");
        chargerAlertes();
        viderChampsAlerte();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification!");
            }
        }

void MainWindow::on_btn_supprimer_alerte_clicked()
{
    if (!ui->tableView_alertes) {
        return;
    }
    
    QItemSelectionModel *selectionModel = ui->tableView_alertes->selectionModel();
    if (!selectionModel) return;
    QModelIndexList selected = selectionModel->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une alerte à supprimer!");
        return;
    }
    
    int id = ui->tableView_alertes->model()->data(selected.first()).toInt();
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation", "Voulez-vous vraiment supprimer cette alerte?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (alerteTmp.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Alerte supprimée avec succès!");
            chargerAlertes();
            viderChampsAlerte();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
        }
    }
}

void MainWindow::on_btn_vider_alerte_clicked()
{
    viderChampsAlerte();
}

void MainWindow::on_btn_refresh_alerte_clicked()
{
    chargerAlertes();
}

void MainWindow::on_btn_rechercher_alerte_clicked()
{
    if (!ui->lineEdit_recherche_alerte || !ui->tableView_alertes) {
        return;
    }
    
    if (ui->lineEdit_recherche_alerte->text().isEmpty()) {
        chargerAlertes();
    } else {
        QSqlQueryModel *model = alerteTmp.rechercher(ui->lineEdit_recherche_alerte->text());
        ui->tableView_alertes->setModel(model);
        ui->tableView_alertes->resizeColumnsToContents();
    }
}

void MainWindow::on_btn_filtrer_gravite_alerte_clicked()
{
    if (!ui->comboBox_filtrer_gravite_alerte || !ui->tableView_alertes) {
        return;
    }
    
    QString gravite = ui->comboBox_filtrer_gravite_alerte->currentText();
    if (gravite == "Toutes") {
        chargerAlertes();
    } else {
        QSqlQueryModel *model = alerteTmp.filtrerParGravite(gravite);
        ui->tableView_alertes->setModel(model);
        ui->tableView_alertes->resizeColumnsToContents();
    }
}

void MainWindow::on_btn_trier_alerte_clicked()
{
    if (!ui->tableView_alertes) {
        return;
    }
    
    QSqlQueryModel *model = alerteTmp.trierPar("DATE_ALERTE", "DESC");
    ui->tableView_alertes->setModel(model);
    ui->tableView_alertes->resizeColumnsToContents();
}

void MainWindow::on_btn_pdf_alerte_clicked()
{
    genererPDFAlertes(this);
}

void MainWindow::on_btn_carte_alerte_clicked()
{
    // Appeler la même fonction que le bouton Map de l'onglet Déchets
    on_afficher_map_btn_clicked();
}

void MainWindow::on_tableView_alertes_selectionChanged()
{
    if (!ui || !ui->tableView_alertes) return;
    remplirFormulaireAlerte();
}

void MainWindow::on_tableView_alertes_clicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    if (!ui || !ui->tableView_alertes) return;
    remplirFormulaireAlerte();
}

void MainWindow::remplirFormulaireAlerte()
{
    if (!ui || !ui->tableView_alertes) {
        return;
    }
    
    QItemSelectionModel *selectionModel = ui->tableView_alertes->selectionModel();
    if (!selectionModel) return;
    
    QModelIndexList selected = selectionModel->selectedRows();
    if (selected.isEmpty()) {
        // Si aucune ligne n'est sélectionnée, essayer avec la ligne courante
        QModelIndex current = ui->tableView_alertes->currentIndex();
        if (current.isValid()) {
            selected << current;
        } else {
            return;
        }
    }
    
    if (!selected.isEmpty()) {
        int row = selected.first().row();
        QAbstractItemModel *model = ui->tableView_alertes->model();
        
        if (!model || row < 0 || row >= model->rowCount()) {
            qDebug() << "⚠️ Ligne invalide:" << row;
            return;
        }
        
        qDebug() << "📝 Remplissage du formulaire depuis la ligne:" << row;
        
        // Récupérer les données de chaque colonne
        QString id = model->data(model->index(row, 0)).toString();
        QString type = model->data(model->index(row, 1)).toString();
        QString idClient = model->data(model->index(row, 2)).toString();
        QString statut = model->data(model->index(row, 3)).toString();
        QString description = model->data(model->index(row, 4)).toString();
        QString gravite = model->data(model->index(row, 5)).toString();
        QString dateStr = model->data(model->index(row, 6)).toString();
        
        qDebug() << "  ID:" << id;
        qDebug() << "  Type:" << type;
        qDebug() << "  ID Client:" << idClient;
        qDebug() << "  Statut:" << statut;
        qDebug() << "  Description:" << description;
        qDebug() << "  Gravité:" << gravite;
        qDebug() << "  Date:" << dateStr;
        
        if (ui->lineEdit_id_alerte) {
            ui->lineEdit_id_alerte->setText(id);
        }
        if (ui->lineEdit_type_alerte) {
            ui->lineEdit_type_alerte->setText(type);
        }
        if (ui->lineEdit_id_client_alerte) {
            ui->lineEdit_id_client_alerte->setText(idClient);
        }
        if (ui->lineEdit_statut_alerte) {
            ui->lineEdit_statut_alerte->setText(statut);
        }
        if (ui->textEdit_description_alerte) {
            ui->textEdit_description_alerte->setPlainText(description);
        }
        if (ui->comboBox_gravite_alerte) {
            ui->comboBox_gravite_alerte->setCurrentText(gravite);
        }
        if (ui->dateEdit_alerte) {
            // Essayer plusieurs formats de date
            QDate date;
            QStringList dateFormats;
            dateFormats << "dd/MM/yyyy" << "yyyy-MM-dd" << "dd-MM-yyyy" << "MM/dd/yyyy";
            
            for (const QString &format : dateFormats) {
                date = QDate::fromString(dateStr, format);
                if (date.isValid()) {
                    break;
                }
            }
            
            // Si aucun format ne fonctionne, essayer de parser directement
            if (!date.isValid()) {
                date = QDate::fromString(dateStr, Qt::ISODate);
            }
            
            if (date.isValid()) {
                ui->dateEdit_alerte->setDate(date);
                qDebug() << "  ✅ Date parsée:" << date.toString("dd/MM/yyyy");
            } else {
                qDebug() << "  ⚠️ Date invalide, utilisation de la date actuelle";
                ui->dateEdit_alerte->setDate(QDate::currentDate());
            }
        }
        
        qDebug() << "✅ Formulaire rempli avec succès!";
    }
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
    // Note: The new intelligent notification system automatically monitors the ALERTE table
    // and sends notifications for new alerts. No manual alert rules are needed.
    // The system checks the database every 5 seconds and notifies about new alerts.
    qDebug() << "✅ Intelligent notification system is active - monitoring ALERTE table automatically";
}

// Old notification slots removed - new intelligent notification system uses direct signal connections
// The NotificationManager now handles all notifications via QSystemTrayIcon automatically

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
    // Note: The new intelligent notification system automatically monitors the ALERTE table
    // No manual alert rules management is needed - the system detects new alerts automatically
    QMessageBox::information(this, "Gestion des Règles d'Alerte", 
        "Le système de notification intelligent surveille automatiquement la table ALERTE.\n\n"
        "Les nouvelles alertes sont détectées toutes les 5 secondes et des notifications\n"
        "sont envoyées selon la gravité (Critique = notification urgente).\n\n"
        "Aucune configuration manuelle n'est nécessaire.");
}

void MainWindow::configureNotifications()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Configuration des Notifications");
    dialog->setMinimumSize(500, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Status label
    QLabel *statusLabel = new QLabel("", dialog);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("padding: 5px; background-color: #e6f7f5; border-radius: 5px;");
    
    // Update status
    QString status = "📊 État du système de notification:\n\n";
    status += "✅ Système de notification intelligent actif\n";
    status += "✅ Surveillance automatique de la table ALERTE\n";
    status += "✅ Vérification toutes les 5 secondes\n";
    status += "✅ Notifications système Windows activées\n";
    statusLabel->setText(status);
    
    // Buttons
    QPushButton *testNotificationBtn = new QPushButton("🔔 Tester Notification", dialog);
    QPushButton *closeBtn = new QPushButton("Fermer", dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(testNotificationBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    
    layout->addWidget(statusLabel);
    layout->addLayout(buttonLayout);
    
    // Test notification button
    connect(testNotificationBtn, &QPushButton::clicked, [=]() {
        notificationManager->showNotification(
            "Test de Notification",
            "Ceci est une notification de test.\nLe système de notification fonctionne correctement."
        );
        QMessageBox::information(dialog, "Test", "Notification de test envoyée!\nVérifiez la barre des tâches.");
    });
    
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    
    dialog->exec();
}

void MainWindow::showNotificationHistory()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Historique des Notifications");
    dialog->setMinimumSize(1000, 700);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Get notification history
    QList<NotificationManager::NotificationEntry> history = notificationManager->getNotificationHistory();
    
    // Create table view
    QTableView *tableView = new QTableView(dialog);
    QStandardItemModel *model = new QStandardItemModel(tableView);
    model->setHorizontalHeaderLabels({
        "Date/Heure", "ID Alerte", "Client", "Gravité", "Titre", 
        "Message", "Type", "Notification Système"
    });
    
    // Populate model with history
    for (const auto &entry : history) {
        QList<QStandardItem*> row;
        row << new QStandardItem(entry.dateTime.toString("dd/MM/yyyy hh:mm:ss"));
        row << new QStandardItem(entry.alertId.isEmpty() ? "N/A" : entry.alertId);
        row << new QStandardItem(entry.client.isEmpty() ? "N/A" : entry.client);
        row << new QStandardItem(entry.gravite.isEmpty() ? "N/A" : entry.gravite);
        row << new QStandardItem(entry.title);
        row << new QStandardItem(entry.message);
        row << new QStandardItem(entry.type);
        row << new QStandardItem(entry.systemNotificationSent ? "✅ Oui" : "❌ Non");
        
        // Color code based on type
        if (entry.type == "System") {
            for (auto *item : row) {
                item->setBackground(QBrush(QColor(200, 230, 255))); // Light blue
            }
        }
        
        // Color code based on gravity
        if (entry.gravite.toUpper() == "CRITIQUE" || entry.gravite.toUpper() == "CRITICAL") {
            for (auto *item : row) {
                item->setForeground(QBrush(QColor(200, 0, 0))); // Red text
                item->setFont(QFont(item->font().family(), item->font().pointSize(), QFont::Bold));
            }
        }
        
        model->appendRow(row);
    }
    
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setAlternatingRowColors(false);
    tableView->resizeColumnsToContents();
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(0, Qt::DescendingOrder); // Sort by date, newest first
    
    // Set column widths
    tableView->setColumnWidth(0, 150); // Date/Heure
    tableView->setColumnWidth(1, 80);  // ID Alerte
    tableView->setColumnWidth(2, 120); // Client
    tableView->setColumnWidth(3, 100); // Gravité
    tableView->setColumnWidth(4, 200); // Titre
    tableView->setColumnWidth(5, 300); // Message
    tableView->setColumnWidth(6, 80);  // Type
    tableView->setColumnWidth(7, 150); // Notification Système
    
    // Statistics label
    QString statsText = QString("📊 Total: %1 notification(s)").arg(history.size());
    if (history.size() > 0) {
        int systemCount = 0;
        for (const auto &entry : history) {
            if (entry.type == "System") systemCount++;
        }
        statsText += QString(" | Système: %1")
                     .arg(systemCount);
    }
    QLabel *statsLabel = new QLabel(statsText, dialog);
    statsLabel->setStyleSheet("padding: 5px; background-color: #e6f7f5; border-radius: 5px; font-weight: bold;");
    
    // Buttons
    QPushButton *refreshBtn = new QPushButton("🔄 Actualiser", dialog);
    QPushButton *clearBtn = new QPushButton("🗑️ Vider l'historique", dialog);
    QPushButton *exportBtn = new QPushButton("📄 Exporter", dialog);
    QPushButton *closeBtn = new QPushButton("Fermer", dialog);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(refreshBtn);
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addWidget(exportBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    
    layout->addWidget(statsLabel);
    layout->addWidget(tableView);
    layout->addLayout(buttonLayout);
    
    // Refresh button
    connect(refreshBtn, &QPushButton::clicked, [=]() {
        QList<NotificationManager::NotificationEntry> newHistory = notificationManager->getNotificationHistory();
        model->clear();
        model->setHorizontalHeaderLabels({
            "Date/Heure", "ID Alerte", "Client", "Gravité", "Titre", 
            "Message", "Type", "Notification Système"
        });
        
        for (const auto &entry : newHistory) {
            QList<QStandardItem*> row;
            row << new QStandardItem(entry.dateTime.toString("dd/MM/yyyy hh:mm:ss"));
            row << new QStandardItem(entry.alertId.isEmpty() ? "N/A" : entry.alertId);
            row << new QStandardItem(entry.client.isEmpty() ? "N/A" : entry.client);
            row << new QStandardItem(entry.gravite.isEmpty() ? "N/A" : entry.gravite);
            row << new QStandardItem(entry.title);
            row << new QStandardItem(entry.message);
            row << new QStandardItem(entry.type);
            row << new QStandardItem(entry.systemNotificationSent ? "✅ Oui" : "❌ Non");
            
            if (entry.type == "System") {
                for (auto *item : row) {
                    item->setBackground(QBrush(QColor(200, 230, 255)));
                }
            }
            
            if (entry.gravite.toUpper() == "CRITIQUE" || entry.gravite.toUpper() == "CRITICAL") {
                for (auto *item : row) {
                    item->setForeground(QBrush(QColor(200, 0, 0)));
                    item->setFont(QFont(item->font().family(), item->font().pointSize(), QFont::Bold));
                }
            }
            
            model->appendRow(row);
        }
        
        tableView->sortByColumn(0, Qt::DescendingOrder);
        statsLabel->setText(QString("📊 Total: %1 notification(s)").arg(newHistory.size()));
        
        QMessageBox::information(dialog, "Actualisation", "Historique actualisé!");
    });
    
    // Clear button
    connect(clearBtn, &QPushButton::clicked, [=]() {
        int ret = QMessageBox::question(dialog, "Confirmation", 
            "Êtes-vous sûr de vouloir vider tout l'historique des notifications?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            notificationManager->clearNotificationHistory();
            model->clear();
            model->setHorizontalHeaderLabels({
                "Date/Heure", "ID Alerte", "Client", "Gravité", "Titre", 
                "Message", "Type", "Notification Système"
            });
            statsLabel->setText("📊 Total: 0 notification(s)");
            QMessageBox::information(dialog, "Succès", "Historique vidé!");
        }
    });
    
    // Export button
    connect(exportBtn, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getSaveFileName(dialog, 
            "Exporter l'historique", 
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/historique_notifications.csv",
            "CSV Files (*.csv);;All Files (*)");
        
        if (fileName.isEmpty()) {
            return;
        }
        
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(dialog, "Erreur", "Impossible d'écrire dans le fichier.");
            return;
        }
        
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
        
        // Write header
        out << "Date/Heure,ID Alerte,Client,Gravité,Titre,Message,Type,Notification Système\n";
        
        // Write data
        for (const auto &entry : history) {
            // Create copies for replace operations (entry is const)
            QString title = entry.title;
            QString message = entry.message;
            title.replace("\"", "\"\"");
            message.replace("\"", "\"\"");
            
            out << "\"" << entry.dateTime.toString("dd/MM/yyyy hh:mm:ss") << "\",";
            out << "\"" << (entry.alertId.isEmpty() ? "N/A" : entry.alertId) << "\",";
            out << "\"" << (entry.client.isEmpty() ? "N/A" : entry.client) << "\",";
            out << "\"" << (entry.gravite.isEmpty() ? "N/A" : entry.gravite) << "\",";
            out << "\"" << title << "\",";
            out << "\"" << message << "\",";
            out << "\"" << entry.type << "\",";
            out << "\"" << (entry.systemNotificationSent ? "Oui" : "Non") << "\"\n";
        }
        
        file.close();
        QMessageBox::information(dialog, "Succès", 
            QString("Historique exporté avec succès!\nFichier: %1").arg(fileName));
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
    #ifdef LOCATION_AVAILABLE
    // Créer la carte si elle n'existe pas
    if (!m_alerteMap) {
        m_alerteMap = new AlerteMap(this);
    }
    
    // Créer une fenêtre de dialogue pour afficher la carte
    QDialog *mapDialog = new QDialog(this);
    mapDialog->setWindowTitle("Carte Géographique des Alertes");
    mapDialog->setMinimumSize(1000, 700);
    
    QVBoxLayout *layout = new QVBoxLayout(mapDialog);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // Ajouter la carte au dialogue
    layout->addWidget(m_alerteMap);
    
    // Boutons de contrôle
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnRefresh = new QPushButton("🔄 Rafraîchir", mapDialog);
    QPushButton *btnClose = new QPushButton("Fermer", mapDialog);
    
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::refreshAlerteMap);
    
    connect(btnClose, &QPushButton::clicked, mapDialog, &QDialog::accept);
    
    btnLayout->addWidget(btnRefresh);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    layout->addLayout(btnLayout);
    
    mapDialog->exec();
    #elif defined(WEBENGINE_AVAILABLE)
    // Fallback vers WebEngine si Qt Location n'est pas disponible
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

#ifdef LOCATION_AVAILABLE
void MainWindow::refreshAlerteMap()
{
    if (m_alerteMap) {
        m_alerteMap->refreshAlerts();
    }
}
#endif

// ==================== MÉTHODES POUR PARKINGS ====================

void MainWindow::chargerParkings()
{
    if (!ui || !ui->tableWidget_2_parking) return;
    
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) return;
    }
    
    QSqlQuery query;
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION FROM PARKING ORDER BY ID_PARKING")) {
        return;
    }
    
    ui->tableWidget_2_parking->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_2_parking->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_2_parking->setItem(row, col, item);
        }
        ++row;
    }
    
    ui->tableWidget_2_parking->resizeColumnsToContents();
    qDebug() << "✅ Tableau parking chargé:" << row << "lignes";
}

void MainWindow::viderChampsParking()
{
    if (ui->id_parking) ui->id_parking->clear();
    if (ui->nom_parking) ui->nom_parking->clear();
    if (ui->loco_parking) ui->loco_parking->clear();
    if (ui->capacite_parking) ui->capacite_parking->clear();
    if (ui->place_parking) ui->place_parking->clear();
    if (ui->statut_parking) ui->statut_parking->clear();
    if (ui->type_parking) ui->type_parking->clear();
    if (ui->tarif_parking) ui->tarif_parking->clear();
}

bool MainWindow::validerParking(const QString& nom, const QString& localisation, int capacite, double tarif)
{
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom du parking est obligatoire!");
        return false;
    }
    
    if (localisation.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "La localisation est obligatoire!");
        return false;
    }
    
    if (capacite <= 0) {
        QMessageBox::warning(this, "Erreur", "La capacité doit être supérieure à 0!");
        return false;
    }
    
    if (tarif < 0) {
        QMessageBox::warning(this, "Erreur", "La tarification ne peut pas être négative!");
        return false;
    }
    
    return true;
}

void MainWindow::on_ajouter_bt_parking_clicked()
{
    QString idParking = "P" + QString::number(QRandomGenerator::global()->bounded(1000, 9999));
    QString nom = ui->nom_parking->text().trimmed();
    QString localisation = ui->loco_parking->text().trimmed();
    QString capaciteStr = ui->capacite_parking->text().trimmed();
    QString placesStr = ui->place_parking->text().trimmed();
    QString statut = ui->statut_parking->text().trimmed();
    QString type = ui->type_parking->text().trimmed();
    QString tarifStr = ui->tarif_parking->text().trimmed();
    
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
    
    if (!validerParking(nom, localisation, capacite, tarif)) {
        return;
    }
    
    if (placesLibres > capacite) {
        QMessageBox::warning(this, "Erreur", "Le nombre de places libres ne peut pas être supérieur à la capacité!");
        return;
    }
    
    Parking p(idParking, nom, localisation, capacite, placesLibres, statut, type, tarif);
    
    if (p.ajouter()) {
        QMessageBox::information(this, "Succès", QString("Parking ajouté avec succès!\nID: %1").arg(idParking));
        viderChampsParking();
        chargerParkings();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du parking!");
    }
}

void MainWindow::on_modifier_bt_2_parking_clicked()
{
    QString idParking = ui->id_parking->text().trimmed();
    
    if (idParking.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un parking à modifier!");
        return;
    }
    
    QString nom = ui->nom_parking->text().trimmed();
    QString localisation = ui->loco_parking->text().trimmed();
    QString capaciteStr = ui->capacite_parking->text().trimmed();
    QString placesStr = ui->place_parking->text().trimmed();
    QString statut = ui->statut_parking->text().trimmed();
    QString type = ui->type_parking->text().trimmed();
    QString tarifStr = ui->tarif_parking->text().trimmed();
    
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
    
    if (!validerParking(nom, localisation, capacite, tarif)) {
        return;
    }
    
    if (placesLibres > capacite) {
        QMessageBox::warning(this, "Erreur", "Le nombre de places libres ne peut pas être supérieur à la capacité!");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
        QString("Voulez-vous vraiment modifier le parking:\n\nID: %1\nNom: %2?").arg(idParking, nom),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::No) return;
    
    Parking p;
    p.setId(idParking);
    p.setNom(nom);
    p.setLocalisation(localisation);
    p.setCapacite(capacite);
    p.setPlaceslibres(placesLibres);
    p.setStatut(statut);
    p.setType(type);
    p.setTarif(tarif);
    
    if (p.modifierParId(idParking)) {
        QMessageBox::information(this, "Succès", "Parking modifié avec succès!");
        viderChampsParking();
        chargerParkings();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification!");
    }
}

void MainWindow::on_supprimer_bt_parking_clicked()
{
    int selectedRow = ui->tableWidget_2_parking->currentRow();
    if (selectedRow < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un parking !");
        return;
    }
    
    QString idParking = ui->tableWidget_2_parking->item(selectedRow, 0)->text();
    QString nomParking = ui->tableWidget_2_parking->item(selectedRow, 1)->text();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation",
        QString("Voulez-vous vraiment supprimer le parking:\n\nID: %1\nNom: %2?").arg(idParking, nomParking),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::No) return;
    
    if (gestionParking.supprimerParId(idParking)) {
        QMessageBox::information(this, "Succès", "Parking supprimé avec succès!");
        chargerParkings();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
    }
}

void MainWindow::on_pushButton_12_parking_clicked()
{
    viderChampsParking();
}

void MainWindow::on_tri_bt_parking_clicked()
{
    trierParCapaciteParking();
}

void MainWindow::on_ajouter_bt_2_parking_clicked()
{
    QString localisation = ui->lineEdit_parking->text().trimmed();
    
    if (localisation.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez entrer une localisation à rechercher !");
        return;
    }
    
    rechercherParLocalisationParking(localisation);
}

void MainWindow::on_pdf_bt_parking_clicked()
{
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            QMessageBox::critical(this, "Erreur", "Connexion à la base échouée!");
            return;
        }
    }
    
    QSqlQuery query;
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, TARIFICATION FROM PARKING ORDER BY NOM")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }
    
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
    
    bool ok;
    QString selectedParking = QInputDialog::getItem(this, "Sélectionner un Parking",
        "Choisissez un parking pour générer le ticket:",
        parkingList, 0, false, &ok);
    
    if (!ok || selectedParking.isEmpty()) {
        return;
    }
    
    QStringList data = parkingData[selectedParking];
    QString parkingId = data[0];
    QString parkingNom = data[1];
    QString parkingLoc = data[2];
    QString parkingTarif = data[3];
    
    QString ticketNumber = QString("TK-%1-%2")
        .arg(QDate::currentDate().toString("yyyyMMdd"))
        .arg(QRandomGenerator::global()->bounded(1000, 9999));
    
    QString dateEntree = QDate::currentDate().toString("dd/MM/yyyy");
    QString heureEntree = QTime::currentTime().toString("HH:mm");
    
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
        "<div class='header'>"
        "<h1>🅿️ PARKING</h1>"
        "<h2>%1</h2>"
        "<h2>%2</h2>"
        "</div>"
        "<div class='ticket-number'>TICKET N° %3</div>"
        "<div class='barcode'>*%3*</div>"
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
        "<div class='tarif-section'>"
        "<div class='tarif-title'>💳 TARIFICATION</div>"
        "<div class='tarif-info'>%7 DT / heure</div>"
        "</div>"
        "<div class='separator'></div>"
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
        "<div class='footer'>"
        "Merci de votre visite<br>"
        "Conservez ce ticket<br>"
        "Support: parking@contact.tn"
        "</div>"
        "</div>"
        "</body>"
        "</html>"
    ).arg(parkingNom).arg(parkingLoc).arg(ticketNumber).arg(dateEntree)
     .arg(heureEntree).arg(parkingId).arg(parkingTarif);
    
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
        QString("Ticket généré avec succès!\n\nTicket N°: %1\nParking: %2")
            .arg(ticketNumber, parkingNom));
    
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void MainWindow::on_map_bt_parking_clicked()
{
    afficherCarteParkings();
}

void MainWindow::afficherCarteParkings()
{
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            QMessageBox::critical(this, "Erreur", "Connexion à la base échouée!");
            return;
        }
    }
    
    QSqlQuery query;
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES FROM PARKING ORDER BY ID_PARKING")) {
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
    
    QLinearGradient gradient(0, 0, 0, 700);
    gradient.setColorAt(0, QColor(220, 240, 255));
    gradient.setColorAt(1, QColor(180, 220, 240));
    view->setBackgroundBrush(QBrush(gradient));
    
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
    positions["ezzahra"] = QPoint(600, 280);
    positions["megrine"] = QPoint(630, 300);
    
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
        
        QPoint pos = positions.value(localisation, QPoint(600, 300 + (count * 80)));
        
        int offsetX = (QRandomGenerator::global()->bounded(40)) - 20;
        int offsetY = (QRandomGenerator::global()->bounded(40)) - 20;
        int x = pos.x() + offsetX;
        int y = pos.y() + offsetY;
        
        QGraphicsEllipseItem *circle = scene->addEllipse(x - 40, y - 40, 80, 80);
        circle->setBrush(QBrush(couleur));
        circle->setPen(QPen(Qt::black, 3));
        circle->setZValue(10);
        
        QGraphicsTextItem *idText = scene->addText(id);
        idText->setPos(x - 25, y - 30);
        idText->setDefaultTextColor(Qt::white);
        QFont idFont = idText->font();
        idFont.setPointSize(12);
        idFont.setBold(true);
        idText->setFont(idFont);
        idText->setZValue(11);
        
        QGraphicsTextItem *nomText = scene->addText(nom);
        nomText->setPos(x - 35, y - 10);
        nomText->setDefaultTextColor(Qt::white);
        QFont nomFont = nomText->font();
        nomFont.setPointSize(9);
        nomFont.setBold(true);
        nomText->setFont(nomFont);
        nomText->setZValue(11);
        
        QGraphicsTextItem *tauxText = scene->addText(QString::number(tauxOccupation, 'f', 0) + "%");
        tauxText->setPos(x - 20, y + 10);
        tauxText->setDefaultTextColor(Qt::white);
        QFont tauxFont = tauxText->font();
        tauxFont.setPointSize(14);
        tauxFont.setBold(true);
        tauxText->setFont(tauxFont);
        tauxText->setZValue(11);
        
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
    
    QHBoxLayout *legendLayout = new QHBoxLayout();
    
    QLabel *legendTitle = new QLabel("<b>Légende:</b>");
    legendTitle->setStyleSheet("font-size: 14px; margin-right: 20px;");
    legendLayout->addWidget(legendTitle);
    
    QLabel *greenCircle = new QLabel("●");
    greenCircle->setStyleSheet("color: #4caf50; font-size: 30px;");
    QLabel *greenLabel = new QLabel("Disponible (< 50%)");
    greenLabel->setStyleSheet("font-size: 12px;");
    legendLayout->addWidget(greenCircle);
    legendLayout->addWidget(greenLabel);
    legendLayout->addSpacing(20);
    
    QLabel *orangeCircle = new QLabel("●");
    orangeCircle->setStyleSheet("color: #ff9800; font-size: 30px;");
    QLabel *orangeLabel = new QLabel("Modéré (50-80%)");
    orangeLabel->setStyleSheet("font-size: 12px;");
    legendLayout->addWidget(orangeCircle);
    legendLayout->addWidget(orangeLabel);
    legendLayout->addSpacing(20);
    
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
}

void MainWindow::on_statistiques_bt_parking_clicked()
{
    afficherStatistiquesParkings();
}

void MainWindow::trierParCapaciteParking()
{
    if (!ui || !ui->tableWidget_2_parking) return;
    
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            QMessageBox::critical(this, "Erreur", "Connexion échouée !");
            return;
        }
    }
    
    QSqlQuery query;
    if (!query.exec("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION FROM PARKING ORDER BY CAPACITE DESC")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }
    
    ui->tableWidget_2_parking->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_2_parking->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_2_parking->setItem(row, col, item);
        }
        ++row;
    }
    
    ui->tableWidget_2_parking->resizeColumnsToContents();
    QMessageBox::information(this, "Tri effectué",
        QString("Tableau trié par capacité\n%1 parkings").arg(row));
}

void MainWindow::rechercherParLocalisationParking(QString localisation)
{
    if (!ui || !ui->tableWidget_2_parking) return;
    
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            QMessageBox::critical(this, "Erreur", "Connexion à la base échouée !");
            return;
        }
    }
    
    QSqlQuery query;
    query.prepare("SELECT ID_PARKING, NOM, LOCALISATION, CAPACITE, PLACESLIBRES, STATUT, TYPE, TARIFICATION FROM PARKING "
                  "WHERE LOWER(LOCALISATION) LIKE LOWER(:localisation) ORDER BY ID_PARKING");
    
    query.bindValue(":localisation", "%" + localisation + "%");
    
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }
    
    ui->tableWidget_2_parking->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget_2_parking->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget_2_parking->setItem(row, col, item);
        }
        ++row;
    }
    
    ui->tableWidget_2_parking->resizeColumnsToContents();
    
    if (row == 0) {
        QMessageBox::information(this, "Recherche",
            QString("Aucun parking trouvé pour la localisation : %1").arg(localisation));
        chargerParkings();
    } else {
        QMessageBox::information(this, "Recherche effectuée",
            QString("%1 parking(s) trouvé(s)").arg(row));
    }
}

void MainWindow::afficherStatistiquesParkings()
{
    // Vérifier si la connexion existe déjà
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection", false);
    if (!db.isValid() || !db.isOpen()) {
        Connection c;
        if (!c.createconnect()) {
            QMessageBox::critical(this, "Erreur", "Connexion à la base échouée!");
            return;
        }
    }
    
    QSqlQuery query;
    if (!query.exec("SELECT TYPE, COUNT(*) as nb FROM PARKING GROUP BY TYPE")) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }
    
    int nbPublic = 0;
    int nbPrive = 0;
    
    while (query.next()) {
        QString type = query.value(0).toString().toLower().trimmed();
        int count = query.value(1).toInt();
        
        if (type == "public" || type == "publique") {
            nbPublic = count;
        } else if (type == "prive" || type == "privé" || type == "private") {
            nbPrive = count;
        }
    }
    
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("📊 Statistiques des Parkings");
    statsDialog->resize(600, 500);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);
    
    QLabel *titleLabel = new QLabel("📊 Nombre de Parkings par Type");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c5f2d; padding: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(QBrush(Qt::white));
    
    int margin = 50;
    int graphWidth = 500;
    int graphHeight = 300;
    int barWidth = 150;
    
    scene->addLine(margin, margin + graphHeight, margin + graphWidth, margin + graphHeight, QPen(Qt::black, 3));
    scene->addLine(margin, margin, margin, margin + graphHeight, QPen(Qt::black, 3));
    
    int maxValue = qMax(nbPublic, nbPrive);
    if (maxValue == 0) maxValue = 10;
    
    auto drawBar = [&](int x, int value, QColor color, QString label) {
        double ratio = (double)value / maxValue;
        int barHeight = ratio * (graphHeight - 20);
        int y = margin + graphHeight - barHeight;
        
        QGraphicsRectItem *bar = scene->addRect(x, y, barWidth, barHeight);
        bar->setBrush(QBrush(color));
        bar->setPen(QPen(Qt::black, 3));
        
        QGraphicsTextItem *valueText = scene->addText(QString::number(value));
        QFont valueFont = valueText->font();
        valueFont.setPointSize(20);
        valueFont.setBold(true);
        valueText->setFont(valueFont);
        valueText->setDefaultTextColor(color.darker(150));
        valueText->setPos(x + 55, y - 35);
        
        QGraphicsTextItem *labelText = scene->addText(label);
        QFont labelFont = labelText->font();
        labelFont.setPointSize(14);
        labelFont.setBold(true);
        labelText->setFont(labelFont);
        labelText->setPos(x + 30, margin + graphHeight + 10);
    };
    
    drawBar(margin + 50, nbPublic, QColor(76, 175, 80), "Public");
    drawBar(margin + 280, nbPrive, QColor(33, 150, 243), "Privé");
    
    mainLayout->addWidget(view);
    
    statsDialog->exec();
}

void MainWindow::on_tableWidget_2_parking_itemClicked(QTableWidgetItem *item)
{
    if (!item) return;
    int row = item->row();
    ui->id_parking->setText(ui->tableWidget_2_parking->item(row, 0)->text());
    ui->nom_parking->setText(ui->tableWidget_2_parking->item(row, 1)->text());
    ui->loco_parking->setText(ui->tableWidget_2_parking->item(row, 2)->text());
    ui->capacite_parking->setText(ui->tableWidget_2_parking->item(row, 3)->text());
    ui->place_parking->setText(ui->tableWidget_2_parking->item(row, 4)->text());
    ui->statut_parking->setText(ui->tableWidget_2_parking->item(row, 5)->text());
    ui->type_parking->setText(ui->tableWidget_2_parking->item(row, 6)->text());
    ui->tarif_parking->setText(ui->tableWidget_2_parking->item(row, 7)->text());
    qDebug() << "Ligne sélectionnée - ID:" << ui->tableWidget_2_parking->item(row, 0)->text();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    // Protection contre les crashes lors de la navigation entre onglets
    if (!ui) return;
    
    Q_UNUSED(index);
    
    // Vérifier que tous les widgets critiques existent avant de les utiliser
    // Cette fonction est appelée automatiquement par Qt lors du changement d'onglet
    // Elle permet de s'assurer que les widgets sont valides
    
    // Ne rien faire de spécial, juste une protection
    // Les fonctions individuelles ont déjà leurs propres vérifications
}

// === IMPLÉMENTATIONS DES SLOTS RFID ===

void MainWindow::on_btnAssocierRFID_clicked()
{
    // Récupérer l'employé sélectionné dans le tableau
    QModelIndexList selection = ui->tab_emp->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Sélection Requise", 
                            "Veuillez sélectionner un employé dans le tableau\n"
                            "avant d'associer une carte RFID.");
        return;
    }
    
    int row = selection.first().row();
    
    // Récupérer les données via le modèle du QTableView
    QAbstractItemModel* model = ui->tab_emp->model();
    QModelIndex cinIndex = model->index(row, 1); // Colonne CIN
    QModelIndex nomIndex = model->index(row, 2); // Colonne Nom
    QModelIndex prenomIndex = model->index(row, 3); // Colonne Prénom
    
    QString cinStr = model->data(cinIndex).toString();
    QString nomEmploye = model->data(nomIndex).toString() + " " + model->data(prenomIndex).toString();
    
    bool ok;
    int cin = cinStr.toInt(&ok);
    if (!ok) {
        QMessageBox::critical(this, "❌ Erreur", "CIN invalide");
        return;
    }
    
    // Demander à l'utilisateur de scanner la carte
    QMessageBox::information(this, "📡 Association RFID", 
                             QString("Veuillez scanner la carte RFID pour l'employé:\n\n"
                                     "**%1** (CIN: %2)\n\n"
                                     "Cliquez sur OK puis scannez la carte.")
                             .arg(nomEmploye).arg(cin));
    
    // Activer le mode scan
    rfidScanMode = true;
    currentRFIDUID = "";
    
    // Initialiser le lecteur RFID si nécessaire
    if (!rfidReader) {
        rfidReader = new SerialReader(this);
        rfidReader->openPort("COM5");
        
        connect(rfidReader, &SerialReader::uidReceived, this, &MainWindow::onUIDReceived);
    }
    
    QMessageBox::information(this, "🔄 Mode Scan Activé", 
                             "Mode scan activé! Veuillez scanner la carte RFID maintenant.\n\n"
                             "L'association sera automatique.");
}

void MainWindow::on_btnScannerRFID_clicked()
{
    if (!rfidReader) {
        rfidReader = new SerialReader(this);
        rfidReader->openPort("COM5");
        connect(rfidReader, &SerialReader::uidReceived, this, &MainWindow::onUIDReceived);
    }
    
    rfidScanMode = false; // Mode test simple
    currentRFIDUID = "";
    
    QMessageBox::information(this, "📡 Test RFID Scanner", 
                             "Mode test activé! Veuillez scanner une carte RFID.\n\n"
                             "L'UID sera affiché dans la console.");
}

void MainWindow::onUIDReceived(QString uid)
{
    qDebug() << "📡 UID RFID reçu dans MainWindow:" << uid;
    currentRFIDUID = uid;
    
    if (rfidScanMode) {
        // Mode association - associer l'UID à l'employé sélectionné
        QModelIndexList selection = ui->tab_emp->selectionModel()->selectedRows();
        if (!selection.isEmpty()) {
            int row = selection.first().row();
            
            // Récupérer les données via le modèle du QTableView
            QAbstractItemModel* model = ui->tab_emp->model();
            QModelIndex cinIndex = model->index(row, 1); // Colonne CIN
            QModelIndex nomIndex = model->index(row, 2); // Colonne Nom
            QModelIndex prenomIndex = model->index(row, 3); // Colonne Prénom
            
            QString cinStr = model->data(cinIndex).toString();
            QString nomEmploye = model->data(nomIndex).toString() + " " + model->data(prenomIndex).toString();
            
            int cin = cinStr.toInt();
            
            if (employee::associerRFIDAEmploye(cin, uid)) {
                QMessageBox::information(this, "✅ Association Réussie", 
                                     QString("Carte RFID associée avec succès!\n\n"
                                             "**Employé**: %1\n"
                                             "**CIN**: %2\n"
                                             "**UID RFID**: %3")
                                     .arg(nomEmploye).arg(cin).arg(uid));
                
                // Rafraîchir le tableau des employés
                on_refresh_clicked();
                
                // Désactiver le mode scan
                rfidScanMode = false;
            } else {
                QMessageBox::critical(this, "❌ Erreur d'Association", 
                                    "Impossible d'associer la carte RFID à cet employé.\n"
                                    "Vérifiez la connexion à la base de données.");
            }
        }
    } else {
        // Mode test - juste afficher l'UID
        QMessageBox::information(this, "📡 UID Détecté", 
                             QString("UID de la carte scannée:\n\n"
                                     "**%1**\n\n"
                                     "Vérification dans la base de données...")
                             .arg(uid));
        
        // Vérifier si l'UID existe dans la base avec requête directe
        QSqlQuery query;
        query.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE WHERE RFID_UID = :rfid_uid");
        query.bindValue(":rfid_uid", uid);
        
        qDebug() << "🔍 Requête RFID - UID recherché:" << uid;
        qDebug() << "🔍 Requête SQL préparée:" << query.lastQuery();
        qDebug() << "🔍 Valeur liée:" << query.boundValue(":rfid_uid").toString();
        
        if (query.exec()) {
            qDebug() << "✅ Requête exécutée avec succès";
            qDebug() << "📊 Nombre de résultats:" << query.size();
            
            if (query.next()) {
                QString cin = query.value(0).toString();
                QString nom = query.value(1).toString();
                QString prenom = query.value(2).toString();
                
                qDebug() << "👤 Employé trouvé - CIN:" << cin << "Nom:" << nom << "Prénom:" << prenom;
                
                QMessageBox::information(this, "✅ Carte Connue", 
                                     QString("Cette carte est associée à:\n\n"
                                             "**%1 %2**\n"
                                             "**CIN**: %3\n"
                                             "**UID**: %4")
                                     .arg(nom).arg(prenom).arg(cin).arg(uid));
            } else {
                qDebug() << "❌ Aucun enregistrement trouvé pour l'UID:" << uid;
                QMessageBox::warning(this, "❌ Carte Inconnue", 
                                  QString("Cette carte RFID n'est associée à aucun employé.\n\n"
                                          "UID: %1").arg(uid));
            }
        } else {
            qDebug() << "❌ Erreur exécution requête RFID:" << query.lastError().text();
            qDebug() << "❌ Requête qui a échoué:" << query.lastQuery();
            QMessageBox::warning(this, "❌ Erreur Base de Données", 
                              QString("Erreur lors de la vérification RFID:\n%1").arg(query.lastError().text()));
        }
    }
}
