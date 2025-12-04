#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QDate>
#include <QFileDialog>
#include <QTableWidgetItem>
#include "employee.h"
#include "equipement.h"
#include "dechet.h"
#include "alerte.h"
#include "parking.h"
#include "notificationmanager.h"
#include "connection.h"
#include "serialreader.h"

// Forward declaration pour AlerteMap (si Qt Location est disponible)
#ifdef LOCATION_AVAILABLE
class AlerteMap;
#endif

QT_BEGIN_NAMESPACE
class QChart;
class QChartView;
class QBarSeries;
class QPieSeries;
class QLineSeries;
class QLineEdit;
class QTextEdit;
class QComboBox;
class QDateEdit;
class QLabel;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    // === SLOTS POUR LES EMPLOYÉS ===
    void on_ajouter_clicked();
    void on_modifier_clicked();
    void on_supprimer_clicked();
    void on_refresh_clicked();
    void on_refresh_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_refresh_3_clicked();
    void on_tab_emp_clicked(const QModelIndex &index);
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_comboBox_3_currentIndexChanged(int index);
    void on_btnCaptureFace_clicked();
    
    // === SLOTS POUR LA GESTION RFID ===
    void on_btnAssocierRFID_clicked();
    void on_btnScannerRFID_clicked();
    void onUIDReceived(QString uid);

    // === SLOTS POUR LES ÉQUIPEMENTS ===
    void on_pushButton_13_clicked();   // Ajouter équipement
    void on_pushButton_15_clicked();   // Modifier équipement
    void on_pushButton_4_clicked();    // Supprimer équipement
    void on_pushButton_12_clicked();   // Afficher tous les équipements
    void on_pushButton_14_clicked();   // Vider champs ajout équipement
    void on_pushButton_16_clicked();   // Vider champs modification équipement
    void on_comboBox_currentIndexChanged(int index);  // Tri équipements
    void on_tableView_clicked(const QModelIndex &index);  // Sélection équipement
    
    // Nouveaux slots pour les fonctionnalités de filtrage (comme Atelier_Connexion)
    void on_pushButton_statistiques_clicked();
    void on_pushButton_notifications_clicked();
    void on_pushButton_test_notifications_clicked();
    void on_pushButton_export_pdf_clicked();
    void on_pushButtonResetFiltres_clicked();

    // === SLOTS POUR LES DÉCHETS ===
    void on_ajouter_dechet_btn_clicked();
    void on_modifier_dechet_btn_clicked();
    void on_supprimer_dechet_btn_clicked();
    void on_afficher_dechet_btn_clicked();
    void on_rech_dechet_btn_clicked();
    void on_trier_dechet_btn_clicked();
    void on_refresh_dechet_btn_clicked();
    void on_pdf_dechet_btn_clicked();
    void on_tableView_dechet_clicked(const QModelIndex &index);  // Charger les données dans le formulaire de modification

    // === NOUVEAUX SLOTS POUR LES STATISTIQUES DÉCHETS ===
    void on_statistiques_dechet_btn_clicked();
    void on_afficher_stats_completes_btn_clicked();
    void on_afficher_stats_tableau_btn_clicked();
    
    // === ADVANCED WASTE MANAGEMENT FEATURES ===
    void on_filtrer_date_range_btn_clicked();
    void on_filtrer_volume_range_btn_clicked();
    void on_filtrer_multi_critere_btn_clicked();
    void on_export_excel_btn_clicked();
    void on_export_csv_btn_clicked();
    void on_afficher_charts_btn_clicked();
    void on_calculer_impact_env_btn_clicked();
    void on_afficher_alertes_btn_clicked();
    void on_analyser_tendances_btn_clicked();
    void on_afficher_map_btn_clicked();
    
    // === SLOTS POUR LES ALERTES (dans l'onglet) ===
    void on_btn_ajouter_alerte_clicked();
    void on_btn_modifier_alerte_clicked();
    void on_btn_supprimer_alerte_clicked();
    void on_btn_vider_alerte_clicked();
    void on_btn_refresh_alerte_clicked();
    void on_btn_rechercher_alerte_clicked();
    void on_btn_filtrer_gravite_alerte_clicked();
    void on_btn_trier_alerte_clicked();
    void on_btn_pdf_alerte_clicked();
    void on_btn_carte_alerte_clicked();
    void on_tableView_alertes_selectionChanged();
    void on_tableView_alertes_clicked(const QModelIndex &index);
    
    // === SLOTS POUR LES PARKINGS ===
    void on_ajouter_bt_parking_clicked();
    void on_modifier_bt_2_parking_clicked();
    void on_supprimer_bt_parking_clicked();
    void on_pushButton_12_parking_clicked();
    void on_tri_bt_parking_clicked();
    void on_ajouter_bt_2_parking_clicked();
    void on_pdf_bt_parking_clicked();
    void on_map_bt_parking_clicked();
    void on_statistiques_bt_parking_clicked();
    void on_tableWidget_2_parking_itemClicked(QTableWidgetItem *item);
    
    // === SLOTS POUR LES EMPLOYÉS ===
    void on_btnGraphiquesEmployes_clicked(); // Nouveau slot pour graphiques employés

