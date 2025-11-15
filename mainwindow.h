#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "employee.h"
#include "equipement.h"
#include "dechet.h"

QT_BEGIN_NAMESPACE
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

    // === SLOTS POUR LES ÉQUIPEMENTS ===
    void on_pushButton_13_clicked();   // Ajouter équipement
    void on_pushButton_15_clicked();   // Modifier équipement
    void on_pushButton_4_clicked();    // Supprimer équipement
    void on_pushButton_12_clicked();   // Rafraîchir liste équipements
    void on_pushButton_14_clicked();   // Vider champs ajout équipement
    void on_pushButton_16_clicked();   // Vider champs modification équipement
    void on_comboBox_currentIndexChanged(int index);  // Tri équipements
    void on_tableView_clicked(const QModelIndex &index);  // Sélection équipement

private:
    Ui::MainWindow *ui;
    employee gestionEmployes;
    Dechet tmpDechet;

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
    //dechets
public slots:
    void on_ajouter_dechet_btn_clicked();
    void on_modifier_dechet_btn_clicked();
    void on_supprimer_dechet_btn_clicked();
    void on_afficher_dechet_btn_clicked();
    void on_rech_dechet_btn_clicked();
    void on_trier_dechet_btn_clicked();
    void on_refresh_dechet_btn_clicked();
    void on_pdf_dechet_btn_clicked();

};

#endif // MAINWINDOW_H
