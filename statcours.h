#ifndef STATCOURS_H
#define STATCOURS_H

#include <QDialog>

class QLabel;
class QChartView;
class QTabWidget;

class StatCours : public QDialog
{
public:
    explicit StatCours(QWidget *parent = nullptr);

    void actualiser();

private:
    // Cartes
    QLabel *labelTotal;
    QLabel *labelPrixMin;
    QLabel *labelPrixMax;

    // Graphiques
    QChartView *chartViewNiveau;
    QChartView *chartViewFormateur;

    void construireInterface();
};

#endif // STATCOURS_H