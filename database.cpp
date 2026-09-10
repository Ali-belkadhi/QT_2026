#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>


bool Database::connect()
{
    // -------------------------------------------------
    // Si une connexion "qt_sql_default_connection" existe déjà
    // -------------------------------------------------

    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        QSqlDatabase db =
            QSqlDatabase::database(
                QSqlDatabase::defaultConnection
                );


        // Déjà ouverte → rien à faire
        if (db.isOpen())
        {
            return true;
        }


        // Existe mais fermée → essayer de rouvrir
        if (db.open())
        {
            qDebug() << "Connexion Oracle réouverte avec succès.";
            return true;
        }
        else
        {
            qDebug()
                << "Erreur réouverture Oracle :"
                << db.lastError().text();

            // Supprimer la connexion morte avant d'en recréer une
            QSqlDatabase::removeDatabase(
                QSqlDatabase::defaultConnection
                );
        }
    }


    // -------------------------------------------------
    // Créer une nouvelle connexion QODBC
    // -------------------------------------------------

    QSqlDatabase db =
        QSqlDatabase::addDatabase("QODBC");


    // Nom du DSN Windows
    db.setDatabaseName("centre");


    // Utilisateur Oracle
    db.setUserName("CENTRE");


    // Mot de passe Oracle
    db.setPassword("Centre123");


    if (!db.open())
    {
        qDebug()
            << "Erreur connexion Oracle :"
            << db.lastError().text();

        return false;
    }


    qDebug() << "Connexion Oracle réussie avec QODBC !";

    return true;
}


void Database::close()
{
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        QSqlDatabase db =
            QSqlDatabase::database(
                QSqlDatabase::defaultConnection
                );

        if (db.isOpen())
        {
            db.close();
        }
    }
}