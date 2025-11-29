#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QString getEmailConnecte() const { return emailConnecte; }

public slots:
    void tentativeConnexion();
    void motDePasseOublie();
    void afficherInformations();
    void basculerVisibiliteMotDePasse();

private:
    QLineEdit *emailEdit;
    QLineEdit *motDePasseEdit;
    QPushButton *connexionBtn;
    QPushButton *motDePasseOublieBtn;
    QPushButton *infoBtn;
    QPushButton *afficherMotDePasseBtn;
    QLabel *statusLabel;

    QString emailConnecte;

    bool validerConnexion(const QString& email, const QString& motDePasse);
    void configurerInterface();
    QString getEmailFromEmployeeId(int employeeId);
};

#endif // LOGINWINDOW_H
