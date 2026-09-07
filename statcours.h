#ifndef STATCOURS_H
#define STATCOURS_H

#include <QDialog>

class QLabel;
class QChartView;

class StatCours : public QDialog
{
public:
    explicit StatCours(QWidget *parent = nullptr);

    void actualiser();

private:
    QLabel *labelTotal;
    QLabel *labelPrixMoyen;
    QLabel *labelDureeMoyenne;

    QChartView *chartView;

    void construireInterface();
};

#endif // STATCOURS_H