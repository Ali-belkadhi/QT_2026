#include "statformateur.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QFrame>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QFont>


// =====================================================
// CONSTRUCTEUR
// =====================================================

StatFormateur::StatFormateur(QWidget *parent)
    : QDialog(parent),
    labelTotal(nullptr),
    labelSalaireMin(nullptr),
    labelSalaireMax(nullptr),
    chartViewSpecialite(nullptr),
    chartViewSalaire(nullptr)
{
    construireInterface();

    actualiser();
}


// =====================================================
// INTERFACE
// =====================================================

void StatFormateur::construireInterface()
{
    setWindowTitle("Statistiques des Formateurs");

    resize(1000, 720);

    setStyleSheet(
        // Fond principal
        "QDialog {"
        "  background-color: #0f1923;"
        "}"

        // Titre global
        "QLabel#titre {"
        "  color: white;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  padding: 10px 0px 5px 0px;"
        "}"

        // Cartes statistiques
        "QFrame#carte {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "    stop:0 #1e2d40, stop:1 #162030);"
        "  border: 1px solid #2a3f58;"
        "  border-radius: 12px;"
        "  padding: 5px;"
        "}"

        "QLabel#carteValeur {"
        "  color: #4fc3f7;"
        "  font-size: 26px;"
        "  font-weight: bold;"
        "  background: transparent;"
        "}"

        "QLabel#carteTitre {"
        "  color: #90a4ae;"
        "  font-size: 12px;"
        "  font-weight: normal;"
        "  background: transparent;"
        "}"

        "QLabel#carteIcone {"
        "  font-size: 22px;"
        "  background: transparent;"
        "}"

        // Onglets
        "QTabWidget::pane {"
        "  border: 1px solid #2a3f58;"
        "  border-radius: 8px;"
        "  background-color: #162030;"
        "}"

        "QTabBar::tab {"
        "  background-color: #1e2d40;"
        "  color: #90a4ae;"
        "  padding: 10px 24px;"
        "  margin-right: 3px;"
        "  border-top-left-radius: 8px;"
        "  border-top-right-radius: 8px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"

        "QTabBar::tab:selected {"
        "  background-color: #1565c0;"
        "  color: white;"
        "}"

        "QTabBar::tab:hover {"
        "  background-color: #1976d2;"
        "  color: white;"
        "}"
    );


    QVBoxLayout *layoutPrincipal =
        new QVBoxLayout(this);

    layoutPrincipal->setSpacing(12);
    layoutPrincipal->setContentsMargins(16, 12, 16, 12);


    // =================================================
    // TITRE
    // =================================================

    QLabel *titre =
        new QLabel(
            "📊  STATISTIQUES DES FORMATEURS",
            this
            );

    titre->setObjectName("titre");

    titre->setAlignment(Qt::AlignCenter);

    layoutPrincipal->addWidget(titre);


    // =================================================
    // SEPARATEUR
    // =================================================

    QFrame *separateur = new QFrame(this);

    separateur->setFrameShape(QFrame::HLine);

    separateur->setStyleSheet(
        "color: #2a3f58;"
        "background-color: #2a3f58;"
        "border: none;"
        "max-height: 1px;"
    );

    layoutPrincipal->addWidget(separateur);


    // =================================================
    // CARTES STATISTIQUES (4 cartes)
    // =================================================

    QHBoxLayout *layoutCartes =
        new QHBoxLayout();

    layoutCartes->setSpacing(12);


    // Fonction lambda pour créer une carte
    auto creerCarte = [this](
        const QString &icone,
        const QString &titreCarte,
        const QString &valeurDefaut,
        QLabel *&refValeur
        ) -> QFrame *
    {
        QFrame *carte =
            new QFrame(this);

        carte->setObjectName("carte");

        carte->setMinimumHeight(90);

        QVBoxLayout *layout =
            new QVBoxLayout(carte);

        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(4);


        QLabel *lIcone =
            new QLabel(icone, carte);

        lIcone->setObjectName("carteIcone");
        lIcone->setAlignment(Qt::AlignCenter);


        QLabel *lTitre =
            new QLabel(titreCarte, carte);

        lTitre->setObjectName("carteTitre");
        lTitre->setAlignment(Qt::AlignCenter);


        refValeur =
            new QLabel(valeurDefaut, carte);

        refValeur->setObjectName("carteValeur");
        refValeur->setAlignment(Qt::AlignCenter);


        layout->addWidget(lIcone);
        layout->addWidget(lTitre);
        layout->addWidget(refValeur);


        return carte;
    };


    QFrame *carteTotal =
        creerCarte(
            "👥",
            "TOTAL FORMATEURS",
            "0",
            labelTotal
            );


    QFrame *carteMin =
        creerCarte(
            "📉",
            "SALAIRE MINIMUM",
            "0 TND",
            labelSalaireMin
            );


    QFrame *carteMax =
        creerCarte(
            "📈",
            "SALAIRE MAXIMUM",
            "0 TND",
            labelSalaireMax
            );


    // Couleurs d'accent différentes par carte
    carteTotal->setStyleSheet(
        "QFrame#carte {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "    stop:0 #1a2e4a, stop:1 #0d1f30);"
        "  border: 1px solid #1565c0;"
        "  border-radius: 12px;"
        "  padding: 5px;"
        "}"
    );

    carteMin->setStyleSheet(
        "QFrame#carte {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "    stop:0 #3a2a1a, stop:1 #231a0d);"
        "  border: 1px solid #e65100;"
        "  border-radius: 12px;"
        "  padding: 5px;"
        "}"
        "QLabel#carteValeur { color: #ffa726; background: transparent; }"
    );

    carteMax->setStyleSheet(
        "QFrame#carte {"
        "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "    stop:0 #3a1a2e, stop:1 #230d1f);"
        "  border: 1px solid #7b1fa2;"
        "  border-radius: 12px;"
        "  padding: 5px;"
        "}"
        "QLabel#carteValeur { color: #ce93d8; background: transparent; }"
    );


    layoutCartes->addWidget(carteTotal);
    layoutCartes->addWidget(carteMin);
    layoutCartes->addWidget(carteMax);


    layoutPrincipal->addLayout(layoutCartes);


    // =================================================
    // ONGLETS GRAPHIQUES
    // =================================================

    QTabWidget *onglets =
        new QTabWidget(this);


    // --- Onglet 1 : Camembert par spécialité ---

    QWidget *pageSpecialite =
        new QWidget();

    QVBoxLayout *layoutSpec =
        new QVBoxLayout(pageSpecialite);


    QChart *chartSpec =
        new QChart();

    chartSpec->setTitle("Répartition des formateurs par spécialité");

    chartSpec->setBackgroundBrush(
        QBrush(QColor("#162030"))
        );

    chartSpec->setTitleBrush(
        QBrush(QColor("white"))
        );


    QFont fontTitre;
    fontTitre.setPointSize(12);
    fontTitre.setBold(true);

    chartSpec->setTitleFont(fontTitre);


    chartViewSpecialite =
        new QChartView(
            chartSpec,
            pageSpecialite
            );

    chartViewSpecialite->setRenderHint(
        QPainter::Antialiasing
        );

    chartViewSpecialite->setStyleSheet(
        "background: transparent;"
        );


    layoutSpec->addWidget(chartViewSpecialite);

    onglets->addTab(
        pageSpecialite,
        "🎓  Par Spécialité"
        );


    // --- Onglet 2 : Barres salaire par spécialité ---

    QWidget *pageSalaire =
        new QWidget();

    QVBoxLayout *layoutSal =
        new QVBoxLayout(pageSalaire);


    QChart *chartSal =
        new QChart();

    chartSal->setTitle("Salaire moyen par spécialité (TND)");

    chartSal->setBackgroundBrush(
        QBrush(QColor("#162030"))
        );

    chartSal->setTitleBrush(
        QBrush(QColor("white"))
        );

    chartSal->setTitleFont(fontTitre);


    chartViewSalaire =
        new QChartView(
            chartSal,
            pageSalaire
            );

    chartViewSalaire->setRenderHint(
        QPainter::Antialiasing
        );

    chartViewSalaire->setStyleSheet(
        "background: transparent;"
        );


    layoutSal->addWidget(chartViewSalaire);

    onglets->addTab(
        pageSalaire,
        "💵  Salaires par Spécialité"
        );


    layoutPrincipal->addWidget(onglets);

    layoutPrincipal->setStretch(0, 0);
    layoutPrincipal->setStretch(1, 0);
    layoutPrincipal->setStretch(2, 0);
    layoutPrincipal->setStretch(3, 1);
}


