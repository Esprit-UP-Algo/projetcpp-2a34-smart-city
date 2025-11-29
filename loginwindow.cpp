#include "loginwindow.h"
#include "passwordrecovery.h"
#include "employee.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QDebug>
#include <QCoreApplication>
#include <QPixmap>
#include <QLabel>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Connexion - Système de Gestion");
    setFixedSize(500, 700);

    setStyleSheet(
        "QDialog {"
        "    background-color: #2b2b2b;"
        "}"
        );

    configurerInterface();

    connect(connexionBtn, &QPushButton::clicked, this, &LoginWindow::tentativeConnexion);
    connect(ajoutParDefautBtn, &QPushButton::clicked, this, &LoginWindow::ajouterUtilisateurParDefaut);
    connect(motDePasseOublieBtn, &QPushButton::clicked, this, &LoginWindow::motDePasseOublie);
    connect(infoBtn, &QPushButton::clicked, this, &LoginWindow::afficherInformations);
    connect(afficherMotDePasseBtn, &QPushButton::clicked, this, &LoginWindow::basculerVisibiliteMotDePasse);
    connect(emailEdit, &QLineEdit::returnPressed, this, &LoginWindow::tentativeConnexion);
    connect(motDePasseEdit, &QLineEdit::returnPressed, this, &LoginWindow::tentativeConnexion);
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::configurerInterface()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Logo optionnel - peut être ajouté plus tard
    QPixmap logoPixmap; // Logo désactivé pour l'instant
    // QPixmap logoPixmap(":/resources/logo.png");  // Décommentez si vous ajoutez un logo

    qDebug() << "Chargement du logo:" << (!logoPixmap.isNull() ? "Succès" : "Échec (optionnel)");
    if (!logoPixmap.isNull()) {
        QLabel *logoLabel = new QLabel();
        logoPixmap = logoPixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        logoLabel->setPixmap(logoPixmap);
        logoLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(logoLabel);
        qDebug() << "Logo ajouté à l'interface";
    }

    QLabel *titleLabel = new QLabel("Connexion au Système");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 20pt;"
        "font-weight: bold;"
        "color: #5cb85c;"
        "margin-bottom: 20px;"
        );
    mainLayout->addWidget(titleLabel);

    QLabel *emailLabel = new QLabel("Email:");
    emailLabel->setStyleSheet(
        "color: white;"
        "font-weight: normal;"
        "font-size: 11pt;"
        );
    mainLayout->addWidget(emailLabel);

    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Entrez votre email");
    emailEdit->setStyleSheet(
        "QLineEdit {"
        "    padding: 10px;"
        "    border: 1px solid #444;"
        "    border-radius: 3px;"
        "    font-size: 11pt;"
        "    background-color: #3a3a3a;"
        "    color: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #5cb85c;"
        "}"
        );
    mainLayout->addWidget(emailEdit);

    QLabel *passLabel = new QLabel("Mot de passe:");
    passLabel->setStyleSheet(
        "color: white;"
        "font-weight: normal;"
        "font-size: 11pt;"
        "margin-top: 10px;"
        );
    mainLayout->addWidget(passLabel);

    QHBoxLayout *passLayout = new QHBoxLayout();
    motDePasseEdit = new QLineEdit();
    motDePasseEdit->setPlaceholderText("Entrez votre mot de passe");
    motDePasseEdit->setEchoMode(QLineEdit::Password);
    motDePasseEdit->setStyleSheet(
        "QLineEdit {"
        "    padding: 10px;"
        "    border: 1px solid #444;"
        "    border-radius: 3px;"
        "    font-size: 11pt;"
        "    background-color: #3a3a3a;"
        "    color: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 1px solid #5cb85c;"
        "}"
        );
    passLayout->addWidget(motDePasseEdit);

    afficherMotDePasseBtn = new QPushButton("👁");
    afficherMotDePasseBtn->setFixedSize(45, 45);
    afficherMotDePasseBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #4a4a4a;"
        "    border: 1px solid #555;"
        "    border-radius: 3px;"
        "    font-size: 14pt;"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: #555;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3a3a3a;"
        "}"
        );
    passLayout->addWidget(afficherMotDePasseBtn);

    mainLayout->addLayout(passLayout);

    statusLabel = new QLabel();
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "color: #d9534f;"
        "font-weight: bold;"
        "min-height: 20px;"
        );
    mainLayout->addWidget(statusLabel);

    connexionBtn = new QPushButton("Se Connecter");
    connexionBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #5cb85c;"
        "    color: white;"
        "    border: none;"
        "    padding: 12px;"
        "    border-radius: 3px;"
        "    font-weight: bold;"
        "    font-size: 12pt;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4cae4c;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #449d44;"
        "}"
        );
    mainLayout->addWidget(connexionBtn);

    ajoutParDefautBtn = new QPushButton("Ajout par défaut");
    ajoutParDefautBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #f0ad4e;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px;"
        "    border-radius: 3px;"
        "    font-weight: bold;"
        "    font-size: 11pt;"
        "    margin-top: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #eea236;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #d58512;"
        "}"
        );
    mainLayout->addWidget(ajoutParDefautBtn);

    motDePasseOublieBtn = new QPushButton("Mot de passe oublié ?");
    motDePasseOublieBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #5cb85c;"
        "    border: none;"
        "    padding: 8px;"
        "    text-decoration: underline;"
        "    font-size: 10pt;"
        "}"
        "QPushButton:hover {"
        "    color: #4cae4c;"
        "}"
        );
    mainLayout->addWidget(motDePasseOublieBtn);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #444;");
    mainLayout->addWidget(line);

    infoBtn = new QPushButton("ℹ Informations");
    infoBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #5bc0de;"
        "    color: white;"
        "    border: none;"
        "    padding: 8px;"
        "    border-radius: 3px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #46b8da;"
        "}"
        );
    mainLayout->addWidget(infoBtn);

    mainLayout->addStretch();
}

