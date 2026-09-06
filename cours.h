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
    int duree;
    double prix;
    QDate dateDebut;
    int idFormateur;

public:
    Cours();

    Cours(
        int idCours,
        const QString &nomCours,
        const QString &description,
        const QString &niveau,
        int duree,
        double prix,
        const QDate &dateDebut,
        int idFormateur
        );

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);

    QSqlQueryModel *afficher();

    // Getters
    int getIdCours() const;
    QString getNomCours() const;
    QString getDescription() const;
    QString getNiveau() const;
    int getDuree() const;
    double getPrix() const;
    QDate getDateDebut() const;
    int getIdFormateur() const;

    // Setters
    void setIdCours(int id);
    void setNomCours(const QString &nom);
    void setDescription(const QString &description);
    void setNiveau(const QString &niveau);
    void setDuree(int duree);
    void setPrix(double prix);
    void setDateDebut(const QDate &date);
    void setIdFormateur(int id);
};

#endif // COURS_H