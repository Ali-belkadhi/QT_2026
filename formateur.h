#ifndef FORMATEUR_H
#define FORMATEUR_H

#include <QString>
#include <QSqlQueryModel>

class Formateur
{
private:
    int idFormateur;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    QString specialite;

public:
    // Constructeurs
    Formateur();

    Formateur(
        int idFormateur,
        const QString &nom,
        const QString &prenom,
        const QString &email,
        const QString &telephone,
        const QString &specialite
        );

    // =========================
    // CRUD
    // =========================

    bool ajouter();
    bool modifier();
    bool supprimer(int id);

    QSqlQueryModel *afficher();

    // Pour remplir le QComboBox des cours
    QSqlQueryModel *listePourCombo();

    // =========================
    // GETTERS
    // =========================

    int getIdFormateur() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getSpecialite() const;

    // =========================
    // SETTERS
    // =========================

    void setIdFormateur(int id);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setSpecialite(const QString &specialite);
};

#endif // FORMATEUR_H