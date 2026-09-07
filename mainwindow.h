#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QString>
#include <QSqlQueryModel>
#include <QPushButton>


// =====================================================
// DECLARATIONS
// =====================================================

class StatFormateur;
class StatCours;


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

    // =====================================================
    // NAVIGATION
    // =====================================================

    void on_btnFormateurs_clicked();

    void on_btnCours_clicked();


    // =====================================================
    // FORMATEURS CRUD
    // =====================================================

    void on_btnAjouterFormateur_clicked();

    void on_btnModifierFormateur_clicked();

    void on_btnSupprimerFormateur_clicked();

    void on_btnAfficherFormateurs_clicked();


    // =====================================================
    // SELECTION FORMATEUR
    // =====================================================

    void on_tableFormateurs_clicked(
        const QModelIndex &index
        );


    // =====================================================
    // RECHERCHE FORMATEUR
    // =====================================================

    void on_editRechercheFormateur_textChanged(
        const QString &texte
        );


    // =====================================================
    // PDF FORMATEUR
    // =====================================================

    void on_btnPdfFormateur_clicked();


    // =====================================================
    // STATISTIQUES FORMATEUR
    // =====================================================

    void on_btnStatFormateur_clicked();


    // =====================================================
    // COURS CRUD
    // =====================================================

    void on_btnAjouterCours_clicked();

    void on_btnModifierCours_clicked();

    void on_btnSupprimerCours_clicked();

    void on_btnAfficherCours_clicked();


    // =====================================================
    // SELECTION COURS
    // =====================================================

    void on_tableCours_clicked(
        const QModelIndex &index
        );


    // =====================================================
    // RECHERCHE COURS
    // =====================================================

    void on_editRechercheCours_textChanged(
        const QString &texte
        );


    // =====================================================
    // PDF COURS
    // =====================================================

    void on_btnPdfCours_clicked();


    // =====================================================
    // STATISTIQUES COURS
    // =====================================================

    void on_btnStatCours_clicked();


private:

    // =====================================================
    // UI
    // =====================================================

    Ui::MainWindow *ui;


    // =====================================================
    // FENETRES STATISTIQUES
    // =====================================================

    StatFormateur *fenetreStatFormateur = nullptr;

    StatCours *fenetreStatCours = nullptr;


    // =====================================================
    // FORMATEURS
    // =====================================================

    void afficherFormateurs();

    void viderChampsFormateur();

    void chargerFormateursCombo();


    // =====================================================
    // COURS
    // =====================================================

    void afficherCours();

    void viderChampsCours();


    // =====================================================
    // CONTROLES SAISIE
    // =====================================================

    void configurerControlesSaisie();

    bool validerFormateur();

    bool validerCours();


    // =====================================================
    // TRI
    // =====================================================

    void installerModeleFormateurs(
        QSqlQueryModel *model
        );

    void installerModeleCours(
        QSqlQueryModel *model
        );

    void activerBoutonNav(
        QPushButton *actif,
        QPushButton *inactif
        );


    // =====================================================
    // GENERATION ID AUTOMATIQUE
    // =====================================================

    int genererIdFormateur();

    int genererIdCours();
};


#endif // MAINWINDOW_H