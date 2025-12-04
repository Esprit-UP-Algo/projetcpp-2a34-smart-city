#include "employee.h"
#include "connection.h"
#include "faceenrolldialog.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QtDebug>
#include <QObject>
#include <QRandomGenerator>
#include <QDateTime>
#include <QPainter>
#include <QPdfWriter>
#include <QPageSize>
#include <QMarginsF>
#include <QFont>
#include <QColor>
#include <QDate>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileDialog>

employee::employee() {
    cin=0;
    nom="";
    prenom="";
    email="";
    motdepass="";
    n_telephon=0;
    poste="";
    salaire=0;
    dateEmbauche = QDate::currentDate();
    faceImagePath = "";
}

employee::employee(int cin,QString nom,QString prenom ,QString email ,QString motdepass,int n_telephon,QString poste,float salaire, QDate dateEmbauche)
{
    this->cin=cin ;
    this->nom=nom ;
    this-> prenom=prenom ;
    this ->email=email;
    this ->motdepass=motdepass ;
    this->n_telephon=n_telephon;
    this->poste=poste;
    this->salaire=salaire;
    this->dateEmbauche = dateEmbauche;
    this->faceImagePath = "";
}

employee::employee(int cin, QString nom, QString prenom, QString email, QString motdepass, int n_telephon, QString poste, float salaire, QDate dateEmbauche, QString faceImagePath)
{
    this->cin = cin;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->motdepass = motdepass;
    this->n_telephon = n_telephon;
    this->poste = poste;
    this->salaire = salaire;
    this->dateEmbauche = dateEmbauche;
    this->faceImagePath = faceImagePath;
}

// Getters
int employee::getcin() { return cin; }
QString employee::getnom() { return nom; }
QString employee::getprenom() { return prenom; }
QString employee::getemail() { return email; }
QString employee::getmotdepass() { return motdepass; }
int employee::getn_telephon() { return n_telephon; }
QString employee::getposte() { return poste; }
float employee::getsalaire() { return salaire; }
QDate employee::getDateEmbauche() { return dateEmbauche; }
QString employee::getFaceImagePath() { return faceImagePath; }

// Setters
void employee::setcin(int c) { cin = c; }
void employee::setnom(QString n) { nom = n; }
void employee::setprenom(QString p) { prenom = p; }
void employee::setemail(QString e) { email = e; }
void employee::setmotdepass(QString m) { motdepass = m; }
void employee::setn_telephon(int t) { n_telephon = t; }
void employee::setposte(QString p) { poste = p; }
void employee::setsalaire(float s) { salaire = s; }
void employee::setDateEmbauche(QDate d) { dateEmbauche = d; }
void employee::setFaceImagePath(QString path) { faceImagePath = path; }

bool employee::ajouter()
{
    QSqlQuery query;

    // ✅ GÉNÉRER UN ID UNIQUE (CIN + timestamp)
    QString idEmp = QString::number(cin) + "_" + QString::number(QDateTime::currentSecsSinceEpoch());

    query.prepare("INSERT INTO EMPLOYE (ID_EMP, CIN, NOM, PRENOM, EMAIL, MOTDEPASS, N_TELEPHON, POSTE, SALAIRE, DATE_EMBAUCHE, FACE_IMAGE_PATH) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(idEmp);
    query.addBindValue(cin);
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(motdepass);
    query.addBindValue(n_telephon);
    query.addBindValue(poste);
    query.addBindValue(salaire);
    query.addBindValue(dateEmbauche);
    query.addBindValue(faceImagePath);

    qDebug() << "🔄 Tentative d'ajout - ID_EMP:" << idEmp;

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de l'ajout:";
        qDebug() << "Message:" << query.lastError().text();
        qDebug() << "Requête:" << query.lastQuery();
        return false;
    }

    qDebug() << "✅ Employé ajouté avec succès - ID:" << idEmp;
    return true;
}

// ==================== MÉTHODES POUR LA GESTION DES VISAGES ====================