void LoginWindow::tentativeConnexion()
{
    qDebug() << "=== TENTATIVE DE CONNEXION ===";

    QString email = emailEdit->text().trimmed();
    QString motDePasse = motDePasseEdit->text();

    statusLabel->clear();

    if (email.isEmpty() || motDePasse.isEmpty()) {
        statusLabel->setText("⚠ Veuillez remplir tous les champs");
        statusLabel->setStyleSheet("color: #f0ad4e; font-weight: bold;");
        QMessageBox::warning(this, "Champs Manquants",
                             "Veuillez entrer votre email et votre mot de passe!");
        return;
    }

    QRegularExpression regexEmail("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regexEmail.match(email).hasMatch()) {
        statusLabel->setText("⚠ Format d'email invalide");
        statusLabel->setStyleSheet("color: #f0ad4e; font-weight: bold;");
        QMessageBox::warning(this, "Email Invalide", "Le format de l'email est incorrect!");
        return;
    }

    if (validerConnexion(email, motDePasse)) {
        emailConnecte = email;
        statusLabel->setText("✓ Identifiants validés!");
        statusLabel->setStyleSheet("color: #5cb85c; font-weight: bold;");

        qDebug() << "Connexion réussie pour:" << email;

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Connexion Réussie");
        msgBox.setText("Bienvenue dans le système de gestion!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox {"
            "    background-color: #2b2b2b;"
            "}"
            "QLabel {"
            "    color: white;"
            "}"
            );
        msgBox.exec();

        accept();
    } else {
        statusLabel->setText("✗ Email ou mot de passe incorrect");
        statusLabel->setStyleSheet("color: #d9534f; font-weight: bold;");

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Erreur de Connexion");
        msgBox.setText("Email ou mot de passe incorrect!\n\n"
                       "Si vous avez oublié votre mot de passe, "
                       "cliquez sur 'Mot de passe oublié ?'");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet(
            "QMessageBox {"
            "    background-color: #2b2b2b;"
            "}"
            "QLabel {"
            "    color: white;"
            "}"
            );
        msgBox.exec();

        motDePasseEdit->clear();
        motDePasseEdit->setFocus();
    }
}

