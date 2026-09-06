#ifndef FORMATEUR_H
#define FORMATEUR_H

#include <QString>
#include <QDate>
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
    double salaire;
    QDate dateEmbauche;

public:
    Formateur();

    Formateur(
        int idFormateur,
        const QString &nom,
        const QString &prenom,
        const QString &email,
        const QString &telephone,
        const QString &specialite,
        double salaire,
        const QDate &dateEmbauche
        );

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);

    QSqlQueryModel *afficher();
    QSqlQueryModel *listePourCombo();

    // Getters
    int getIdFormateur() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getSpecialite() const;
    double getSalaire() const;
    QDate getDateEmbauche() const;

    // Setters
    void setIdFormateur(int id);
    void setNom(const QString &nom);
    void setPrenom(const QString &prenom);
    void setEmail(const QString &email);
    void setTelephone(const QString &telephone);
    void setSpecialite(const QString &specialite);
    void setSalaire(double salaire);
    void setDateEmbauche(const QDate &date);
};

#endif // FORMATEUR_H