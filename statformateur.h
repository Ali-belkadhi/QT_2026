#ifndef STATFORMATEUR_H
#define STATFORMATEUR_H

#include <QDialog>

class QLabel;
class QChartView;
class QTabWidget;

class StatFormateur : public QDialog
{
public:
    explicit StatFormateur(QWidget *parent = nullptr);

    void actualiser();

private:
    QLabel *labelTotal;
    QLabel *labelSalaireMoyen;
    QLabel *labelSalaireMin;
    QLabel *labelSalaireMax;

    QChartView *chartViewSpecialite;
    QChartView *chartViewSalaire;

    void construireInterface();
};

#endif // STATFORMATEUR_H