bool LoginWindow::validerConnexion(const QString& email, const QString& motDePasse)
{
    qDebug() << "Validation de la connexion pour:" << email;

    QSqlQuery query;
    // Utiliser les noms de colonnes réels de la table : MOT_DE_PASSE et NOM_EMP
    query.prepare("SELECT CIN, NOM_EMP, PRENOM, MOT_DE_PASSE FROM EMPLOYE WHERE EMAIL = :email");
    query.bindValue(":email", email);

    if (query.exec()) {
        if (query.next()) {
            QString motDePasseDB = query.value(3).toString();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();

            qDebug() << "Utilisateur trouvé:" << nom << prenom;
            qDebug() << "Mot de passe DB:" << motDePasseDB;
            qDebug() << "Mot de passe saisi:" << motDePasse;
            qDebug() << "Comparaison des mots de passe...";

            if (motDePasseDB == motDePasse) {
                qDebug() << "✅ Mot de passe correct";
                return true;
            } else {
                qDebug() << "❌ Mot de passe incorrect";
                qDebug() << "   DB length:" << motDePasseDB.length();
                qDebug() << "   Saisi length:" << motDePasse.length();
                return false;
            }
        } else {
            qDebug() << "❌ Aucun utilisateur trouvé avec cet email";
            return false;
        }
    } else {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return false;
    }
}

void LoginWindow::motDePasseOublie()
{
    qDebug() << "=== OUVERTURE RÉCUPÉRATION MOT DE PASSE ===";

    PasswordRecovery *dialog = new PasswordRecovery(this);

    if (dialog->exec() == QDialog::Accepted) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Succès");
        msgBox.setText("Mot de passe réinitialisé avec succès!\n"
                       "Vous pouvez maintenant vous connecter avec votre nouveau mot de passe.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox {"
            "    background-color: #2b2b2b;"
            "}"
            "QLabel {"
            "    color: white;"
            "}"
            );
        msgBox.exec();
    }

    delete dialog;
}

void LoginWindow::afficherInformations()
{
    QString info = "🏢 SYSTÈME DE GESTION INTÉGRÉ\n\n"
                   "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
                   "📋 FONCTIONNALITÉS:\n"
                   "  • Gestion des employés (CRUD)\n"
                   "  • Gestion des équipements\n"
                   "  • Statistiques et rapports\n"
                   "  • Tri et recherche avancés\n"
                   "  • Récupération de mot de passe\n\n"
                   "🔒 SÉCURITÉ:\n"
                   "  • Authentification requise\n"
                   "  • Validation des données\n"
                   "  • Code de vérification SMS\n\n"
                   "💡 AIDE:\n"
                   "  • Utilisez votre email professionnel\n"
                   "  • En cas d'oubli: 'Mot de passe oublié ?'\n"
                   "  • Le code sera envoyé par SMS\n\n"
                   "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                   "Version 2.0 | Support: contact@systeme.tn";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Informations Système");
    msgBox.setText(info);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(
        "QMessageBox {"
        "    background-color: #2b2b2b;"
        "}"
        "QLabel {"
        "    color: white;"
        "    font-family: 'Consolas', monospace;"
        "}"
        );
    msgBox.exec();
}

void LoginWindow::basculerVisibiliteMotDePasse()
{
    if (motDePasseEdit->echoMode() == QLineEdit::Password) {
        motDePasseEdit->setEchoMode(QLineEdit::Normal);
        afficherMotDePasseBtn->setText("🔒");
    } else {
        motDePasseEdit->setEchoMode(QLineEdit::Password);
        afficherMotDePasseBtn->setText("👁");
    }
}

