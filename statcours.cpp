#include "statcours.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTabWidget>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <algorithm>

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
#include <QFont>


// =====================================================
// CONSTRUCTEUR
// =====================================================

StatCours::StatCours(QWidget *parent)
    : QDialog(parent),
    labelTotal(nullptr),
    labelPrixMin(nullptr),
    labelPrixMax(nullptr),
    chartViewNiveau(nullptr),
    chartViewFormateur(nullptr)
{
    construireInterface();
    actualiser();
}


// =====================================================
// INTERFACE
// =====================================================

void StatCours::construireInterface()
{
    setWindowTitle("Statistiques des Cours");

    resize(1000, 720);

    setStyleSheet(
        "QDialog {"
        "  background-color: #0f1923;"
        "}"

        "QLabel#titre {"
        "  color: white;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  padding: 10px 0px 5px 0px;"
        "}"

        "QLabel#carteValeur {"
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
            "📊  STATISTIQUES DES COURS",
            this
            );

    titre->setObjectName("titre");
    titre->setAlignment(Qt::AlignCenter);
    layoutPrincipal->addWidget(titre);


    // =================================================
    // SÉPARATEUR
    // =================================================

    QFrame *sep = new QFrame(this);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet(
        "color: #2a3f58;"
        "background-color: #2a3f58;"
        "border: none;"
        "max-height: 1px;"
    );
    layoutPrincipal->addWidget(sep);


    // =================================================
    // CARTES (4 cartes)
    // =================================================

    QHBoxLayout *layoutCartes = new QHBoxLayout();
    layoutCartes->setSpacing(12);


    auto creerCarte = [this](
        const QString &icone,
        const QString &titreCarte,
        const QString &valeurDefaut,
        QLabel *&refValeur,
        const QString &couleurBordure,
        const QString &couleurFond1,
        const QString &couleurFond2,
        const QString &couleurValeur
        ) -> QFrame *
    {
        QFrame *carte = new QFrame(this);
        carte->setMinimumHeight(90);

        carte->setStyleSheet(
            "QFrame {"
            "  background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
            "    stop:0 " + couleurFond1 + ","
            "    stop:1 " + couleurFond2 + ");"
            "  border: 1px solid " + couleurBordure + ";"
            "  border-radius: 12px;"
            "  padding: 5px;"
            "}"
        );

        QVBoxLayout *layout = new QVBoxLayout(carte);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(4);

        QLabel *lIcone = new QLabel(icone, carte);
        lIcone->setObjectName("carteIcone");
        lIcone->setAlignment(Qt::AlignCenter);

        QLabel *lTitre = new QLabel(titreCarte, carte);
        lTitre->setObjectName("carteTitre");
        lTitre->setAlignment(Qt::AlignCenter);

        refValeur = new QLabel(valeurDefaut, carte);
        refValeur->setObjectName("carteValeur");
        refValeur->setAlignment(Qt::AlignCenter);
        refValeur->setStyleSheet(
            "color: " + couleurValeur + ";"
            "background: transparent;"
            "font-size: 24px;"
            "font-weight: bold;"
        );

        layout->addWidget(lIcone);
        layout->addWidget(lTitre);
        layout->addWidget(refValeur);

        return carte;
    };


    QFrame *carteTotal =
        creerCarte(
            "📚",
            "TOTAL COURS",
            "0",
            labelTotal,
            "#1565c0", "#1a2e4a", "#0d1f30", "#4fc3f7"
            );

    QFrame *cartePrixMin =
        creerCarte(
            "📉",
            "PRIX MINIMUM",
            "0 TND",
            labelPrixMin,
            "#e65100", "#3a2a1a", "#231a0d", "#ffa726"
            );

    QFrame *cartePrixMax =
        creerCarte(
            "📈",
            "PRIX MAXIMUM",
            "0 TND",
            labelPrixMax,
            "#7b1fa2", "#3a1a2e", "#230d1f", "#ce93d8"
            );


    layoutCartes->addWidget(carteTotal);
    layoutCartes->addWidget(cartePrixMin);
    layoutCartes->addWidget(cartePrixMax);

    layoutPrincipal->addLayout(layoutCartes);


    // =================================================
    // ONGLETS GRAPHIQUES
    // =================================================

    QTabWidget *onglets = new QTabWidget(this);

    QFont fontTitre;
    fontTitre.setPointSize(12);
    fontTitre.setBold(true);

    QFont fontLegende;
    fontLegende.setPointSize(9);


    // --- Onglet 1 : Camembert par niveau ---

    QWidget *pageNiveau = new QWidget();
    QVBoxLayout *layoutNiv = new QVBoxLayout(pageNiveau);

    QChart *chartNiv = new QChart();
    chartNiv->setTitle("Répartition des cours par niveau");
    chartNiv->setBackgroundBrush(QBrush(QColor("#162030")));
    chartNiv->setTitleBrush(QBrush(Qt::white));
    chartNiv->setTitleFont(fontTitre);

    chartViewNiveau = new QChartView(chartNiv, pageNiveau);
    chartViewNiveau->setRenderHint(QPainter::Antialiasing);
    chartViewNiveau->setStyleSheet("background: transparent;");

    layoutNiv->addWidget(chartViewNiveau);

    onglets->addTab(pageNiveau, "🎓  Par Niveau");


    // --- Onglet 2 : Barres par formateur ---

    QWidget *pageFormateur = new QWidget();
    QVBoxLayout *layoutForm = new QVBoxLayout(pageFormateur);

    QChart *chartForm = new QChart();
    chartForm->setTitle("Nombre de cours par formateur");
    chartForm->setBackgroundBrush(QBrush(QColor("#162030")));
    chartForm->setTitleBrush(QBrush(Qt::white));
    chartForm->setTitleFont(fontTitre);

    chartViewFormateur = new QChartView(chartForm, pageFormateur);
    chartViewFormateur->setRenderHint(QPainter::Antialiasing);
    chartViewFormateur->setStyleSheet("background: transparent;");

    layoutForm->addWidget(chartViewFormateur);

    onglets->addTab(pageFormateur, "👨‍🏫  Par Formateur");


    layoutPrincipal->addWidget(onglets);

    layoutPrincipal->setStretch(0, 0);
    layoutPrincipal->setStretch(1, 0);
    layoutPrincipal->setStretch(2, 0);
    layoutPrincipal->setStretch(3, 1);
}