QString employee::generateFaceImagePath()
{
    // Créer le dossier faces s'il n'existe pas
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDir(appDataPath);
    if (!appDir.exists("faces")) {
        appDir.mkpath("faces");
    }
    
    QDir facesDir(appDir.filePath("faces"));
    
    // Générer un nom de fichier unique: CIN_timestamp.png
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString fileName = QString("%1_%2.png").arg(cin).arg(timestamp);
    
    return facesDir.filePath(fileName);
}

bool employee::saveFaceImage(const QImage &image)
{
    if (image.isNull()) {
        qDebug() << "❌ Image faciale nulle";
        return false;
    }
    
    QString imagePath = generateFaceImagePath();
    
    // Redimensionner l'image si nécessaire (max 800x800)
    QImage resizedImage = image;
    if (resizedImage.width() > 800 || resizedImage.height() > 800) {
        resizedImage = resizedImage.scaled(800, 800, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    // Sauvegarder l'image en PNG avec bonne qualité
    bool success = resizedImage.save(imagePath, "PNG", 90);
    
    if (success) {
        faceImagePath = imagePath;
        qDebug() << "✅ Image faciale sauvegardée:" << imagePath;
        return true;
    } else {
        qDebug() << "❌ Échec sauvegarde image faciale:" << imagePath;
        return false;
    }
}

bool employee::ajouterAvecFace(const QImage &faceImage)
{
    // D'abord sauvegarder l'image faciale
    if (!saveFaceImage(faceImage)) {
        qDebug() << "❌ Impossible de sauvegarder l'image faciale";
        return false;
    }
    
    // Puis ajouter l'employé avec le chemin de l'image
    return ajouter();
}

bool employee::modifier(int cin,QString nom,QString prenom ,QString email ,QString motdepass,int n_telephon,QString poste,float salaire, QDate dateEmbauche)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM = ?, PRENOM = ?, EMAIL = ?, "
                  "MOTDEPASS = ?, N_TELEPHON = ?, POSTE = ?, "
                  "SALAIRE = ?, DATE_EMBAUCHE = ? WHERE CIN = ?");
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(motdepass);
    query.addBindValue(n_telephon);
    query.addBindValue(poste);
    query.addBindValue(salaire);
    query.addBindValue(dateEmbauche);
    query.addBindValue(cin);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        return false;
    }

    return true;
}

QSqlQueryModel * employee::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));

    return model;
}

bool employee::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE id_emp = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }

    return true;
}

// Tri par nom
QSqlQueryModel * employee::trierParNom(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString ordre = croissant ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM EMPLOYE ORDER BY NOM " + ordre);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));

    return model;
}

// Recherche par CIN
QSqlQueryModel * employee::rechercherParCIN(int cin)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE CIN = ?");
    query.addBindValue(cin);
    
    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche par CIN:" << query.lastError().text();
        // Return empty model instead of null
        model->setQuery("SELECT * FROM EMPLOYE WHERE 1=0"); // Empty result set
        return model;
    }

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));

    return model;
}

// Fonctions statistiques
QSqlQueryModel * employee::trierParSalaire(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString ordre = croissant ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM EMPLOYE ORDER BY SALAIRE " + ordre);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));
    return model;
}

// Tri par poste
QSqlQueryModel * employee::trierParPoste(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString ordre = croissant ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM EMPLOYE ORDER BY POSTE " + ordre);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));
    return model;
}

// Tri par date d'embauche
QSqlQueryModel * employee::trierParDateEmbauche(bool croissant)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString ordre = croissant ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM EMPLOYE ORDER BY DATE_EMBAUCHE " + ordre);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));
    return model;
}

