#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "alerte.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots pour les boutons CRUD
    void on_pushButton_3_clicked();      // Ajouter
    void on_pushButton_5_clicked();      // Modifier
    void on_pushButton_4_clicked();      // Annuler
    void on_pushButton_supprimer_clicked(); // Supprimer (si tu as ce bouton)

    // Slots pour afficher et rafraîchir
    void afficherAlertes();

    // Slots pour les métiers
    void on_pushButton_rechercher_clicked();  // Recherche (si tu as ce bouton)
    void on_pushButton_trier_clicked();       // Tri
    void on_pushButton_pdf_clicked();         // PDF

private:
    Ui::MainWindow *ui;
    Alerte alerteTmp;  // Objet temporaire pour manipuler les alertes

    // Méthodes utilitaires
    void clearFields();  // Vider les champs
    bool validateFields();  // Valider les champs
};

#endif // MAINWINDOW_H
