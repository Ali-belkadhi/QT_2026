#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE

namespace Ui
{
class MainWindow;
}

QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:

    explicit MainWindow(
        QWidget *parent = nullptr
        );

    ~MainWindow();


private slots:

    // ======================================
    // NAVIGATION
    // ======================================

    void on_btnFormateurs_clicked();

    void on_btnCours_clicked();


    // ======================================
    // FORMATEURS
    // ======================================

    void on_btnAjouterFormateur_clicked();

    void on_btnModifierFormateur_clicked();

    void on_btnSupprimerFormateur_clicked();

    void on_btnAfficherFormateurs_clicked();


    // ======================================
    // COURS
    // ======================================

    void on_btnAjouterCours_clicked();

    void on_btnModifierCours_clicked();

    void on_btnSupprimerCours_clicked();

    void on_btnAfficherCours_clicked();


private:

    Ui::MainWindow *ui;


    // ======================================
    // FONCTIONS FORMATEURS
    // ======================================

    void afficherFormateurs();

    void viderChampsFormateur();

    void chargerFormateursCombo();


    // ======================================
    // FONCTIONS COURS
    // ======================================

    void afficherCours();

    void viderChampsCours();
};


#endif // MAINWINDOW_H