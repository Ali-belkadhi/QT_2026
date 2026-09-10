#include "formateur.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <utility>


// =====================================================
// CONSTRUCTEURS
// =====================================================

Formateur::Formateur()
    : idFormateur(0),
    salaire(0)
{
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
    : idFormateur(idFormateur),
    nom(nom),
    prenom(prenom),
    email(email),
    telephone(telephone),
    specialite(specialite),
    salaire(salaire),
    dateEmbauche(dateEmbauche)
{
}


// =====================================================
// AJOUTER
// =====================================================

bool Formateur::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO FORMATEUR "
        "(ID_FORMATEUR, NOM, PRENOM, EMAIL, TELEPHONE, "
        "SPECIALITE, SALAIRE, DATE_EMBAUCHE) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, "
        "TO_DATE(?, 'YYYY-MM-DD'))"
        );

    query.addBindValue(idFormateur);
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(telephone);
    query.addBindValue(specialite);
    query.addBindValue(salaire);

    query.addBindValue(
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


// =====================================================
// MODIFIER
// =====================================================

bool Formateur::modifier()
{
    QSqlQuery query;

    query.prepare(
        "UPDATE FORMATEUR SET "
        "NOM = ?, "
        "PRENOM = ?, "
        "EMAIL = ?, "
        "TELEPHONE = ?, "
        "SPECIALITE = ?, "
        "SALAIRE = ?, "
        "DATE_EMBAUCHE = TO_DATE(?, 'YYYY-MM-DD') "
        "WHERE ID_FORMATEUR = ?"
        );


    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(telephone);
    query.addBindValue(specialite);
    query.addBindValue(salaire);

    query.addBindValue(
        dateEmbauche.toString("yyyy-MM-dd")
        );

    query.addBindValue(idFormateur);


    if (!query.exec())
    {
        qDebug()
        << "Erreur modification formateur :"
        << query.lastError().text();

        return false;
    }


    return query.numRowsAffected() > 0;
}


// =====================================================
// SUPPRIMER
// =====================================================

bool Formateur::supprimer(int id)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM FORMATEUR "
        "WHERE ID_FORMATEUR = ?"
        );

    query.addBindValue(id);


    if (!query.exec())
    {
        qDebug()
        << "Erreur suppression formateur :"
        << query.lastError().text();

        return false;
    }


    return query.numRowsAffected() > 0;
}


// =====================================================
// AFFICHER
// =====================================================

QSqlQueryModel *Formateur::afficher()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    QSqlQuery query;

    query.prepare(
        "SELECT "
        "ID_FORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "TRIM(TO_CHAR(SALAIRE, 'FM99999990.000')) || ' TND' AS SALAIRE, "
        "TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') "
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
        "Nom"
        );

    model->setHeaderData(
        2,
        Qt::Horizontal,
        "Prénom"
        );

    model->setHeaderData(
        3,
        Qt::Horizontal,
        "Email"
        );

    model->setHeaderData(
        4,
        Qt::Horizontal,
        "Téléphone"
        );

    model->setHeaderData(
        5,
        Qt::Horizontal,
        "Spécialité"
        );

    model->setHeaderData(
        6,
        Qt::Horizontal,
        "Salaire"
        );

    model->setHeaderData(
        7,
        Qt::Horizontal,
        "Date embauche"
        );


    return model;
}


// =====================================================
// RECHERCHE MULTICRITERE
// =====================================================

QSqlQueryModel *Formateur::rechercher(
    const QString &texte
    )
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    QSqlQuery query;


    query.prepare(
        "SELECT "
        "ID_FORMATEUR, "
        "NOM, "
        "PRENOM, "
        "EMAIL, "
        "TELEPHONE, "
        "SPECIALITE, "
        "TRIM(TO_CHAR(SALAIRE, 'FM99999990.000')) || ' TND' AS SALAIRE, "
        "TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY') "
        "FROM FORMATEUR "
        "WHERE "
        "LOWER(TO_CHAR(ID_FORMATEUR)) LIKE ? "
        "OR LOWER(NOM) LIKE ? "
        "OR LOWER(PRENOM) LIKE ? "
        "OR LOWER(EMAIL) LIKE ? "
        "OR LOWER(TELEPHONE) LIKE ? "
        "OR LOWER(SPECIALITE) LIKE ? "
        "OR LOWER(TRIM(TO_CHAR(SALAIRE, 'FM99999990.000')) || ' TND') LIKE ? "
        "OR LOWER(TO_CHAR(SALAIRE)) LIKE ? "
        "OR LOWER(TO_CHAR(DATE_EMBAUCHE, 'DD/MM/YYYY')) LIKE ? "
        "ORDER BY ID_FORMATEUR"
        );


    QString valeur =
        "%"
        + texte.trimmed().toLower()
        + "%";


    // 9 critères
    for (int i = 0; i < 9; i++)
    {
        query.addBindValue(
            valeur
            );
    }


    if (!query.exec())
    {
        qDebug()
        << "Erreur recherche formateur :"
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
        "Nom"
        );

    model->setHeaderData(
        2,
        Qt::Horizontal,
        "Prénom"
        );

    model->setHeaderData(
        3,
        Qt::Horizontal,
        "Email"
        );

    model->setHeaderData(
        4,
        Qt::Horizontal,
        "Téléphone"
        );

    model->setHeaderData(
        5,
        Qt::Horizontal,
        "Spécialité"
        );

    model->setHeaderData(
        6,
        Qt::Horizontal,
        "Salaire"
        );

    model->setHeaderData(
        7,
        Qt::Horizontal,
        "Date embauche"
        );


    return model;
}


// =====================================================
// LISTE POUR COMBO COURS
// =====================================================

QSqlQueryModel *Formateur::listePourCombo()
{
    QSqlQueryModel *model =
        new QSqlQueryModel();


    QSqlQuery query;

    query.prepare(
        "SELECT "
        "ID_FORMATEUR, "
        "NOM, "
        "PRENOM "
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


    model->setQuery(
        std::move(query)
        );


    return model;
}


// =====================================================
// GETTERS
// =====================================================

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


// =====================================================
// SETTERS
// =====================================================

void Formateur::setIdFormateur(int id)
{
    idFormateur = id;
}

void Formateur::setNom(
    const QString &nom
    )
{
    this->nom = nom;
}

void Formateur::setPrenom(
    const QString &prenom
    )
{
    this->prenom = prenom;
}

void Formateur::setEmail(
    const QString &email
    )
{
    this->email = email;
}

void Formateur::setTelephone(
    const QString &telephone
    )
{
    this->telephone = telephone;
}

void Formateur::setSpecialite(
    const QString &specialite
    )
{
    this->specialite = specialite;
}

void Formateur::setSalaire(
    double salaire
    )
{
    this->salaire = salaire;
}

void Formateur::setDateEmbauche(
    const QDate &date
    )
{
    dateEmbauche = date;
}