// =====================================================
// ACTUALISER
// =====================================================

void StatCours::actualiser()
{
    // =================================================
    // TOTAL COURS
    // =================================================

    int total = 0;

    QSqlQuery queryTotal;
    queryTotal.prepare("SELECT COUNT(*) FROM COURS");

    if (queryTotal.exec() && queryTotal.next())
    {
        total = queryTotal.value(0).toInt();
    }

    labelTotal->setText(QString::number(total));


    // =================================================
    // PRIX MIN / MAX
    // =================================================

    double prixMin = 0.0;
    double prixMax = 0.0;

    QSqlQuery queryPrix;
    queryPrix.prepare(
        "SELECT "
        "NVL(MIN(PRIX), 0), "
        "NVL(MAX(PRIX), 0) "
        "FROM COURS"
        );

    if (queryPrix.exec() && queryPrix.next())
    {
        prixMin = queryPrix.value(0).toDouble();
        prixMax = queryPrix.value(1).toDouble();
    }
    else
    {
        qDebug() << "Erreur prix cours :" << queryPrix.lastError().text();
    }

    labelPrixMin->setText(
        QString::number(prixMin, 'f', 2) + " TND"
        );

    labelPrixMax->setText(
        QString::number(prixMax, 'f', 2) + " TND"
        );



    // =================================================
    // GRAPHIQUE 1 : CAMEMBERT PAR NIVEAU
    // =================================================

    QPieSeries *serieNiv = new QPieSeries();

    QList<QColor> couleurs = {
        QColor("#42a5f5"),
        QColor("#66bb6a"),
        QColor("#ffa726"),
        QColor("#ef5350"),
        QColor("#ab47bc"),
        QColor("#26c6da"),
        QColor("#ffee58"),
    };

    int indexCouleur = 0;

    QSqlQuery queryNiv;
    queryNiv.prepare(
        "SELECT "
        "NVL(NIVEAU, 'Non défini'), "
        "COUNT(*) "
        "FROM COURS "
        "GROUP BY NIVEAU "
        "ORDER BY COUNT(*) DESC"
        );

    if (queryNiv.exec())
    {
        while (queryNiv.next())
        {
            QString niveau = queryNiv.value(0).toString();
            int nombre     = queryNiv.value(1).toInt();

            QPieSlice *slice =
                serieNiv->append(
                    niveau + " (" + QString::number(nombre) + ")",
                    nombre
                    );

            slice->setColor(couleurs[indexCouleur % couleurs.size()]);
            slice->setLabelColor(Qt::white);
            slice->setLabelVisible(true);

            indexCouleur++;
        }
    }
    else
    {
        qDebug() << "Erreur niveaux :" << queryNiv.lastError().text();
    }


    // Explosion du plus grand secteur
    QList<QPieSlice *> slicesNiv = serieNiv->slices();

    if (!slicesNiv.isEmpty())
    {
        slicesNiv.first()->setExploded(true);
        slicesNiv.first()->setExplodeDistanceFactor(0.07);
    }


    QChart *chartNiv = chartViewNiveau->chart();
    chartNiv->removeAllSeries();
    chartNiv->addSeries(serieNiv);

    QFont fontLegende;
    fontLegende.setPointSize(9);

    chartNiv->legend()->setVisible(true);
    chartNiv->legend()->setAlignment(Qt::AlignRight);
    chartNiv->legend()->setLabelColor(Qt::white);
    chartNiv->legend()->setFont(fontLegende);


    // =================================================
    // GRAPHIQUE 2 : BARRES PAR FORMATEUR
    // =================================================

    QSqlQuery queryForm;
    queryForm.prepare(
        "SELECT "
        "NVL(F.NOM || ' ' || F.PRENOM, 'Inconnu'), "
        "COUNT(C.ID_COURS) "
        "FROM COURS C "
        "LEFT JOIN FORMATEUR F "
        "  ON C.ID_FORMATEUR = F.ID_FORMATEUR "
        "GROUP BY F.NOM, F.PRENOM "
        "ORDER BY COUNT(C.ID_COURS) DESC"
        );


    QBarSet *setCours = new QBarSet("Nombre de cours");
    setCours->setColor(QColor("#42a5f5"));
    setCours->setLabelColor(Qt::white);

    QStringList categories;

    if (queryForm.exec())
    {
        while (queryForm.next())
        {
            categories << queryForm.value(0).toString();
            *setCours  << queryForm.value(1).toInt();
        }
    }
    else
    {
        qDebug() << "Erreur cours par formateur :" << queryForm.lastError().text();
    }


    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(setCours);
    barSeries->setLabelsVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);


    QChart *chartForm = chartViewFormateur->chart();
    chartForm->removeAllSeries();

    for (QAbstractAxis *axe : chartForm->axes())
    {
        chartForm->removeAxis(axe);
    }

    chartForm->addSeries(barSeries);


    // Axe X : noms des formateurs
    QBarCategoryAxis *axeX = new QBarCategoryAxis();
    axeX->append(categories);
    axeX->setLabelsColor(Qt::white);

    QFont fontAxe;
    fontAxe.setPointSize(8);
    axeX->setLabelsFont(fontAxe);
    axeX->setGridLineColor(QColor("#2a3f58"));

    chartForm->addAxis(axeX, Qt::AlignBottom);
    barSeries->attachAxis(axeX);


    // Axe Y : valeurs
    QValueAxis *axeY = new QValueAxis();
    axeY->setTitleText("Nombre de cours");
    axeY->setTitleBrush(QBrush(Qt::white));
    axeY->setLabelsBrush(QBrush(Qt::white));
    axeY->setLabelsFont(fontAxe);
    axeY->setGridLineColor(QColor("#2a3f58"));
    axeY->setLinePenColor(QColor("#2a3f58"));
    axeY->setTickCount(5);
    axeY->setLabelFormat("%d");

    chartForm->addAxis(axeY, Qt::AlignLeft);
    barSeries->attachAxis(axeY);


    chartForm->legend()->setVisible(false);
}