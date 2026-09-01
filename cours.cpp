#include "cours.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


Cours::Cours()
{
    idCours = 0;
    duree = 0;
    prix = 0;
    idFormateur = 0;
}


Cours::Cours(
    int idCours,
    QString nomCours,
    int duree,
    double prix,
    int idFormateur
    )
{
    this->idCours = idCours;
    this->nomCours = nomCours;
    this->duree = duree;
    this->prix = prix;
    this->idFormateur = idFormateur;
}


// =========================
// AJOUTER
// =========================

bool Cours::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO COURS "
        "(ID_COURS, NOM_COURS, DUREE, PRIX, ID_FORMATEUR) "
        "VALUES (:id, :nom, :duree, :prix, :formateur)"
        );

    query.bindValue(":id", idCours);
    query.bindValue(":nom", nomCours);
    query.bindValue(":duree", duree);
    query.bindValue(":prix", prix);
    query.bindValue(":formateur", idFormateur);

    if (!query.exec())
    {
        qDebug() << "Erreur ajout cours :"
                 << query.lastError().text();

        return false;
    }

    return true;
}


// =========================
// MODIFIER
// =========================

bool Cours::modifier()
{
    QSqlQuery query;

    query.prepare(
        "UPDATE COURS SET "
        "NOM_COURS = :nom, "
        "DUREE = :duree, "
        "PRIX = :prix, "
        "ID_FORMATEUR = :formateur "
        "WHERE ID_COURS = :id"
        );

    query.bindValue(":nom", nomCours);
    query.bindValue(":duree", duree);
    query.bindValue(":prix", prix);
    query.bindValue(":formateur", idFormateur);
    query.bindValue(":id", idCours);

    if (!query.exec())
    {
        qDebug() << "Erreur modification cours :"
                 << query.lastError().text();

        return false;
    }

    return true;
}


// =========================
// SUPPRIMER
// =========================

bool Cours::supprimer(int id)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM COURS "
        "WHERE ID_COURS = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "Erreur suppression cours :"
                 << query.lastError().text();

        return false;
    }

    return true;
}


// =========================
// AFFICHER
// =========================

QSqlQueryModel* Cours::afficher()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "C.ID_COURS, "
        "C.NOM_COURS, "
        "C.DUREE, "
        "C.PRIX, "
        "F.NOM || ' ' || F.PRENOM AS FORMATEUR "
        "FROM COURS C "
        "JOIN FORMATEUR F "
        "ON C.ID_FORMATEUR = F.ID_FORMATEUR "
        "ORDER BY C.ID_COURS"
        );

    query.exec();

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Cours");
    model->setHeaderData(2, Qt::Horizontal, "Durée");
    model->setHeaderData(3, Qt::Horizontal, "Prix");
    model->setHeaderData(4, Qt::Horizontal, "Formateur");

    return model;
}


// GETTERS

int Cours::getIdCours() const
{
    return idCours;
}

QString Cours::getNomCours() const
{
    return nomCours;
}

int Cours::getDuree() const
{
    return duree;
}

double Cours::getPrix() const
{
    return prix;
}

int Cours::getIdFormateur() const
{
    return idFormateur;
}


// SETTERS

void Cours::setIdCours(int id)
{
    idCours = id;
}

void Cours::setNomCours(QString nom)
{
    nomCours = nom;
}

void Cours::setDuree(int duree)
{
    this->duree = duree;
}

void Cours::setPrix(double prix)
{
    this->prix = prix;
}

void Cours::setIdFormateur(int id)
{
    idFormateur = id;
}