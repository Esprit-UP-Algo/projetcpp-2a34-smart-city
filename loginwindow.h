#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class LoginWindow;
}

class DatabaseManager;

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QString getEmailConnecte() const { return emailConnecte; }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

public slots:
    void tentativeConnexion();
    void motDePasseOublie();
    void basculerVisibiliteMotDePasse();
    void connexionParReconnaissanceFaciale();

private:
    Ui::LoginWindow *ui;
    DatabaseManager *dbManager;
    QString emailConnecte;

    bool validerConnexion(const QString& email, const QString& motDePasse);
    QString getEmailFromEmployeeId(int employeeId);
    QString getEmailFromCin(const QString& cin);
};

#endif // LOGINWINDOW_H
