#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

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
    // Slot pour le bouton Ajouter
    void on_ajouter_bt_clicked();

    // Slot pour le bouton Modifier
    void on_modifier_bt_clicked();

    // Slot pour le bouton Supprimer
    void on_supprimer_bt_clicked();

    // Slot pour la sélection d'une ligne dans le tableau
    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);

    // Slot pour le bouton Trier
    void on_tri_bt_clicked();

    // Slot pour le bouton Rechercher
    void on_ajouter_bt_2_clicked();

    // Slot pour le bouton PDF
    void on_pdf_bt_clicked();

private:
    Ui::MainWindow *ui;

    // Méthode pour charger les données du tableau
    void loadParkingTable();

    // Méthode pour trier les parkings par capacité
    void trierParCapacite();

    // Méthode pour rechercher par localisation
    void rechercherParLocalisation(QString localisation);
};

#endif // MAINWINDOW_H
