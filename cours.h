#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class Cours
{
private:
    int idCours;
    QString nomCours;
    QString description;
    QString niveau;
    QString duree;
    double prix;
    QDate dateDebut;
    int idFormateur;

public:
    // Constructeurs
    Cours();

    Cours(
        int idCours,
        const QString &nomCours,
        const QString &description,
        const QString &niveau,
        const QString &duree,
        double prix,
        const QDate &dateDebut,
        int idFormateur
        );

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);

    // Affichage
    QSqlQueryModel *afficher();

    // Recherche multicritère
    QSqlQueryModel *rechercher(const QString &texte);

    // Getters
    int getIdCours() const;
    QString getNomCours() const;
    QString getDescription() const;
    QString getNiveau() const;
    QString getDuree() const;
    double getPrix() const;
    QDate getDateDebut() const;
    int getIdFormateur() const;

    // Setters
    void setIdCours(int id);
    void setNomCours(const QString &nom);
    void setDescription(const QString &description);
    void setNiveau(const QString &niveau);
    void setDuree(const QString &duree);
    void setPrix(double prix);
    void setDateDebut(const QDate &date);
    void setIdFormateur(int id);
};

#endif // COURS_H