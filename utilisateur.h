#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <QString>


// =====================================================
// CLASSE UTILISATEUR
// Gestion de l'authentification via la table UTILISATEUR
// (Oracle / QODBC)
// =====================================================

class Utilisateur
{
public:

    Utilisateur();


    // =====================================================
    // AUTHENTIFICATION
    // Retourne true si email + password correspondent
    // à un enregistrement valide dans la table UTILISATEUR
    // =====================================================

    bool authentifier(
        const QString &email,
        const QString &password
        );
};


#endif // UTILISATEUR_H