private:
    Ui::MainWindow *ui;
    employee gestionEmployes;
    Dechet tmpDechet;
    Alerte alerteTmp;
    Parking gestionParking;

    // Méthodes pour employés
    bool validerEmploye(const QString& cin, const QString& nom, const QString& prenom,
                        const QString& email, const QString& telephone, const QString& salaire);
    void viderChampsEmploye();
    void chargerEmployes();
    void viderChampsModificationEmploye();

    // Méthodes pour équipements
    bool validerEquipement(const QString& id, const QString& nom, const QString& type,
                           const QString& zone, const QString& etat, const QString& consommation);
    void viderChampsEquipement();
    void chargerEquipements();
    void viderChampsModificationEquipement();
    
    // Fonctionnalités de filtrage et recherche (comme Atelier_Connexion)
    void setupFiltresEquipements();
    void remplirFiltresType();
    void remplirFiltresZone();
    void appliquerFiltresEquipements();
    void rechercherParIDEquipement(const QString &idText);
    void onFiltreEquipementChanged();
    void onRechercheIDEquipementChanged(const QString &text);
    void onResetFiltresEquipements();
    
    // Méthodes pour déchets
    void chargerDechets();
    
    // Méthodes pour alertes
    void gererAlertes();
    void chargerAlertes();
    void viderChampsAlerte();
    void viderChampsAlerteForm(QLineEdit *id, QLineEdit *type, QLineEdit *idClient, 
                                QLineEdit *statut, QTextEdit *description, 
                                QComboBox *gravite, QDateEdit *date);
    bool validerAlerte(const QString& type, const QString& idClient, const QString& statut, const QString& gravite);
    void updateStatsAlerte(QLabel *label);
    void genererPDFAlertes(QWidget *parent);
    void remplirFormulaireAlerte();
    void afficherStatistiquesGraphiquesAlertes(); // Nouvelle fonction pour popup graphique
    
    // Méthodes pour parkings
    void chargerParkings();
    void viderChampsParking();
    bool validerParking(const QString& nom, const QString& localisation, int capacite, double tarif);
    void afficherCarteParkings();
    void afficherStatistiquesParkings();
    void trierParCapaciteParking();
    void rechercherParLocalisationParking(QString localisation);
    
    // Protection contre les crashes lors de la navigation
    void on_tabWidget_currentChanged(int index);

    // Méthodes pour statistiques déchets
    void afficherStatistiquesDansTableau();
    void genererRapportStatistiques();
    void afficherStatistiquesGraphiques();

    // Export PDF Statistics
    void on_export_pdf_stats_btn_clicked();
    
    // Advanced waste management methods
    void afficherCharts();
    void afficherImpactEnvironnemental();
    void afficherAlertes();
    void afficherTendances();
    void afficherEmployeeCharts(); // Nouvelle fonction pour graphiques employés
    #ifdef LOCATION_AVAILABLE
    void refreshAlerteMap();
    #endif
    
    // Notification system methods
    void on_notifications_settings_btn_clicked();
    void on_notification_history_btn_clicked();
    void on_manage_alert_rules_btn_clicked();
    void configureNotifications();
    void showNotificationHistory();
    void manageAlertRules();
    void setupDefaultAlertRules();
    
private slots:
    // Notification slots removed - using lambda connections in constructor instead
    
private:
    NotificationManager *notificationManager;
    #ifdef LOCATION_AVAILABLE
    AlerteMap *m_alerteMap;
    #endif
    
    // === ATTRIBUTS POUR LA GESTION RFID ===
    SerialReader *rfidReader;
    QString currentRFIDUID;
    bool rfidScanMode;
};

#endif // MAINWINDOW_H
