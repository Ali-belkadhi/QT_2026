#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QSqlQueryModel>

class Cours
{
private:
    int idCours;
    QString nomCours;
    int duree;
    double prix;
    int idFormateur;

public:
    Cours();

    Cours(
        int idCours,
        QString nomCours,
        int duree,
        double prix,
        int idFormateur
        );

    bool ajouter();
    bool modifier();
    bool supprimer(int id);

    QSqlQueryModel* afficher();

    int getIdCours() const;
    QString getNomCours() const;
    int getDuree() const;
    double getPrix() const;
    int getIdFormateur() const;

    void setIdCours(int id);
    void setNomCours(QString nom);
    void setDuree(int duree);
    void setPrix(double prix);
    void setIdFormateur(int id);
};

#endif // COURS_H