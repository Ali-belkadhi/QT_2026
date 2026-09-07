#include "cours.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

#include <utility>


// =====================================================
// CONSTRUCTEUR VIDE
// =====================================================

Cours::Cours()
    : idCours(0),
    duree(0),
    prix(0.0),
    idFormateur(0)
{
}


// =====================================================
// CONSTRUCTEUR AVEC PARAMETRES
// =====================================================

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
    : idCours(idCours),
    nomCours(nomCours),
    description(description),
    niveau(niveau),
    duree(duree),
    prix(prix),
    dateDebut(dateDebut),
    idFormateur(idFormateur)
{
}


// =====================================================
// AJOUTER
// =====================================================

bool Cours::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO COURS "
        "("
        "ID_COURS, "
        "NOM_COURS, "
        "DESCRIPTION, "
        "NIVEAU, "
        "DUREE, "
        "PRIX, "
        "DATE_DEBUT, "
        "ID_FORMATEUR"
        ") "
        "VALUES "
        "(?, ?, ?, ?, ?, ?, TO_DATE(?, 'YYYY-MM-DD'), ?)"
        );

    query.addBindValue(idCours);
    query.addBindValue(nomCours);
    query.addBindValue(description);
    query.addBindValue(niveau);
    query.addBindValue(duree);
    query.addBindValue(prix);

    query.addBindValue(
        dateDebut.toString("yyyy-MM-dd")
        );

    query.addBindValue(idFormateur);


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
        "NOM_COURS = ?, "
        "DESCRIPTION = ?, "
        "NIVEAU = ?, "
        "DUREE = ?, "
        "PRIX = ?, "
        "DATE_DEBUT = TO_DATE(?, 'YYYY-MM-DD'), "
        "ID_FORMATEUR = ? "
        "WHERE ID_COURS = ?"
        );

    query.addBindValue(nomCours);
    query.addBindValue(description);
    query.addBindValue(niveau);
    query.addBindValue(duree);
    query.addBindValue(prix);

    query.addBindValue(
        dateDebut.toString("yyyy-MM-dd")
        );

    query.addBindValue(idFormateur);
    query.addBindValue(idCours);


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
        "WHERE ID_COURS = ?"
        );

    query.addBindValue(id);


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
// AFFICHER TOUS LES COURS
// =====================================================

QSqlQueryModel *Cours::afficher()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    QSqlQuery query;

    query.prepare(
        "SELECT "
        "C.ID_COURS, "
        "C.NOM_COURS, "
        "C.DESCRIPTION, "
        "C.NIVEAU, "
        "C.DUREE, "
        "C.PRIX, "
        "TO_CHAR(C.DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT, "
        "F.NOM || ' ' || F.PRENOM AS FORMATEUR, "
        "C.ID_FORMATEUR "
        "FROM COURS C "
        "LEFT JOIN FORMATEUR F "
        "ON C.ID_FORMATEUR = F.ID_FORMATEUR "
        "ORDER BY C.ID_COURS"
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
        "Nom cours"
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
        "Formateur"
        );

    model->setHeaderData(
        8,
        Qt::Horizontal,
        "ID Formateur"
        );


    return model;
}


// =====================================================
// RECHERCHE MULTICRITERE
// =====================================================

QSqlQueryModel *Cours::rechercher(
    const QString &texte
    )
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    QSqlQuery query;

    query.prepare(
        "SELECT "
        "C.ID_COURS, "
        "C.NOM_COURS, "
        "C.DESCRIPTION, "
        "C.NIVEAU, "
        "C.DUREE, "
        "C.PRIX, "
        "TO_CHAR(C.DATE_DEBUT, 'DD/MM/YYYY') AS DATE_DEBUT, "
        "F.NOM || ' ' || F.PRENOM AS FORMATEUR, "
        "C.ID_FORMATEUR "
        "FROM COURS C "
        "LEFT JOIN FORMATEUR F "
        "ON C.ID_FORMATEUR = F.ID_FORMATEUR "
        "WHERE "
        "LOWER(TO_CHAR(C.ID_COURS)) LIKE ? "
        "OR LOWER(C.NOM_COURS) LIKE ? "
        "OR LOWER(C.DESCRIPTION) LIKE ? "
        "OR LOWER(C.NIVEAU) LIKE ? "
        "OR LOWER(TO_CHAR(C.DUREE)) LIKE ? "
        "OR LOWER(TO_CHAR(C.PRIX)) LIKE ? "
        "OR LOWER(TO_CHAR(C.DATE_DEBUT, 'DD/MM/YYYY')) LIKE ? "
        "OR LOWER(F.NOM) LIKE ? "
        "OR LOWER(F.PRENOM) LIKE ? "
        "OR LOWER(F.NOM || ' ' || F.PRENOM) LIKE ? "
        "ORDER BY C.ID_COURS"
        );


    QString valeur =
        "%"
        + texte.trimmed().toLower()
        + "%";


    // Il y a 10 critères dans WHERE
    for (int i = 0; i < 10; ++i)
    {
        query.addBindValue(valeur);
    }


    if (!query.exec())
    {
        qDebug()
        << "Erreur recherche cours :"
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
        "Nom cours"
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
        "Formateur"
        );

    model->setHeaderData(
        8,
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
    this->description =
        description;
}


void Cours::setNiveau(
    const QString &niveau
    )
{
    this->niveau =
        niveau;
}


void Cours::setDuree(int duree)
{
    this->duree =
        duree;
}


void Cours::setPrix(double prix)
{
    this->prix =
        prix;
}


void Cours::setDateDebut(
    const QDate &date
    )
{
    dateDebut =
        date;
}


void Cours::setIdFormateur(int id)
{
    idFormateur =
        id;
}