// =====================================================
// ACTUALISER
// =====================================================

void StatFormateur::actualiser()
{
    // =================================================
    // TOTAL FORMATEURS
    // =================================================

    int total = 0;


    QSqlQuery queryTotal;


    queryTotal.prepare(
        "SELECT COUNT(*) "
        "FROM FORMATEUR"
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
        << "Erreur statistiques total formateurs :"
        << queryTotal.lastError().text();
    }


    labelTotal->setText(
        QString::number(total)
        );


    // =================================================
    // STATISTIQUES SALAIRES
    // =================================================

    double salaireMin = 0.0;
    double salaireMax = 0.0;


    QSqlQuery querySalaire;


    querySalaire.prepare(
        "SELECT "
        "NVL(MIN(SALAIRE), 0), "
        "NVL(MAX(SALAIRE), 0) "
        "FROM FORMATEUR"
        );


    if (querySalaire.exec() &&
        querySalaire.next())
    {
        salaireMin = querySalaire.value(0).toDouble();
        salaireMax = querySalaire.value(1).toDouble();
    }
    else
    {
        qDebug()
        << "Erreur statistiques salaires :"
        << querySalaire.lastError().text();
    }


    labelSalaireMin->setText(
        QString::number(salaireMin, 'f', 2) + " TND"
        );


    labelSalaireMax->setText(
        QString::number(salaireMax, 'f', 2) + " TND"
        );



    // =================================================
    // GRAPHIQUE 1 : CAMEMBERT PAR SPECIALITE
    // =================================================

    QPieSeries *serieSpec =
        new QPieSeries();


    QSqlQuery querySpecialite;


    querySpecialite.prepare(
        "SELECT "
        "NVL(SPECIALITE, 'Non définie'), "
        "COUNT(*) "
        "FROM FORMATEUR "
        "GROUP BY SPECIALITE "
        "ORDER BY COUNT(*) DESC"
        );


    // Palette de couleurs vives
    QList<QColor> couleurs = {
        QColor("#42a5f5"),   // Bleu
        QColor("#66bb6a"),   // Vert
        QColor("#ffa726"),   // Orange
        QColor("#ef5350"),   // Rouge
        QColor("#ab47bc"),   // Violet
        QColor("#26c6da"),   // Cyan
        QColor("#ffee58"),   // Jaune
        QColor("#ff7043"),   // Orange foncé
        QColor("#26a69a"),   // Teal
        QColor("#ec407a"),   // Rose
    };


    int indexCouleur = 0;


    if (querySpecialite.exec())
    {
        while (querySpecialite.next())
        {
            QString specialite =
                querySpecialite.value(0).toString();


            int nombre =
                querySpecialite.value(1).toInt();


            QPieSlice *slice =
                serieSpec->append(
                    specialite
                        + " ("
                        + QString::number(nombre)
                        + ")",
                    nombre
                    );


            slice->setColor(
                couleurs[indexCouleur % couleurs.size()]
                );

            slice->setLabelColor(Qt::white);

            slice->setLabelVisible(true);

            indexCouleur++;
        }
    }
    else
    {
        qDebug()
        << "Erreur statistiques spécialité :"
        << querySpecialite.lastError().text();
    }


    // Légère explosion du plus grand secteur
    QList<QPieSlice *> slices = serieSpec->slices();

    if (!slices.isEmpty())
    {
        slices.first()
            ->setExploded(true);

        slices.first()
            ->setExplodeDistanceFactor(0.07);
    }


    QChart *chartSpec =
        chartViewSpecialite->chart();

    chartSpec->removeAllSeries();
    chartSpec->addSeries(serieSpec);

    chartSpec->legend()->setVisible(true);
    chartSpec->legend()->setAlignment(Qt::AlignRight);

    chartSpec->legend()->setLabelColor(Qt::white);

    QFont fontLegende;
    fontLegende.setPointSize(9);

    chartSpec->legend()->setFont(fontLegende);


    // =================================================
    // GRAPHIQUE 2 : BARRES SALAIRE PAR SPECIALITE
    // =================================================

    QSqlQuery querySalSpec;


    querySalSpec.prepare(
        "SELECT "
        "NVL(SPECIALITE, 'Non définie'), "
        "NVL(AVG(SALAIRE), 0), "
        "NVL(MIN(SALAIRE), 0), "
        "NVL(MAX(SALAIRE), 0) "
        "FROM FORMATEUR "
        "GROUP BY SPECIALITE "
        "ORDER BY AVG(SALAIRE) DESC"
        );


    QBarSet *setMoyen =
        new QBarSet("Salaire moyen");

    QBarSet *setMin =
        new QBarSet("Salaire min");

    QBarSet *setMax =
        new QBarSet("Salaire max");


    setMoyen->setColor(QColor("#42a5f5"));
    setMin->setColor(QColor("#ffa726"));
    setMax->setColor(QColor("#66bb6a"));


    QStringList categories;


    if (querySalSpec.exec())
    {
        while (querySalSpec.next())
        {
            categories
                << querySalSpec.value(0).toString();

            *setMoyen
                << querySalSpec.value(1).toDouble();

            *setMin
                << querySalSpec.value(2).toDouble();

            *setMax
                << querySalSpec.value(3).toDouble();
        }
    }
    else
    {
        qDebug()
        << "Erreur salaires par spécialité :"
        << querySalSpec.lastError().text();
    }


    QBarSeries *barSeries =
        new QBarSeries();

    barSeries->append(setMoyen);
    barSeries->append(setMin);
    barSeries->append(setMax);


    QChart *chartSal =
        chartViewSalaire->chart();

    chartSal->removeAllSeries();


    // Nettoyer les anciens axes
    for (QAbstractAxis *axe : chartSal->axes())
    {
        chartSal->removeAxis(axe);
    }


    chartSal->addSeries(barSeries);


    // Axe X : catégories
    QBarCategoryAxis *axeX =
        new QBarCategoryAxis();

    axeX->append(categories);

    axeX->setLabelsColor(Qt::white);

    QFont fontAxe;
    fontAxe.setPointSize(8);

    axeX->setLabelsFont(fontAxe);

    axeX->setGridLineColor(QColor("#2a3f58"));

    chartSal->addAxis(axeX, Qt::AlignBottom);

    barSeries->attachAxis(axeX);


    // Axe Y : valeurs
    QValueAxis *axeY =
        new QValueAxis();

    axeY->setTitleText("TND");

    axeY->setTitleBrush(QBrush(Qt::white));

    axeY->setLabelsBrush(QBrush(Qt::white));

    axeY->setLabelsFont(fontAxe);

    axeY->setGridLineColor(QColor("#2a3f58"));

    axeY->setLinePenColor(QColor("#2a3f58"));

    chartSal->addAxis(axeY, Qt::AlignLeft);

    barSeries->attachAxis(axeY);


    chartSal->legend()->setVisible(true);
    chartSal->legend()->setAlignment(Qt::AlignBottom);
    chartSal->legend()->setLabelColor(Qt::white);
    chartSal->legend()->setFont(fontLegende);
}