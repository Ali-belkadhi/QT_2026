#include "utilisateur.h"
#include "database.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>


// =====================================================
// CONSTRUCTEUR
// =====================================================

Utilisateur::Utilisateur()
{
}


// =====================================================
// AUTHENTIFICATION
// =====================================================

bool Utilisateur::authentifier(
    const QString &email,
    const QString &password
    )
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid())
    {
        qWarning() << "[Utilisateur] Connexion SQL invalide. Reconnexion...";
        if (!Database::connect())
        {
            qWarning() << "[Utilisateur] Reconnexion échouée.";
            return false;
        }
        db = QSqlDatabase::database();
    }

    if (!db.isOpen())
    {
        qWarning() << "[Utilisateur] Connexion fermée. Réouverture...";
        if (!db.open())
        {
            qWarning() << "[Utilisateur] Réouverture échouée :" << db.lastError().text();
            return false;
        }
    }

    // Récupérer le mot de passe / hash stocké pour cet email
    QSqlQuery query(db);
    query.prepare("SELECT PASSWORD_HASH FROM UTILISATEUR WHERE LOWER(TRIM(EMAIL)) = :email");
    query.bindValue(":email", email.trimmed().toLower());

    if (!query.exec())
    {
        qWarning() << "[Utilisateur] Erreur SQL authentification :" << query.lastError().text();
        return false;
    }

    if (query.next())
    {
        QString dbPass = query.value(0).toString().trimmed();
        QString inputPass = password.trimmed();

        // Calcul des différents formats de hachage possibles
        QString sha256 = QString::fromUtf8(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
        QString sha1   = QString::fromUtf8(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex());
        QString md5    = QString::fromUtf8(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());

        // 1. Comparaison mot de passe en clair (exact ou sans espaces)
        if (dbPass == password || dbPass == inputPass)
        {
            return true;
        }

        // 2. Comparaison avec les hashs (insensible à la casse hexadécimale)
        if (dbPass.compare(sha256, Qt::CaseInsensitive) == 0 ||
            dbPass.compare(sha1, Qt::CaseInsensitive) == 0 ||
            dbPass.compare(md5, Qt::CaseInsensitive) == 0)
        {
            return true;
        }

        qWarning() << "[Utilisateur] Mot de passe non correspondant pour" << email;
        qWarning() << "  -> Stocké en base :" << dbPass;
        qWarning() << "  -> Reçu (en clair) :" << password;
        qWarning() << "  -> Reçu (SHA-256)  :" << sha256;
        return false;
    }
    else
    {
        qWarning() << "[Utilisateur] Aucun utilisateur trouvé avec l'email :" << email;

        // Afficher les emails disponibles pour aider au débogage
        QSqlQuery listQuery(db);
        if (listQuery.exec("SELECT EMAIL FROM UTILISATEUR"))
        {
            qDebug() << "=== Emails existants dans la table UTILISATEUR ===";
            while (listQuery.next())
            {
                qDebug() << " - [" << listQuery.value(0).toString() << "]";
            }
            qDebug() << "==================================================";
        }

        return false;
    }
}
