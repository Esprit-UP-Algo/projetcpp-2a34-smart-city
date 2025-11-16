#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_bt_clicked();
    void on_modifier_bt_clicked();
    void on_supprimer_bt_clicked();
    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);
    void on_tri_bt_clicked();
    void on_ajouter_bt_2_clicked();  // Bouton rechercher

private:
    Ui::MainWindow *ui;
    void loadParkingTable();
    void trierParCapacite();
    void rechercherParLocalisation(QString localisation);
};
#endif // MAINWINDOW_H
