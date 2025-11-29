#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>

class Connection
{
public:
    Connection();
    bool createconnect();

    // Méthodes statiques pour la récupération de mot de passe
    static QString genererCodeVerification();
    static QString genererMotDePasseTemporaire();
    static bool envoyerEmail(const QString &email, const QString &sujet, const QString &message);
};

#endif // CONNECTION_H