QString LoginWindow::getEmailFromEmployeeId(int employeeId)
{
    QSqlQuery query;
    query.prepare("SELECT EMAIL FROM EMPLOYE WHERE CIN = :id OR id_emp = :id");
    query.bindValue(":id", employeeId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

void LoginWindow::ajouterUtilisateurParDefaut()
{
    qDebug() << "=== AJOUT UTILISATEUR PAR DÉFAUT ===";

    // Données par défaut
    QString emailDefaut = "admin@test.tn";  // 15 caractères - bien en dessous de la limite de 20
    QString motDePasseDefaut = "admin123";
    QString nomDefaut = "Admin";
    QString prenomDefaut = "System";
    int cinDefaut = 99999999;
    int telephoneDefaut = 12345678;
    QString posteDefaut = "Administrateur";

    // Vérifier si l'email existe déjà
    if (employee::verifierEmailExiste(emailDefaut)) {
        QMessageBox::information(this, "Utilisateur Existant",
                                QString("Un utilisateur avec l'email '%1' existe déjà!\n\n"
                                       "Vous pouvez vous connecter avec:\n"
                                       "Email: %1\n"
                                       "Mot de passe: %2")
                                    .arg(emailDefaut)
                                    .arg(motDePasseDefaut));
        
        // Remplir automatiquement les champs
        emailEdit->setText(emailDefaut);
        motDePasseEdit->setText(motDePasseDefaut);
        return;
    }

    // Trouver un ID_EMP disponible
    QSqlQuery maxIdQuery("SELECT MAX(ID_EMP) FROM EMPLOYE");
    int nouveauIdEmp = 100; // Valeur par défaut
    if (maxIdQuery.exec() && maxIdQuery.next()) {
        nouveauIdEmp = maxIdQuery.value(0).toInt() + 1;
    }

    // Insérer l'utilisateur avec la structure réelle de la table
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO EMPLOYE (ID_EMP, N_TEL, PRENOM, MOT_DE_PASSE, NOM_EMP, EMAIL, POSTE, CIN) "
                       "VALUES (:ID_EMP, :N_TEL, :PRENOM, :MOT_DE_PASSE, :NOM_EMP, :EMAIL, :POSTE, :CIN)");
    
    insertQuery.bindValue(":ID_EMP", nouveauIdEmp);
    insertQuery.bindValue(":N_TEL", telephoneDefaut);
    insertQuery.bindValue(":PRENOM", prenomDefaut);
    insertQuery.bindValue(":MOT_DE_PASSE", motDePasseDefaut);
    insertQuery.bindValue(":NOM_EMP", nomDefaut);
    insertQuery.bindValue(":EMAIL", emailDefaut);
    insertQuery.bindValue(":POSTE", posteDefaut);
    insertQuery.bindValue(":CIN", cinDefaut);

    if (insertQuery.exec()) {
        qDebug() << "✅ Utilisateur par défaut créé avec succès!";
        
        // Afficher les identifiants
        QString message = QString(
            "✅ UTILISATEUR CRÉÉ AVEC SUCCÈS!\n\n"
            "📧 Email: %1\n"
            "🔑 Mot de passe: %2\n"
            "👤 Nom: %3 %4\n"
            "🆔 CIN: %5\n"
            "💼 Poste: %6\n\n"
            "Les champs de connexion ont été remplis automatiquement.\n"
            "Cliquez sur 'Se Connecter' pour continuer."
        ).arg(emailDefaut)
         .arg(motDePasseDefaut)
         .arg(nomDefaut)
         .arg(prenomDefaut)
         .arg(cinDefaut)
         .arg(posteDefaut);

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("✅ Utilisateur Créé");
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox {"
            "    background-color: #2b2b2b;"
            "}"
            "QLabel {"
            "    color: white;"
            "    font-family: 'Consolas', monospace;"
            "}"
        );
        msgBox.exec();

        // Remplir automatiquement les champs de connexion
        emailEdit->setText(emailDefaut);
        motDePasseEdit->setText(motDePasseDefaut);
        
        statusLabel->setText("✓ Utilisateur créé! Vous pouvez vous connecter.");
        statusLabel->setStyleSheet("color: #5cb85c; font-weight: bold;");
    } else {
        qDebug() << "❌ Erreur lors de la création de l'utilisateur:" << insertQuery.lastError().text();
        
        QMessageBox::critical(this, "Erreur",
                             QString("Erreur lors de la création de l'utilisateur:\n%1")
                                 .arg(insertQuery.lastError().text()));
        
        statusLabel->setText("✗ Erreur lors de la création");
        statusLabel->setStyleSheet("color: #d9534f; font-weight: bold;");
    }
}
