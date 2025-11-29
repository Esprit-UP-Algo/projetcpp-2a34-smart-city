#include "passwordrecovery.h"
#include "employee.h"
#include "connection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

PasswordRecovery::PasswordRecovery(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Récupération de Mot de Passe");
    setFixedSize(450, 550);

    emailUtilisateur = "";
    codeVerification = "";
    motDePasseTemporaire = "";

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel("Récupération de Mot de Passe");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #4CAF50; margin-bottom: 20px;");
    layout->addWidget(titleLabel);

    QLabel *emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet("font-weight: bold; color: #333;");
    QLineEdit *emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Entrez votre email professionnel");
    emailEdit->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 4px;");

    QPushButton *verifyEmailBtn = new QPushButton("Vérifier l'Email");
    verifyEmailBtn->setStyleSheet(getButtonStyle());

    QLabel *emailStatus = new QLabel();
    emailStatus->setStyleSheet("color: #d9534f; font-weight: bold; min-height: 20px;");

    QLabel *codeLabel = new QLabel("Code de vérification:");
    codeLabel->setStyleSheet("font-weight: bold; color: #333;");
    QLineEdit *codeEdit = new QLineEdit();
    codeEdit->setPlaceholderText("Code reçu par email/SMS");
    codeEdit->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 4px;");

    QHBoxLayout *codeLayout = new QHBoxLayout();
    QPushButton *sendCodeBtn = new QPushButton("📧 Obtenir le Code");
    sendCodeBtn->setStyleSheet(getButtonStyle());

    QPushButton *resendCodeBtn = new QPushButton("🔄 Renvoyer");
    resendCodeBtn->setStyleSheet(getSecondaryButtonStyle());
    resendCodeBtn->setVisible(false);

    codeLayout->addWidget(sendCodeBtn);
    codeLayout->addWidget(resendCodeBtn);

    QLabel *newPassLabel = new QLabel("Nouveau mot de passe:");
    newPassLabel->setStyleSheet("font-weight: bold; color: #333;");
    QLineEdit *newPassEdit = new QLineEdit();
    newPassEdit->setPlaceholderText("Minimum 6 caractères");
    newPassEdit->setEchoMode(QLineEdit::Password);
    newPassEdit->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 4px;");

    QLabel *confirmPassLabel = new QLabel("Confirmer le mot de passe:");
    confirmPassLabel->setStyleSheet("font-weight: bold; color: #333;");
    QLineEdit *confirmPassEdit = new QLineEdit();
    confirmPassEdit->setPlaceholderText("Retapez le même mot de passe");
    confirmPassEdit->setEchoMode(QLineEdit::Password);
    confirmPassEdit->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 4px;");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *resetBtn = new QPushButton("🔄 Réinitialiser");
    resetBtn->setStyleSheet(getButtonStyle());
    resetBtn->setMinimumHeight(40);

    QPushButton *cancelBtn = new QPushButton("❌ Annuler");
    cancelBtn->setStyleSheet(getCancelButtonStyle());
    cancelBtn->setMinimumHeight(40);

    buttonLayout->addWidget(resetBtn);
    buttonLayout->addWidget(cancelBtn);

    layout->addWidget(emailLabel);
    layout->addWidget(emailEdit);
    layout->addWidget(verifyEmailBtn);
    layout->addWidget(emailStatus);
    layout->addSpacing(10);
    layout->addWidget(codeLabel);
    layout->addWidget(codeEdit);
    layout->addLayout(codeLayout);
    layout->addSpacing(10);
    layout->addWidget(newPassLabel);
    layout->addWidget(newPassEdit);
    layout->addWidget(confirmPassLabel);
    layout->addWidget(confirmPassEdit);
    layout->addSpacing(20);
    layout->addLayout(buttonLayout);

    connect(verifyEmailBtn, &QPushButton::clicked, [=]() {
        QString email = emailEdit->text().trimmed();

        if (email.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer votre adresse email!");
            return;
        }

        QRegularExpression regexEmail("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (!regexEmail.match(email).hasMatch()) {
            QMessageBox::warning(this, "Erreur", "Format d'email invalide!");
            return;
        }

        if (employee::verifierEmailExiste(email)) {
            emailUtilisateur = email;
            emailStatus->setText("✓ Email vérifié: " + email);
            emailStatus->setStyleSheet("color: #5cb85c; font-weight: bold;");

            QMessageBox::information(this, "Succès",
                                     "Email trouvé! Cliquez sur 'Obtenir le Code' pour recevoir votre code de vérification.");

            resendCodeBtn->setVisible(true);
        } else {
            emailStatus->setText("✗ Aucun compte trouvé avec cet email");
            emailStatus->setStyleSheet("color: #d9534f; font-weight: bold;");
            QMessageBox::warning(this, "Erreur",
                                 "Aucun compte trouvé avec cet email.\nVeuillez vérifier l'adresse email.");
        }
    });

    connect(sendCodeBtn, &QPushButton::clicked, [=]() {
        envoyerCodeVerification();
    });

    connect(resendCodeBtn, &QPushButton::clicked, [=]() {
        envoyerCodeVerification();
    });

    connect(resetBtn, &QPushButton::clicked, [=]() {
        QString codeSaisi = codeEdit->text().trimmed();
        QString nouveauMotDePasse = newPassEdit->text();
        QString confirmerMotDePasse = confirmPassEdit->text();

        if (emailUtilisateur.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez d'abord vérifier votre email.");
            return;
        }

        if (codeSaisi != codeVerification) {
            QMessageBox::warning(this, "Erreur", "Code de vérification incorrect!");
            return;
        }

        if (nouveauMotDePasse != confirmerMotDePasse) {
            QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas!");
            return;
        }

        if (nouveauMotDePasse.length() < 6) {
            QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 6 caractères!");
            return;
        }

        if (employee::changerMotDePasse(emailUtilisateur, nouveauMotDePasse)) {
            QMessageBox::information(this, "✅ Succès",
                                     "Votre mot de passe a été réinitialisé avec succès!\n\n"
                                     "Vous pouvez maintenant vous connecter avec votre nouveau mot de passe.");
            accept();
        } else {
            QMessageBox::critical(this, "❌ Erreur",
                                  "Erreur lors de la réinitialisation du mot de passe.");
        }
    });

    connect(cancelBtn, &QPushButton::clicked, this, &PasswordRecovery::reject);
}

