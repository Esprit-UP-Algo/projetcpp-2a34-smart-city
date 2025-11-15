#include "alerte.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

// Constructeur vide
Alerte::Alerte() {
    id = 0;
    type = "";
    id_client = 0;        // ← AJOUTÉ
    statut = "";
    description = "";
    gravite = "";
    date = QDate::currentDate();
}

// Constructeur avec paramètres
Alerte::Alerte(int i, QString t, int idc, QString s, QString d, QString g, QDate dt) {
    id = i;
    type = t;
    id_client = idc;      // ← AJOUTÉ
    statut = s;
    description = d;
    gravite = g;
    date = dt;
}

// ==================== GETTERS ====================
int Alerte::getId() {
    return id;
}

QString Alerte::getType() {
    return type;
}

int Alerte::getIdClient() {       // ← AJOUTÉ
    return id_client;
}

QString Alerte::getStatut() {
    return statut;
}

QString Alerte::getDescription() {
    return description;
}

QString Alerte::getGravite() {
    return gravite;
}

QDate Alerte::getDate() {
    return date;
}

// ==================== SETTERS ====================
void Alerte::setId(int i) {
    id = i;
}

void Alerte::setType(QString t) {
    type = t;
}

void Alerte::setIdClient(int idc) {   // ← AJOUTÉ
    id_client = idc;
}

void Alerte::setStatut(QString s) {
    statut = s;
}

void Alerte::setDescription(QString d) {
    description = d;
}

void Alerte::setGravite(QString g) {
    gravite = g;
}

void Alerte::setDate(QDate dt) {
    date = dt;
}

// ==================== CREATE ====================
bool Alerte::ajouter() {
    // ========== CONTRÔLES DE SAISIE ==========

    // 1. Vérifier que le type n'est pas vide
    if (type.trimmed().isEmpty()) {
        qDebug() << "Erreur: Le type est obligatoire";
        return false;
    }

    // 2. Vérifier que id_client est valide
    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide";
        return false;
    }

    // 3. Vérifier que le statut n'est pas vide
    if (statut.trimmed().isEmpty()) {
        qDebug() << "Erreur: Le statut est obligatoire";
        return false;
    }

    // 4. Vérifier que la gravité est valide (par exemple: Faible, Moyenne, Élevée)
    QStringList gravitesValides = {"Faible", "Moyenne", "Élevée", "Critique"};
    if (!gravitesValides.contains(gravite)) {
        qDebug() << "Erreur: Gravité invalide. Choisir parmi: Faible, Moyenne, Élevée, Critique";
        return false;
    }

    // 5. Vérifier que la date n'est pas dans le futur
    if (date > QDate::currentDate()) {
        qDebug() << "Erreur: La date ne peut pas être dans le futur";
        return false;
    }

    // ========== INSERTION DANS LA BD ==========
    QSqlQuery query;

    query.prepare("INSERT INTO ALERTE (TYPE, ID_CLIENT, STATUT, DESCRIPTION, GRAVITE, DATE_ALERTE) "
                  "VALUES (:type, :id_client, :statut, :description, :gravite, :date)");

    query.bindValue(":type", type);
    query.bindValue(":id_client", id_client);
    query.bindValue(":statut", statut);
    query.bindValue(":description", description);
    query.bindValue(":gravite", gravite);
    query.bindValue(":date", date);

    if(query.exec()) {
        qDebug() << "Alerte ajoutée avec succès!";
        return true;
    } else {
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }
}
// ==================== READ ====================
QSqlQueryModel* Alerte::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT ID, TYPE, ID_CLIENT, STATUT, DESCRIPTION, GRAVITE, DATE_ALERTE FROM ALERTE");

    // Noms des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));    // ← AJOUTÉ
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

// ==================== UPDATE ====================
bool Alerte::modifier() {
    // ========== CONTRÔLES DE SAISIE (même que ajouter) ==========
    if (type.trimmed().isEmpty()) {
        qDebug() << "Erreur: Le type est obligatoire";
        return false;
    }

    if (id_client <= 0) {
        qDebug() << "Erreur: ID client invalide";
        return false;
    }

    if (statut.trimmed().isEmpty()) {
        qDebug() << "Erreur: Le statut est obligatoire";
        return false;
    }

    QStringList gravitesValides = {"Faible", "Moyenne", "Élevée", "Critique"};
    if (!gravitesValides.contains(gravite)) {
        qDebug() << "Erreur: Gravité invalide";
        return false;
    }

    if (date > QDate::currentDate()) {
        qDebug() << "Erreur: La date ne peut pas être dans le futur";
        return false;
    }

    // ========== UPDATE ==========
    QSqlQuery query;

    query.prepare("UPDATE ALERTE SET TYPE=:type, ID_CLIENT=:id_client, STATUT=:statut, "
                  "DESCRIPTION=:description, GRAVITE=:gravite, DATE_ALERTE=:date "
                  "WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":id_client", id_client);
    query.bindValue(":statut", statut);
    query.bindValue(":description", description);
    query.bindValue(":gravite", gravite);
    query.bindValue(":date", date);

    if(query.exec()) {
        qDebug() << "Alerte modifiée avec succès!";
        return true;
    } else {
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }
}

// ==================== DELETE ====================
bool Alerte::supprimer(int id) {
    QSqlQuery query;

    query.prepare("DELETE FROM ALERTE WHERE ID = :id");
    query.bindValue(":id", id);

    if(query.exec()) {
        qDebug() << "Alerte supprimée avec succès!";
        return true;
    } else {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return false;
    }
}
// ==================== MÉTIERS ====================

// 1. RECHERCHE (par type, statut, ou gravité)
QSqlQueryModel* Alerte::rechercher(QString critere) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM ALERTE WHERE "
                  "UPPER(TYPE) LIKE :critere OR "
                  "UPPER(STATUT) LIKE :critere OR "
                  "UPPER(GRAVITE) LIKE :critere OR "
                  "UPPER(DESCRIPTION) LIKE :critere");

    query.bindValue(":critere", "%" + critere.toUpper() + "%");
    query.exec();

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

// 2. TRI (par date, gravité, ou statut)
QSqlQueryModel* Alerte::trierPar(QString colonne, QString ordre) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM ALERTE ORDER BY " + colonne + " " + ordre;
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

// 3. FILTRER PAR GRAVITÉ
QSqlQueryModel* Alerte::filtrerParGravite(QString grav) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM ALERTE WHERE GRAVITE = :gravite");
    query.bindValue(":gravite", grav);
    query.exec();

    model->setQuery(std::move(query));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Gravité"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

// 4. STATISTIQUES - Compter les alertes
int Alerte::compterAlertes() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM ALERTE");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// 5. STATISTIQUES - Compter par gravité
int Alerte::compterParGravite(QString grav) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM ALERTE WHERE GRAVITE = :gravite");
    query.bindValue(":gravite", grav);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
