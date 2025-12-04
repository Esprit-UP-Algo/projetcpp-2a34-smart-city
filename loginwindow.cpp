#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "passwordrecovery.h"
#include "employee.h"
#include "facedialog.h"
#include "database_manager.h"
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
#include <QDateTime>
#include <QDate>
#include <QFrame>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    
    // Appliquer le style gris clair avec texte foncé pour meilleure lisibilité
    this->setStyleSheet("QDialog, QWidget { background-color: #f0f0f0; color: #003366; }"
                        "QLineEdit { background-color: white; border: 2px solid #3399ff; }"
                        "QPushButton { background-color: #4CAF50; color: white; }");
    
    // Charger le logo
    QPixmap logoPixmap(":/assets/c__Users_anisk_AppData_Roaming_Cursor_User_workspaceStorage_222809712738002fa20cdf442d8feefc_images_image-000e32bd-7527-43bf-b331-c1b120b7bd4a.png");
    if (!logoPixmap.isNull()) {
        ui->label_21->setPixmap(logoPixmap);
        ui->label_21->setScaledContents(true);
        qDebug() << "✅ Logo chargé avec succès";
    } else {
        qDebug() << "❌ Erreur: Logo non trouvé";
    }
    
    // Créer le gestionnaire de base de données
    dbManager = new DatabaseManager(this);
    
    // Connecter les signaux et slots
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginWindow::tentativeConnexion);
    connect(ui->btnMotDePasseOublie, &QPushButton::clicked, this, &LoginWindow::motDePasseOublie);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &LoginWindow::connexionParReconnaissanceFaciale);
    connect(ui->faceLoginButton, &QPushButton::clicked, this, &LoginWindow::connexionParReconnaissanceFaciale);
    connect(ui->lineEdit_cin_2, &QLineEdit::returnPressed, this, &LoginWindow::tentativeConnexion);
    connect(ui->lineEdit_cin_3, &QLineEdit::returnPressed, this, &LoginWindow::tentativeConnexion);
    
    // Configurer le bouton oeil pour afficher/masquer le mot de passe
    ui->label_25->installEventFilter(this);
    
    // Afficher la base de données au démarrage
    qDebug() << "\n=== BASE DE DONNÉES AU DÉMARRAGE ===";
    dbManager->afficherTousLesEmployes();
    qDebug() << "================================\n";
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->label_25 && event->type() == QEvent::MouseButtonPress) {
        basculerVisibiliteMotDePasse();
        return true;
    }
    return QDialog::eventFilter(obj, event);
}


void LoginWindow::tentativeConnexion()
{
    QString email = ui->lineEdit_cin_2->text().trimmed();
    QString motDePasse = ui->lineEdit_cin_3->text();

    qDebug() << "=== TENTATIVE DE CONNEXION ===";
    qDebug() << "Email:" << email;
    qDebug() << "Mot de passe:" << motDePasse;

    if (email.isEmpty() || motDePasse.isEmpty()) {
        QMessageBox::warning(this, "Champs Manquants",
                             "Veuillez entrer votre email et votre mot de passe!");
        return;
    }

    QRegularExpression regexEmail("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::warning(this, "Email Invalide", "Le format de l'email est incorrect!");
        return;
    }

    if (validerConnexion(email, motDePasse)) {
        emailConnecte = email;
        QMessageBox::information(this, "Connexion Réussie", "Bienvenue dans le système de gestion!");
        accept();
    } else {
        qDebug() << "Connexion échouée pour:" << email;
        QMessageBox::warning(this, "Erreur de Connexion", "Email ou mot de passe incorrect!");
        ui->lineEdit_cin_3->clear();
        ui->lineEdit_cin_3->setFocus();
    }
}