void PasswordRecovery::envoyerCodeVerification()
{
    if (emailUtilisateur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez d'abord vérifier votre email.");
        return;
    }

    codeVerification = Connection::genererCodeVerification();
    motDePasseTemporaire = Connection::genererMotDePasseTemporaire();

    qDebug() << "Code de vérification généré:" << codeVerification;
    qDebug() << "Mot de passe temporaire:" << motDePasseTemporaire;

    QString sujet = "🔐 Code de Vérification - Système de Gestion Malek";
    QString messageEmail = QString(
                               "Bonjour,\n\n"
                               "Vous avez demandé une réinitialisation de mot de passe pour votre compte.\n\n"
                               "🔢 **VOTRE CODE DE VÉRIFICATION** : %1\n\n"
                               "🔑 **Mot de passe temporaire** : %2\n\n"
                               "📋 **Instructions :**\n"
                               "1. Retournez sur la page de récupération\n"
                               "2. Entrez le code : %1\n"
                               "3. Choisissez votre nouveau mot de passe\n"
                               "4. Confirmez le nouveau mot de passe\n\n"
                               "⏰ **Ce code est valable 10 minutes**\n\n"
                               "Si vous n'avez pas demandé cette réinitialisation, veuillez ignorer ce message.\n\n"
                               "Cordialement,\n"
                               "L'équipe de Gestion Intégrée - Malek"
                               ).arg(codeVerification).arg(motDePasseTemporaire);

    QMessageBox::information(this, "⏳ Envoi en cours",
                             "Envoi du code de vérification par email...\n\n"
                             "Veuillez patienter, cela peut prendre quelques secondes.");

    bool succes = Connection::envoyerEmail(emailUtilisateur, sujet, messageEmail);

    if (succes) {
        qDebug() << "✅ Email envoyé avec succès";
        QMessageBox::information(this, "✅ Succès",
                                 "Code de vérification envoyé avec succès!\n\n"
                                 "Vérifiez votre boîte email : " + emailUtilisateur);
    } else {
        qDebug() << "❌ Échec envoi email, affichage manuel";
        QMessageBox::information(this, "🔐 Code de Vérification",
                                 QString("L'envoi d'email a échoué.\n\n"
                                         "🔢 **VOTRE CODE DE VÉRIFICATION** : %1\n\n"
                                         "🔑 **Mot de passe temporaire** : %2\n\n"
                                         "Utilisez ces informations pour continuer la récupération.")
                                     .arg(codeVerification)
                                     .arg(motDePasseTemporaire));
    }
}

QString PasswordRecovery::getButtonStyle()
{
    return "QPushButton {"
           "    background-color: #5cb85c;"
           "    color: white;"
           "    border: none;"
           "    padding: 10px 15px;"
           "    border-radius: 4px;"
           "    font-weight: bold;"
           "    min-width: 120px;"
           "}"
           "QPushButton:hover {"
           "    background-color: #4cae4c;"
           "}"
           "QPushButton:pressed {"
           "    background-color: #449d44;"
           "}";
}

QString PasswordRecovery::getSecondaryButtonStyle()
{
    return "QPushButton {"
           "    background-color: #f0ad4e;"
           "    color: white;"
           "    border: none;"
           "    padding: 8px 12px;"
           "    border-radius: 4px;"
           "    font-weight: bold;"
           "}"
           "QPushButton:hover {"
           "    background-color: #eea236;"
           "}";
}

QString PasswordRecovery::getCancelButtonStyle()
{
    return "QPushButton {"
           "    background-color: #d9534f;"
           "    color: white;"
           "    border: none;"
           "    padding: 10px 15px;"
           "    border-radius: 4px;"
           "    font-weight: bold;"
           "    min-width: 120px;"
           "}"
           "QPushButton:hover {"
           "    background-color: #c9302c;"
           "}";
}

void PasswordRecovery::verifierEmail() {}
void PasswordRecovery::envoyerCode() {}
void PasswordRecovery::reinitialiserPassword() {}
void PasswordRecovery::annuler() {}
