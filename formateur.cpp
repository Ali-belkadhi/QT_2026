#include "formateur.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <utility>


Formateur::Formateur()
{
    idFormateur = 0;
    salaire = 0;
}


Formateur::Formateur(
    int idFormateur,
    const QString &nom,
    const QString &prenom,
    const QString &email,
    const QString &telephone,
    const QString &specialite,
    double salaire,
    const QDate &dateEmbauche
    )
{
    this->idFormateur = idFormateur;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->telephone = telephone;
    this->specialite = specialite;
    this->salaire = salaire;
    this->dateEmbauche = dateEmbauche;
}


// ======================================================
// AJOUTER
// ======================================================

bool Formateur::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO FORMATEUR "
        "(ID_FORMATEUR, NOM, PRENOM, EMAIL, TELEPHONE, "
        "SPECIALITE, SALAIRE, DATE_EMBAUCHE) "
        "VALUES "
        "(:id, :nom, :prenom, :email, :telephone, "
        ":specialite, :salaire, "
        "TO_DATE(:dateEmbauche, 'YYYY-MM-DD'))"
        );

    query.bindValue(":id", idFormateur);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);
    query.bindValue(":salaire", salaire);

    query.bindValue(
        ":dateEmbauche",
        dateEmbauche.toString("yyyy-MM-dd")
        );

    if (!query.exec())
    {
        qDebug()
        << "Erreur ajout formateur :"
        << query.lastError().text();

        return false;
    }

    return true;
}


// ======================================================
// MODIFIER
// ======================================================

bool Formateur::modifier()
{
    QSqlQuery query;

    query.prepare(
        "UPDATE FORMATEUR SET "
        "NOM = :nom, "
        "PRENOM = :prenom, "
        "EMAIL = :email, "
        "TELEPHONE = :telephone, "
        "SPECIALITE = :specialite, "
        "SALAIRE = :salaire, "
        "DATE_EMBAUCHE = TO_DATE(:dateEmbauche, 'YYYY-MM-DD') "
        "WHERE ID_FORMATEUR = :id"
        );

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);
    query.bindValue(":salaire", salaire);

    query.bindValue(
        ":dateEmbauche",
        dateEmbauche.toString("yyyy-MM-dd")
        );

    query.bindValue(":id", idFormateur);

    if (!query.exec())
    {
        qDebug()
        << "Erreur modification formateur :"
        << query.lastError().text();

        return false;
    }

    return query.numRowsAffected() > 0;
}


// ======================================================
// SUPPRIMER
// ======================================================

bool Formateur::supprimer(int id)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM FORMATEUR "
        "WHERE ID_FORMATEUR = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug()
        << "Erreur suppression formateur :"
        << query.lastError().text();

        return false;
    }

    return query.numRowsAffected() > 0;
}


// ======================================================
// AFFICHER
// ======================================================

QSqlQueryModel *Formateur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT "
        "ID_FORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "SALAIRE, "
        "TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') "
        "AS DATE_EMBAUCHE "
        "FROM FORMATEUR "
        "ORDER BY ID_FORMATEUR"
        );

    if (!query.exec())
    {
        qDebug()
        << "Erreur affichage formateurs :"
        << query.lastError().text();

        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));

    model->setHeaderData(
        0, Qt::Horizontal, "ID"
        );

    model->setHeaderData(
        1, Qt::Horizontal, "Nom"
        );

    model->setHeaderData(
        2, Qt::Horizontal, "Prénom"
        );

    model->setHeaderData(
        3, Qt::Horizontal, "Email"
        );

    model->setHeaderData(
        4, Qt::Horizontal, "Téléphone"
        );

    model->setHeaderData(
        5, Qt::Horizontal, "Spécialité"
        );

    model->setHeaderData(
        6, Qt::Horizontal, "Salaire"
        );

    model->setHeaderData(
        7, Qt::Horizontal, "Date d'embauche"
        );

    return model;
}


// ======================================================
// LISTE POUR COMBOBOX
// ======================================================

QSqlQueryModel *Formateur::listePourCombo()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QSqlQuery query;

    query.prepare(
        "SELECT ID_FORMATEUR, NOM, PRENOM "
        "FROM FORMATEUR "
        "ORDER BY NOM, PRENOM"
        );

    if (!query.exec())
    {
        qDebug()
        << "Erreur liste formateurs :"
        << query.lastError().text();

        delete model;
        return nullptr;
    }

    model->setQuery(std::move(query));

    return model;
}


// ======================================================
// GETTERS
// ======================================================

int Formateur::getIdFormateur() const
{
    return idFormateur;
}

QString Formateur::getNom() const
{
    return nom;
}

QString Formateur::getPrenom() const
{
    return prenom;
}

QString Formateur::getEmail() const
{
    return email;
}

QString Formateur::getTelephone() const
{
    return telephone;
}

QString Formateur::getSpecialite() const
{
    return specialite;
}

double Formateur::getSalaire() const
{
    return salaire;
}

QDate Formateur::getDateEmbauche() const
{
    return dateEmbauche;
}


// ======================================================
// SETTERS
// ======================================================

void Formateur::setIdFormateur(int id)
{
    idFormateur = id;
}

void Formateur::setNom(const QString &nom)
{
    this->nom = nom;
}

void Formateur::setPrenom(const QString &prenom)
{
    this->prenom = prenom;
}

void Formateur::setEmail(const QString &email)
{
    this->email = email;
}

void Formateur::setTelephone(const QString &telephone)
{
    this->telephone = telephone;
}

void Formateur::setSpecialite(const QString &specialite)
{
    this->specialite = specialite;
}

void Formateur::setSalaire(double salaire)
{
    this->salaire = salaire;
}

void Formateur::setDateEmbauche(const QDate &date)
{
    dateEmbauche = date;
}