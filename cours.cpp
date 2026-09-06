#include "cours.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <utility>


Cours::Cours()
{
    idCours = 0;
    duree = 0;
    prix = 0;
    idFormateur = 0;
}


Cours::Cours(
    int idCours,
    const QString &nomCours,
    const QString &description,
    const QString &niveau,
    int duree,
    double prix,
    const QDate &dateDebut,
    int idFormateur
    )
{
    this->idCours = idCours;
    this->nomCours = nomCours;
    this->description = description;
    this->niveau = niveau;
    this->duree = duree;
    this->prix = prix;
    this->dateDebut = dateDebut;
    this->idFormateur = idFormateur;
}


// =====================================================
// AJOUTER
// =====================================================

bool Cours::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO COURS "
        "(ID_COURS, NOM_COURS, DESCRIPTION, NIVEAU, "
        "DUREE, PRIX, DATE_DEBUT, ID_FORMATEUR) "
        "VALUES "
        "(:id, :nom, :description, :niveau, "
        ":duree, :prix, TO_DATE(:dateDebut, 'YYYY-MM-DD'), :formateur)"
        );

    query.bindValue(":id", idCours);
    query.bindValue(":nom", nomCours);
    query.bindValue(":description", description);
    query.bindValue(":niveau", niveau);
    query.bindValue(":duree", duree);
    query.bindValue(":prix", prix);

    query.bindValue(
        ":dateDebut",
        dateDebut.toString("yyyy-MM-dd")
        );

    query.bindValue(
        ":formateur",
        idFormateur
        );

    if (!query.exec())
    {
        qDebug()
        << "Erreur ajout cours :"
        << query.lastError().text();

        return false;
    }

    return true;
}


// =====================================================
// MODIFIER
// =====================================================

bool Cours::modifier()
{
    QSqlQuery query;

    query.prepare(
        "UPDATE COURS SET "
        "NOM_COURS = :nom, "
        "DESCRIPTION = :description, "
        "NIVEAU = :niveau, "
        "DUREE = :duree, "
        "PRIX = :prix, "
        "DATE_DEBUT = TO_DATE(:dateDebut, 'YYYY-MM-DD'), "
        "ID_FORMATEUR = :formateur "
        "WHERE ID_COURS = :id"
        );

    query.bindValue(":nom", nomCours);
    query.bindValue(":description", description);
    query.bindValue(":niveau", niveau);
    query.bindValue(":duree", duree);
    query.bindValue(":prix", prix);

    query.bindValue(
        ":dateDebut",
        dateDebut.toString("yyyy-MM-dd")
        );

    query.bindValue(
        ":formateur",
        idFormateur
        );

    query.bindValue(
        ":id",
        idCours
        );

    if (!query.exec())
    {
        qDebug()
        << "Erreur modification cours :"
        << query.lastError().text();

        return false;
    }

    return query.numRowsAffected() > 0;
}


// =====================================================
// SUPPRIMER
// =====================================================

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
        qDebug()
        << "Erreur suppression cours :"
        << query.lastError().text();

        return false;
    }

    return query.numRowsAffected() > 0;
}


// =====================================================
// AFFICHER
// =====================================================

QSqlQueryModel *Cours::afficher()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "ID_COURS, "
        "NOM_COURS, "
        "DESCRIPTION, "
        "NIVEAU, "
        "DUREE, "
        "PRIX, "
        "TO_CHAR(DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT, "
        "ID_FORMATEUR "
        "FROM COURS "
        "ORDER BY ID_COURS"
        );

    if (!query.exec())
    {
        qDebug()
        << "Erreur affichage cours :"
        << query.lastError().text();

        delete model;
        return nullptr;
    }

    model->setQuery(
        std::move(query)
        );

    model->setHeaderData(
        0,
        Qt::Horizontal,
        "ID"
        );

    model->setHeaderData(
        1,
        Qt::Horizontal,
        "Cours"
        );

    model->setHeaderData(
        2,
        Qt::Horizontal,
        "Description"
        );

    model->setHeaderData(
        3,
        Qt::Horizontal,
        "Niveau"
        );

    model->setHeaderData(
        4,
        Qt::Horizontal,
        "Durée"
        );

    model->setHeaderData(
        5,
        Qt::Horizontal,
        "Prix"
        );

    model->setHeaderData(
        6,
        Qt::Horizontal,
        "Date début"
        );

    model->setHeaderData(
        7,
        Qt::Horizontal,
        "ID Formateur"
        );

    return model;
}


// =====================================================
// GETTERS
// =====================================================

int Cours::getIdCours() const
{
    return idCours;
}

QString Cours::getNomCours() const
{
    return nomCours;
}

QString Cours::getDescription() const
{
    return description;
}

QString Cours::getNiveau() const
{
    return niveau;
}

int Cours::getDuree() const
{
    return duree;
}

double Cours::getPrix() const
{
    return prix;
}

QDate Cours::getDateDebut() const
{
    return dateDebut;
}

int Cours::getIdFormateur() const
{
    return idFormateur;
}


// =====================================================
// SETTERS
// =====================================================

void Cours::setIdCours(int id)
{
    idCours = id;
}

void Cours::setNomCours(
    const QString &nom
    )
{
    nomCours = nom;
}

void Cours::setDescription(
    const QString &description
    )
{
    this->description = description;
}

void Cours::setNiveau(
    const QString &niveau
    )
{
    this->niveau = niveau;
}

void Cours::setDuree(int duree)
{
    this->duree = duree;
}

void Cours::setPrix(double prix)
{
    this->prix = prix;
}

void Cours::setDateDebut(
    const QDate &date
    )
{
    dateDebut = date;
}

void Cours::setIdFormateur(int id)
{
    idFormateur = id;
}