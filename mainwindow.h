#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employee.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_clicked();
    void on_refresh_clicked();
    void on_refresh_2_clicked();  // Recherche par CIN
    void on_refresh_3_clicked();  // Afficher statistiques dans ListView
    void on_tab_emp_clicked(const QModelIndex &index);
    void on_modifier_clicked();
    void on_supprimer_clicked();
    void on_pushButton_3_clicked();  // Tri par nom
    void on_pushButton_5_clicked();  // Export PDF
    void on_pushButton_6_clicked();  // Annuler (Modifier)
    void on_pushButton_7_clicked();  // Annuler (Ajouter)

private:
    Ui::MainWindow *ui;
    employee E;

    // Fonctions de validation
    bool validerNom(const QString &nom);
    bool validerEmail(const QString &email);
    bool validerTelephone(const QString &telephone);
    bool validerSalaire(const QString &salaire);
    bool validerCIN(const QString &cin);

    void genererPDF(const QString &fileName);
    void afficherStatistiques();
    void genererPDFStatistiques();
};

#endif // MAINWINDOW_H
