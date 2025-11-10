#include "dechet.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QtDebug>
#include <QObject>

//-------------------------------------------
// Constructeurs
//-------------------------------------------

Dechet::Dechet()
    : m_cn(0), m_volumePoids(0.0)
{}

Dechet::Dechet(int cn, const QDate& date_collecte,
               const QString& lieu, double volume_poids,
               const QString& etat)
    : m_cn(cn),
    m_dateCollecte(date_collecte),
    m_lieu(lieu),
    m_volumePoids(volume_poids),
    m_etat(etat)
{}

//-------------------------------------------
// ✅ AJOUTER
//-------------------------------------------
bool Dechet::ajouter()
{
    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO ANIS.DECHETS
            (ID, DATE_COLLECTE, LIEU_COLLECTE, VOLUME_POIDS, ETAT)
        VALUES
            (:id, TO_DATE(:d,'YYYY-MM-DD'), :lieu, :vol, :etat)
    )");

    q.bindValue(":id", m_cn);
    q.bindValue(":d", m_dateCollecte.toString("yyyy-MM-dd"));
    q.bindValue(":lieu", m_lieu);
    q.bindValue(":vol", m_volumePoids);
    q.bindValue(":etat", m_etat);

    bool ok = q.exec();
    if (!ok)
        qDebug() << "❌ Erreur insertion:" << q.lastError().text();
    return ok;
}

//-------------------------------------------
// ✅ AFFICHER
//-------------------------------------------
QSqlQueryModel* Dechet::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(R"(
        SELECT ID AS CN, DATE_COLLECTE, LIEU_COLLECTE AS LIEU,
               VOLUME_POIDS, ETAT
        FROM ANIS.DECHETS
        ORDER BY ID
    )");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Cn"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date de collecte"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Lieu de collecte"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Volume/Poids"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));

    return model;
}

//-------------------------------------------
// ✅ SUPPRIMER
//-------------------------------------------
bool Dechet::supprimer(int cn)
{
    QSqlQuery q;
    q.prepare("DELETE FROM ANIS.DECHETS WHERE ID = :id");
    q.bindValue(":id", cn);

    bool ok = q.exec();
    if (!ok)
        qDebug() << "❌ Erreur suppression:" << q.lastError().text();
    return ok;
}

//-------------------------------------------
// ✅ MODIFIER
//-------------------------------------------
bool Dechet::modifier()
{
    if (m_cn <= 0) {
        qDebug() << "❌ ID invalide pour modification";
        return false;
    }

    QSqlQuery q;
    q.prepare(R"(
        UPDATE ANIS.DECHETS
        SET DATE_COLLECTE = TO_DATE(:d,'YYYY-MM-DD'),
            LIEU_COLLECTE = :lieu,
            VOLUME_POIDS = :vol,
            ETAT = :etat
        WHERE ID = :id
    )");

    q.bindValue(":d", m_dateCollecte.toString("yyyy-MM-dd"));
    q.bindValue(":lieu", m_lieu);
    q.bindValue(":vol", m_volumePoids);
    q.bindValue(":etat", m_etat);
    q.bindValue(":id", m_cn);

    bool ok = q.exec();
    if (!ok) {
        qDebug() << "❌ Erreur modification:" << q.lastError().text();

        QSqlQuery check;
        check.prepare("SELECT COUNT(*) FROM ANIS.DECHETS WHERE ID = :id");
        check.bindValue(":id", m_cn);
        if (check.exec() && check.next() && check.value(0).toInt() == 0)
            qDebug() << "⚠️ L'ID n'existe pas dans la table.";

        return false;
    }

    if (q.numRowsAffected() == 0) {
        qDebug() << "⚠️ Aucun enregistrement modifié. L'ID peut ne pas exister.";
        return false;
    }

    return true;
}

//-------------------------------------------
// ✅ TRI PERSONNALISÉ
//-------------------------------------------
QString Dechet::mapColonne(const QString& colUi)
{
    if (colUi.compare("CN", Qt::CaseInsensitive) == 0) return "ID";
    if (colUi.compare("LIEU", Qt::CaseInsensitive) == 0) return "LIEU_COLLECTE";
    if (colUi.compare("DATE_COLLECTE", Qt::CaseInsensitive) == 0) return "DATE_COLLECTE";
    if (colUi.compare("VOLUME_POIDS", Qt::CaseInsensitive) == 0) return "VOLUME_POIDS";
    if (colUi.compare("ETAT", Qt::CaseInsensitive) == 0) return "ETAT";
    return "ID";
}

bool Dechet::colonneValide(const QString& col)
{
    static const QStringList ok = {
        "CN", "DATE_COLLECTE", "LIEU", "VOLUME_POIDS", "ETAT",
        "cn", "date_collecte", "lieu", "volume_poids", "etat"
    };
    return ok.contains(col);
}

QSqlQueryModel* Dechet::afficherTriePar(const QString& colonne, bool asc)
{
    QString colPhysique = mapColonne(colonne);
    QString ordre = asc ? "ASC" : "DESC";

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(QString(R"(
        SELECT ID AS CN, DATE_COLLECTE, LIEU_COLLECTE AS LIEU,
               VOLUME_POIDS, ETAT
        FROM ANIS.DECHETS
        ORDER BY %1 %2
    )").arg(colPhysique, ordre));

    return model;
}

//-------------------------------------------
// ✅ RECHERCHER
//-------------------------------------------
QSqlQueryModel* Dechet::rechercher(const QString& motif)
{
    QSqlQuery q;
    q.prepare(R"(
        SELECT ID AS CN, DATE_COLLECTE, LIEU_COLLECTE AS LIEU,
               VOLUME_POIDS, ETAT
        FROM ANIS.DECHETS
        WHERE TO_CHAR(ID) LIKE :m
           OR UPPER(LIEU_COLLECTE) LIKE :m
           OR UPPER(ETAT) LIKE :m
    )");

    q.bindValue(":m", "%" + motif.toUpper() + "%");
    q.exec();

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(q);
    return model;
}