bool LoginWindow::validerConnexion(const QString& email, const QString& motDePasse)
{
    qDebug() << "Validation de la connexion pour:" << email;

    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, PRENOM, MOTDEPASS FROM EMPLOYE WHERE EMAIL = ?");
    query.addBindValue(email);

    qDebug() << "Requête SQL:" << query.lastQuery();
    qDebug() << "Paramètre email:" << email;
    
    if (query.exec()) {
        qDebug() << "Requête exécutée avec succès";
        
        if (query.next()) {
            QString motDePasseDB = query.value(3).toString();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();

            qDebug() << "Utilisateur trouvé:" << nom << prenom;
            qDebug() << "Mot de passe DB:" << motDePasseDB;
            qDebug() << "Mot de passe saisi:" << motDePasse;
            qDebug() << "Comparaison:" << (motDePasseDB == motDePasse ? "OK" : "ÉCHEC");

            if (motDePasseDB == motDePasse) {
                qDebug() << "✅ Mot de passe correct";
                return true;
            } else {
                qDebug() << "❌ Mot de passe incorrect";
                return false;
            }
        } else {
            qDebug() << "❌ Aucun utilisateur trouvé avec cet email";
            return false;
        }
    } else {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        qDebug() << "Requête qui a échoué:" << query.executedQuery();
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


void LoginWindow::basculerVisibiliteMotDePasse()
{
    if (ui->lineEdit_cin_3->echoMode() == QLineEdit::Password) {
        ui->lineEdit_cin_3->setEchoMode(QLineEdit::Normal);
        ui->label_25->setText("🔒");
    } else {
        ui->lineEdit_cin_3->setEchoMode(QLineEdit::Password);
        ui->label_25->setText("👁️");
    }
}

QString LoginWindow::getEmailFromEmployeeId(int employeeId)
{
    QSqlQuery query;
    QString sql = "SELECT EMAIL FROM EMPLOYE WHERE CIN = ? OR id_emp = ?";
    query.prepare(sql);
    query.addBindValue(employeeId);
    query.addBindValue(employeeId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

QString LoginWindow::getEmailFromCin(const QString& cin)
{
    QSqlQuery query;
    QString sql = "SELECT EMAIL FROM EMPLOYE WHERE CIN = ?";
    query.prepare(sql);
    query.addBindValue(cin);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

void LoginWindow::connexionParReconnaissanceFaciale()
{
    qDebug() << "=== CONNEXION PAR RECONNAISSANCE FACIALE ===";

    // Ouvrir le dialogue de reconnaissance faciale
    FaceDialog *faceDialog = new FaceDialog(this);
    
    if (faceDialog->exec() == QDialog::Accepted) {
        QString recognizedCin = faceDialog->getRecognizedCin();
        
        if (recognizedCin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", 
                                "Aucun CIN n'a été reconnu. Veuillez réessayer.");
            delete faceDialog;
            return;
        }

        qDebug() << "CIN reconnu:" << recognizedCin;

        // Récupérer l'email associé au CIN
        QString email = getEmailFromCin(recognizedCin);
        
        if (email.isEmpty()) {
            QMessageBox::warning(this, "Employé introuvable",
                                QString("Aucun employé trouvé avec le CIN: %1\n\n"
                                       "Veuillez vous connecter avec votre email et mot de passe.")
                                .arg(recognizedCin));
            delete faceDialog;
            return;
        }

        qDebug() << "Email trouvé:" << email;

        // Remplir automatiquement le champ email
        ui->lineEdit_cin_2->setText(email);

        // Mettre le focus sur le champ mot de passe
        ui->lineEdit_cin_3->setFocus();

        QMessageBox::information(this, "Reconnaissance réussie",
                                QString("Visage reconnu!\n\n"
                                       "CIN: %1\n"
                                       "Email: %2\n\n"
                                       "Veuillez entrer votre mot de passe pour finaliser la connexion.")
                                .arg(recognizedCin)
                                .arg(email));
    } else {
        qDebug() << "Reconnaissance faciale annulée";
    }

    delete faceDialog;
}

