#ifndef PASSWORDRECOVERY_H
#define PASSWORDRECOVERY_H

#include <QDialog>

class PasswordRecovery : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordRecovery(QWidget *parent = nullptr);

public slots:
    void verifierEmail();
    void envoyerCode();
    void reinitialiserPassword();
    void annuler();

private:
    QString emailUtilisateur;
    QString codeVerification;
    QString motDePasseTemporaire;

    void envoyerCodeVerification();
    QString getButtonStyle();
    QString getSecondaryButtonStyle();
    QString getCancelButtonStyle();
};

#endif // PASSWORDRECOVERY_H
