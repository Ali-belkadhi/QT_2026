#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

bool Database::connect()
{
    // Vérifier si la connexion existe déjà
    if (QSqlDatabase::contains())
    {
        QSqlDatabase db = QSqlDatabase::database();

        if (db.isOpen())
            return true;
    }

    // Une seule connexion avec QODBC
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    // Nom du DSN Windows
    db.setDatabaseName("centre");

    // Utilisateur Oracle
    db.setUserName("CENTRE");

    // Mot de passe Oracle
    db.setPassword("Centre123");

    if (!db.open())
    {
        qDebug() << "Erreur connexion Oracle :"
                 << db.lastError().text();

        return false;
    }

    qDebug() << "Connexion Oracle réussie avec QODBC !";

    return true;
}

void Database::close()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (db.isOpen())
    {
        db.close();
    }
}