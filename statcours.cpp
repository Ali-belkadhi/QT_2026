#include "statcours.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>

#include <QPainter>


// =====================================================
// CONSTRUCTEUR
// =====================================================

StatCours::StatCours(QWidget *parent)
    : QDialog(parent),
    labelTotal(nullptr),
    labelPrixMoyen(nullptr),
    labelDureeMoyenne(nullptr),
    chartView(nullptr)
{
    construireInterface();

    actualiser();
}


// =====================================================
// INTERFACE
// =====================================================

void StatCours::construireInterface()
{
    setWindowTitle(
        "Statistiques des Cours"
        );


    resize(
        950,
        650
        );


    setStyleSheet(
        "QDialog {"
        " background-color: #172033;"
        "}"

        "QLabel {"
        " color: white;"
        "}"

        "QLabel#carte {"
        " background-color: white;"
        " color: #172033;"
        " border-radius: 10px;"
        " padding: 20px;"
        " font-size: 16px;"
        " font-weight: bold;"
        "}"
        );


    QVBoxLayout *layoutPrincipal =
        new QVBoxLayout(this);


    // =================================================
    // TITRE
    // =================================================

    QLabel *titre =
        new QLabel(
            "STATISTIQUES DES COURS",
            this
            );


    titre->setAlignment(
        Qt::AlignCenter
        );


    titre->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: white;"
        "padding: 15px;"
        );


    layoutPrincipal->addWidget(
        titre
        );


    // =================================================
    // CARTES
    // =================================================

    QHBoxLayout *layoutCartes =
        new QHBoxLayout();


    labelTotal =
        new QLabel(
            "Total\n0",
            this
            );


    labelTotal->setObjectName(
        "carte"
        );


    labelTotal->setAlignment(
        Qt::AlignCenter
        );


    labelPrixMoyen =
        new QLabel(
            "Prix moyen\n0 TND",
            this
            );


    labelPrixMoyen->setObjectName(
        "carte"
        );


    labelPrixMoyen->setAlignment(
        Qt::AlignCenter
        );


    labelDureeMoyenne =
        new QLabel(
            "Durée moyenne\n0 h",
            this
            );


    labelDureeMoyenne->setObjectName(
        "carte"
        );


    labelDureeMoyenne->setAlignment(
        Qt::AlignCenter
        );


    layoutCartes->addWidget(
        labelTotal
        );


    layoutCartes->addWidget(
        labelPrixMoyen
        );


    layoutCartes->addWidget(
        labelDureeMoyenne
        );


    layoutPrincipal->addLayout(
        layoutCartes
        );


    // =================================================
    // GRAPHIQUE
    // =================================================

    QChart *chart =
        new QChart();


    chart->setTitle(
        "Répartition des cours par niveau"
        );


    chartView =
        new QChartView(
            chart,
            this
            );


    chartView->setRenderHint(
        QPainter::Antialiasing
        );


    chartView->setMinimumHeight(
        400
        );


    layoutPrincipal->addWidget(
        chartView
        );
}


// =====================================================
// ACTUALISER
// =====================================================

void StatCours::actualiser()
{
    // =================================================
    // TOTAL COURS
    // =================================================

    int total =
        0;


    QSqlQuery queryTotal;


    queryTotal.prepare(
        "SELECT COUNT(*) "
        "FROM COURS"
        );


    if (queryTotal.exec() &&
        queryTotal.next())
    {
        total =
            queryTotal.value(0)
                .toInt();
    }
    else
    {
        qDebug()
        << "Erreur total cours :"
        << queryTotal.lastError().text();
    }


    labelTotal->setText(
        "Nombre total de cours\n"
        + QString::number(total)
        );


    // =================================================
    // PRIX MOYEN
    // =================================================

    double prixMoyen =
        0.0;


    QSqlQuery queryPrix;


    queryPrix.prepare(
        "SELECT NVL(AVG(PRIX), 0) "
        "FROM COURS"
        );


    if (queryPrix.exec() &&
        queryPrix.next())
    {
        prixMoyen =
            queryPrix.value(0)
                .toDouble();
    }


    labelPrixMoyen->setText(
        "Prix moyen\n"
        + QString::number(
            prixMoyen,
            'f',
            2
            )
        + " TND"
        );


    // =================================================
    // DUREE MOYENNE
    // =================================================

    double dureeMoyenne =
        0.0;


    QSqlQuery queryDuree;


    queryDuree.prepare(
        "SELECT NVL(AVG(DUREE), 0) "
        "FROM COURS"
        );


    if (queryDuree.exec() &&
        queryDuree.next())
    {
        dureeMoyenne =
            queryDuree.value(0)
                .toDouble();
    }


    labelDureeMoyenne->setText(
        "Durée moyenne\n"
        + QString::number(
            dureeMoyenne,
            'f',
            1
            )
        + " h"
        );


    // =================================================
    // GRAPHIQUE NIVEAU
    // =================================================

    QPieSeries *series =
        new QPieSeries();


    QSqlQuery queryNiveau;


    queryNiveau.prepare(
        "SELECT "
        "NVL(NIVEAU, 'Non défini'), "
        "COUNT(*) "
        "FROM COURS "
        "GROUP BY NIVEAU "
        "ORDER BY COUNT(*) DESC"
        );


    if (queryNiveau.exec())
    {
        while (queryNiveau.next())
        {
            QString niveau =
                queryNiveau
                    .value(0)
                    .toString();


            int nombre =
                queryNiveau
                    .value(1)
                    .toInt();


            series->append(
                niveau
                    + " ("
                    + QString::number(nombre)
                    + ")",
                nombre
                );
        }
    }
    else
    {
        qDebug()
        << "Erreur statistiques niveaux :"
        << queryNiveau
                .lastError()
                .text();
    }


    for (QPieSlice *slice :
         series->slices())
    {
        slice->setLabelVisible(
            true
            );
    }


    QChart *chart =
        chartView->chart();


    chart->removeAllSeries();


    chart->addSeries(
        series
        );


    chart->setTitle(
        "Répartition des cours par niveau"
        );


    chart->legend()
        ->setVisible(true);


    chart->legend()
        ->setAlignment(
            Qt::AlignBottom
            );
}