QSqlQueryModel * employee::filtrerParPoste(QString poste)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE POSTE = :poste");
    query.bindValue(":poste", poste);
    query.exec();

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("motdepass"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("n_telephon"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("poste"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("salaire"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("date_embauche"));
    return model;
}

QMap<QString, int> employee::statistiquesParPoste()
{
    QMap<QString, int> stats;
    QSqlQuery query("SELECT POSTE, COUNT(*) as nombre FROM EMPLOYE GROUP BY POSTE");

    while (query.next()) {
        QString poste = query.value(0).toString();
        int nombre = query.value(1).toInt();
        stats[poste] = nombre;
    }
    return stats;
}

QMap<QString, float> employee::salaireMoyenParPoste()
{
    QMap<QString, float> stats;
    QSqlQuery query("SELECT POSTE, AVG(SALAIRE) as moyenne FROM EMPLOYE GROUP BY POSTE");

    while (query.next()) {
        QString poste = query.value(0).toString();
        float moyenne = query.value(1).toFloat();
        stats[poste] = moyenne;
    }
    return stats;
}

QMap<QString, int> employee::statistiquesEmbaucheParMois()
{
    QMap<QString, int> stats;
    QSqlQuery query("SELECT TO_CHAR(DATE_EMBAUCHE, 'YYYY-MM'), COUNT(*) FROM EMPLOYE GROUP BY TO_CHAR(DATE_EMBAUCHE, 'YYYY-MM') ORDER BY TO_CHAR(DATE_EMBAUCHE, 'YYYY-MM')");

    while (query.next()) {
        QString mois = query.value(0).toString();
        int nombre = query.value(1).toInt();
        stats[mois] = nombre;
    }
    return stats;
}

int employee::nombreEmployesParPoste(QString poste)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE POSTE = :poste");
    query.bindValue(":poste", poste);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

float employee::salaireMoyen()
{
    QSqlQuery query("SELECT AVG(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

float employee::salaireMin()
{
    QSqlQuery query("SELECT MIN(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

float employee::salaireMax()
{
    QSqlQuery query("SELECT MAX(SALAIRE) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toFloat();
    }
    return 0.0;
}

int employee::nombreEmployes()
{
    QSqlQuery query("SELECT COUNT(*) FROM EMPLOYE");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// Méthodes statiques pour la récupération de mot de passe
bool employee::verifierEmailExiste(const QString &email)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE EMAIL = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    qDebug() << "Erreur lors de la vérification de l'email:" << query.lastError().text();
    return false;
}

QString employee::getTelephoneParEmail(const QString& email)
{
    QSqlQuery query;
    query.prepare("SELECT N_TELEPHON FROM EMPLOYE WHERE EMAIL = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "";
}

bool employee::reinitialiserMotDePasse(const QString& email)
{
    // Vérifier si l'email existe
    if (!verifierEmailExiste(email)) {
        qDebug() << "Email non trouvé:" << email;
        return false;
    }

    QString motDePasseTemporaire = Connection::genererMotDePasseTemporaire();
    qDebug() << "Mot de passe temporaire généré:" << motDePasseTemporaire;

    // Récupérer le numéro de téléphone
    QString telephone = getTelephoneParEmail(email);
    if (telephone.isEmpty()) {
        qDebug() << "Numéro de téléphone non trouvé pour l'email:" << email;
        return false;
    }

    qDebug() << "Téléphone trouvé:" << telephone;

    // Note: envoyerSMS n'est pas implémenté dans connection.cpp
    // Pour l'instant, on met juste à jour le mot de passe dans la base de données
    // Vous pouvez ajouter envoyerSMS dans connection.cpp si nécessaire

    // Mettre à jour le mot de passe dans la base de données
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE EMPLOYE SET MOTDEPASS = :motdepasse WHERE EMAIL = :email");
    updateQuery.bindValue(":motdepasse", motDePasseTemporaire);
    updateQuery.bindValue(":email", email);

    if (updateQuery.exec()) {
        qDebug() << "Mot de passe mis à jour dans la base de données";
        qDebug() << "⚠️ Note: SMS non envoyé (fonctionnalité non implémentée)";
        qDebug() << "Mot de passe temporaire:" << motDePasseTemporaire;
        return true;
    } else {
        qDebug() << "Erreur lors de la mise à jour du mot de passe:" << updateQuery.lastError().text();
    }

    return false;
}

bool employee::changerMotDePasse(const QString &email, const QString &nouveauMotDePasse)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET MOTDEPASS = :motdepasse WHERE EMAIL = :email");
    query.bindValue(":motdepasse", nouveauMotDePasse);
    query.bindValue(":email", email);

    if (query.exec()) {
        qDebug() << "Mot de passe changé avec succès pour:" << email;
        return true;
    }

    qDebug() << "Erreur lors du changement de mot de passe:" << query.lastError().text();
    return false;
}

// Méthodes pour générer l'attestation de travail
bool employee::genererAttestationTravail(int cinEmploye, const QString& nom, const QString& prenom,
                                         const QString& poste, float salaire, const QDate& dateEmbauche,
                                         const QString& cheminFichier)
{
    qDebug() << "=== GÉNÉRATION PDF HAUTE QUALITÉ ===";

    if (nom.isEmpty() || prenom.isEmpty()) {
        qDebug() << "Erreur: Nom ou prénom vide";
        return false;
    }

    // ✅ CONFIGURATION PDF OPTIMALE
    QPdfWriter pdfWriter(cheminFichier);

    // 🔥 HAUTE RÉSOLUTION (300 DPI)
    pdfWriter.setResolution(300);

    // Format A4
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setTitle("Attestation de Travail - " + nom + " " + prenom);

    // Marges en millimètres
    pdfWriter.setPageMargins(QMarginsF(25, 25, 25, 25), QPageLayout::Millimeter);

    // ✅ CRÉATION DU PAINTER AVEC ANTIALIASING
    QPainter painter(&pdfWriter);

    if (!painter.isActive()) {
        qDebug() << "Erreur: Impossible de démarrer le painter PDF";
        return false;
    }

    // 🔥 ACTIVER L'ANTIALIASING POUR TEXTE NET
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // ✅ DIMENSIONS DE LA PAGE
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int margin = pageWidth * 0.05;  // 5% de marge
    int contentWidth = pageWidth - (2 * margin);

    int currentY = margin + 200;

    // ========================
    // 📌 TITRE PRINCIPAL
    // ========================

    QFont titleFont("Arial", 28, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor(70, 130, 180));  // Bleu élégant

    QString titre = "ATTESTATION DE TRAVAIL";
    QRect titleRect(margin, currentY, contentWidth, 120);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignTop, titre);

    currentY += 180;

    // ========================
    // 📅 DATE D'ÉMISSION
    // ========================

    QFont dateFont("Arial", 11);
    painter.setFont(dateFont);
    painter.setPen(Qt::black);

    QString dateEmission = "Délivrée le " + QDate::currentDate().toString("dd/MM/yyyy");
    QRect dateRect(margin, currentY, contentWidth, 50);
    painter.drawText(dateRect, Qt::AlignLeft, dateEmission);

    currentY += 100;

    // ========================
    // 📝 INTRODUCTION
    // ========================

    QFont bodyFont("Arial", 12);
    painter.setFont(bodyFont);
    painter.setPen(Qt::black);

    QString intro = "Je soussigné(e), Directeur des Ressources Humaines de l'entreprise MALEK CORPORATION,";
    QRect introRect(margin, currentY, contentWidth, 50);
    painter.drawText(introRect, Qt::AlignLeft | Qt::TextWordWrap, intro);
    currentY += 70;

    QString atteste = "atteste que :";
    QRect attesteRect(margin, currentY, contentWidth, 50);
    painter.drawText(attesteRect, Qt::AlignLeft, atteste);
    currentY += 80;

    // ========================
    // 📊 INFORMATIONS EMPLOYÉ
    // ========================

    QFont labelFont("Arial", 12, QFont::Bold);
    QFont valueFont("Arial", 12);

    int labelWidth = 350;
    int valueX = margin + labelWidth;
    int lineHeight = 70;

    painter.setFont(labelFont);
    painter.setPen(Qt::black);

    // Monsieur/Madame
    painter.setFont(labelFont);
    painter.drawText(margin, currentY, "Monsieur/Madame :");
    painter.setFont(valueFont);
    painter.drawText(valueX, currentY, nom + " " + prenom);
    currentY += lineHeight;

    // Numéro CIN
    painter.setFont(labelFont);
    painter.drawText(margin, currentY, "Numéro CIN :");
    painter.setFont(valueFont);
    painter.drawText(valueX, currentY, QString::number(cinEmploye));
    currentY += lineHeight;

    // Poste
    painter.setFont(labelFont);
    painter.drawText(margin, currentY, "Occupe le poste de :");
    painter.setFont(valueFont);
    painter.drawText(valueX, currentY, poste);
    currentY += lineHeight;

    // Date d'embauche
    QString dateEmbaucheStr = dateEmbauche.isValid() ?
                                  dateEmbauche.toString("dd/MM/yyyy") : QDate::currentDate().toString("dd/MM/yyyy");
    painter.setFont(labelFont);
    painter.drawText(margin, currentY, "Date d'embauche :");
    painter.setFont(valueFont);
    painter.drawText(valueX, currentY, dateEmbaucheStr);
    currentY += lineHeight;

    // Salaire
    painter.setFont(labelFont);
    painter.drawText(margin, currentY, "Salaire mensuel :");
    painter.setFont(valueFont);
    painter.drawText(valueX, currentY, QString::number(salaire, 'f', 3) + " DT");
    currentY += lineHeight;

    currentY += 80;

    // ========================
    // 📜 CLAUSE FINALE
    // ========================

    painter.setFont(bodyFont);
    QString clause = "La présente attestation est délivrée pour servir et valoir ce que de droit.";
    QRect clauseRect(margin, currentY, contentWidth, 50);
    painter.drawText(clauseRect, Qt::AlignLeft | Qt::TextWordWrap, clause);
    currentY += 100;

    // Date et lieu
    QString lieu = "Fait à Tunis, le " + QDate::currentDate().toString("dd/MM/yyyy");
    QRect lieuRect(margin, currentY, contentWidth, 50);
    painter.drawText(lieuRect, Qt::AlignLeft, lieu);
    currentY += 150;

    // ========================
    // ✍️ SIGNATURE
    // ========================

    QFont signatureFont("Arial", 12, QFont::Bold);
    painter.setFont(signatureFont);

    QString directeur = "Le Directeur des Ressources Humaines";
    QRect directeurRect(margin, currentY, contentWidth, 50);
    painter.drawText(directeurRect, Qt::AlignLeft, directeur);
    currentY += 50;

    QString entreprise = "MALEK CORPORATION";
    QRect entrepriseRect(margin, currentY, contentWidth, 50);
    painter.drawText(entrepriseRect, Qt::AlignLeft, entreprise);
    currentY += 80;

    // Ligne de signature
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(margin, currentY, margin + 350, currentY);
    currentY += 40;

    QFont smallFont("Arial", 9);
    painter.setFont(smallFont);
    painter.setPen(QColor(100, 100, 100));
    QString cachet = "Signature et cachet";
    QRect cachetRect(margin, currentY, 350, 40);
    painter.drawText(cachetRect, Qt::AlignLeft, cachet);

    painter.end();

    qDebug() << "✅ PDF HAUTE QUALITÉ généré avec succès :" << cheminFichier;
    return true;
}

bool employee::genererAttestationSansDateEmbauche(int cinEmploye, const QString& nom, const QString& prenom,
                                                  const QString& poste, float salaire, const QString& cheminFichier)
{
    // Appeler la fonction principale avec la date d'aujourd'hui
    return genererAttestationTravail(cinEmploye, nom, prenom, poste, salaire, QDate::currentDate(), cheminFichier);
}

bool employee::ajouterColonneDateEmbauche()
{
    QSqlQuery query;
    bool success = query.exec("ALTER TABLE EMPLOYE ADD DATE_EMBAUCHE DATE");

    if (success) {
        qDebug() << "✅ Colonne DATE_EMBAUCHE ajoutée avec succès";

        // Mettre à jour les dates existantes avec une date par défaut
        QSqlQuery updateQuery;
        updateQuery.exec("UPDATE EMPLOYE SET DATE_EMBAUCHE = TO_DATE('2023-01-01', 'YYYY-MM-DD') WHERE DATE_EMBAUCHE IS NULL");
        qDebug() << "✅ Dates d'embauche mises à jour pour les employés existants";
    } else {
        qDebug() << "❌ Erreur lors de l'ajout de la colonne DATE_EMBAUCHE:" << query.lastError().text();
    }

    return success;
}

bool employee::ajouterColonneRFID()
{
    QSqlQuery query;
    bool success = query.exec("ALTER TABLE EMPLOYE ADD RFID_UID VARCHAR(50)");

    if (success) {
        qDebug() << "✅ Colonne RFID_UID ajoutée avec succès";
    } else {
        qDebug() << "❌ Erreur lors de l'ajout de la colonne RFID_UID:" << query.lastError().text();
        // Vérifier si la colonne existe déjà
        if (query.lastError().text().contains("duplicate column name") || 
            query.lastError().text().contains("colonne dupliquée")) {
            qDebug() << "ℹ️ La colonne RFID_UID existe déjà";
            return true;
        }
    }

    return success;
}

bool employee::verifierRFIDUID(const QString &rfid_uid)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE RFID_UID = :rfid_uid");
    query.bindValue(":rfid_uid", rfid_uid);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    qDebug() << "❌ Erreur lors de la vérification RFID UID:" << query.lastError().text();
    return false;
}

bool employee::associerRFIDAEmploye(int cin, const QString &rfid_uid)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET RFID_UID = :rfid_uid WHERE CIN = :cin");
    query.bindValue(":rfid_uid", rfid_uid);
    query.bindValue(":cin", cin);

    if (query.exec()) {
        qDebug() << "✅ RFID UID associé à l'employé CIN:" << cin;
        return true;
    } else {
        qDebug() << "❌ Erreur lors de l'association RFID:" << query.lastError().text();
        return false;
    }
}

QString employee::getEmployeNomByRFID(const QString &rfid_uid)
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM EMPLOYE WHERE RFID_UID = :rfid_uid");
    query.bindValue(":rfid_uid", rfid_uid);

    if (query.exec() && query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        return nom + " " + prenom;
    }

    return "";
}

// ==================== MÉTHODES STATIQUES POUR LA RECONNAISSANCE FACIALE ====================

QImage employee::capturerVisage(QWidget *parent)
{
    // Options: capture depuis caméra ou charger depuis fichier
    QMessageBox::StandardButton reply = QMessageBox::question(
        parent, 
        "📷 Capture de visage", 
        "Voulez-vous capturer depuis la caméra ou charger depuis un fichier ?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::Yes
    );
    
    if (reply == QMessageBox::Cancel) {
        return QImage(); // Annulé
    }
    
    if (reply == QMessageBox::Yes) {
        // Capture depuis caméra - utiliser FaceEnrollDialog sans enrôlement
        // Note: Pour l'instant, on utilise le dialogue de fichier comme fallback
        QString filePath = QFileDialog::getOpenFileName(
            parent,
            "📷 Sélectionner image du visage",
            QDir::homePath(),
            "Images (*.png *.jpg *.jpeg *.bmp)"
        );
        
        if (!filePath.isEmpty()) {
            QImage image(filePath);
            if (!image.isNull()) {
                // Redimensionner si trop grande
                if (image.width() > 1024 || image.height() > 1024) {
                    image = image.scaled(1024, 1024, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                }
                return image;
            }
        }
    } else {
        // Charger depuis fichier
        QString filePath = QFileDialog::getOpenFileName(
            parent,
            "📷 Sélectionner image du visage",
            QDir::homePath(),
            "Images (*.png *.jpg *.jpeg *.bmp)"
        );
        
        if (!filePath.isEmpty()) {
            QImage image(filePath);
            if (!image.isNull()) {
                // Redimensionner si trop grande
                if (image.width() > 1024 || image.height() > 1024) {
                    image = image.scaled(1024, 1024, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                }
                return image;
            }
        }
    }
    
    return QImage(); // Échec
}

bool employee::ajouterEmployeAvecFace(employee &emp, QWidget *parent)
{
    // Capturer le visage
    QImage faceImage = capturerVisage(parent);
    
    if (faceImage.isNull()) {
        QMessageBox::warning(parent, "⚠️ Capture annulée", 
                           "Aucune image de visage capturée. L'employé sera ajouté sans photo.");
        return emp.ajouter(); // Ajouter sans visage
    }
    
    // Ajouter avec visage
    return emp.ajouterAvecFace(faceImage);
}
