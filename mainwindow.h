#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_trie_clicked();
    void on_lineEdit_recherche_textChanged(const QString &text);

private:
    Ui::MainWindow *ui;

    void rafraichirAvecModele(QSqlQueryModel* model);
    void rafraichir();
    static QDate parseDateFromLineEdit(const QString& s);
};

#endif // MAINWINDOW_H
