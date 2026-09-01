#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "formateur.h"
#include "cours.h"

#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QSqlQueryModel>
#include <QHeaderView>

// ==========================================
// CONSTRUCTEUR
// ==========================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ------------------------------------------
    // Page affichée au démarrage
    // ------------------------------------------
    ui->stackedWidget->setCurrentWidget(
        ui->pageFormateurs
        );


    // ------------------------------------------
    // CONTRÔLES DE SAISIE
    // ------------------------------------------

    // ID Formateur : entier positif
    ui->editIdFormateur->setValidator(
        new QIntValidator(1, 999999, this)
        );

    // ID Cours : entier positif
    ui->editIdCours->setValidator(
        new QIntValidator(1, 999999, this)
        );

    // Durée : entier positif
    ui->editDuree->setValidator(
        new QIntValidator(1, 10000, this)
        );

    // Prix : nombre décimal positif
    QDoubleValidator *prixValidator =
        new QDoubleValidator(
            0,
            1000000,
            2,
            this
            );

    prixValidator->setNotation(
        QDoubleValidator::StandardNotation
        );

    ui->editPrix->setValidator(prixValidator);


    // ------------------------------------------
    // Charger les données au démarrage
    // ------------------------------------------

    afficherFormateurs();
    chargerFormateursCombo();
}


// ==========================================
// DESTRUCTEUR
// ==========================================

MainWindow::~MainWindow()
{
    delete ui;
}


// ==========================================
// NAVIGATION
// ==========================================

void MainWindow::on_btnFormateurs_clicked()
{
    ui->stackedWidget->setCurrentWidget(
        ui->pageFormateurs
        );

    afficherFormateurs();
}


void MainWindow::on_btnCours_clicked()
{
    chargerFormateursCombo();

    ui->stackedWidget->setCurrentWidget(
        ui->pageCours
        );

    afficherCours();
}


// ==========================================
// FORMATEUR : AJOUTER
// ==========================================

void MainWindow::on_btnAjouterFormateur_clicked()
{
    // Vérification champs obligatoires
    if (ui->editIdFormateur->text().isEmpty() ||
        ui->editNom->text().trimmed().isEmpty() ||
        ui->editPrenom->text().trimmed().isEmpty() ||
        ui->editEmail->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Champs obligatoires",
            "Veuillez remplir ID, Nom, Prénom et Email."
            );

        return;
    }


    int id =
        ui->editIdFormateur->text().toInt();

    QString nom =
        ui->editNom->text().trimmed();

    QString prenom =
        ui->editPrenom->text().trimmed();

    QString email =
        ui->editEmail->text().trimmed();

    QString telephone =
        ui->editTelephone->text().trimmed();

    QString specialite =
        ui->editSpecialite->text().trimmed();


    Formateur formateur(
        id,
        nom,
        prenom,
        email,
        telephone,
        specialite
        );


    if (formateur.ajouter())
    {
        QMessageBox::information(
            this,
            "Succès",
            "Formateur ajouté avec succès."
            );

        afficherFormateurs();

        chargerFormateursCombo();

        viderChampsFormateur();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'ajouter le formateur."
            );
    }
}


// ==========================================
// FORMATEUR : MODIFIER
// ==========================================

void MainWindow::on_btnModifierFormateur_clicked()
{
    if (ui->editIdFormateur->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez saisir l'ID du formateur."
            );

        return;
    }


    int id =
        ui->editIdFormateur->text().toInt();

    QString nom =
        ui->editNom->text().trimmed();

    QString prenom =
        ui->editPrenom->text().trimmed();

    QString email =
        ui->editEmail->text().trimmed();

    QString telephone =
        ui->editTelephone->text().trimmed();

    QString specialite =
        ui->editSpecialite->text().trimmed();


    if (nom.isEmpty() ||
        prenom.isEmpty() ||
        email.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Nom, prénom et email sont obligatoires."
            );

        return;
    }


    Formateur formateur(
        id,
        nom,
        prenom,
        email,
        telephone,
        specialite
        );


    if (formateur.modifier())
    {
        QMessageBox::information(
            this,
            "Succès",
            "Formateur modifié avec succès."
            );

        afficherFormateurs();

        chargerFormateursCombo();

        viderChampsFormateur();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de modifier le formateur."
            );
    }
}


// ==========================================
// FORMATEUR : SUPPRIMER
// ==========================================

void MainWindow::on_btnSupprimerFormateur_clicked()
{
    if (ui->editIdFormateur->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez saisir l'ID du formateur à supprimer."
            );

        return;
    }


    int id =
        ui->editIdFormateur->text().toInt();


    QMessageBox::StandardButton confirmation;

    confirmation = QMessageBox::question(
        this,
        "Confirmation",
        "Voulez-vous vraiment supprimer ce formateur ?",
        QMessageBox::Yes | QMessageBox::No
        );


    if (confirmation != QMessageBox::Yes)
    {
        return;
    }


    Formateur formateur;


    if (formateur.supprimer(id))
    {
        QMessageBox::information(
            this,
            "Succès",
            "Formateur supprimé avec succès."
            );

        afficherFormateurs();

        chargerFormateursCombo();

        viderChampsFormateur();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Suppression impossible.\n"
            "Le formateur possède peut-être des cours."
            );
    }
}


// ==========================================
// FORMATEUR : AFFICHER
// ==========================================

void MainWindow::on_btnAfficherFormateurs_clicked()
{
    afficherFormateurs();
}


void MainWindow::afficherFormateurs()
{
    Formateur formateur;

    ui->tableFormateurs->setModel(
        formateur.afficher()
        );

    ui->tableFormateurs
        ->resizeColumnsToContents();

    ui->tableFormateurs
        ->horizontalHeader()
        ->setStretchLastSection(true);
}


// ==========================================
// FORMATEUR : VIDER LES CHAMPS
// ==========================================

void MainWindow::viderChampsFormateur()
{
    ui->editIdFormateur->clear();
    ui->editNom->clear();
    ui->editPrenom->clear();
    ui->editEmail->clear();
    ui->editTelephone->clear();
    ui->editSpecialite->clear();

    ui->editIdFormateur->setFocus();
}


// ==========================================
// COMBO BOX DES FORMATEURS
// ==========================================

void MainWindow::chargerFormateursCombo()
{
    ui->comboFormateur->clear();


    Formateur formateur;

    QSqlQueryModel *model =
        formateur.listePourCombo();


    if (model == nullptr)
    {
        return;
    }


    for (int ligne = 0;
         ligne < model->rowCount();
         ligne++)
    {
        int id =
            model->data(
                     model->index(ligne, 0)
                     ).toInt();


        QString nom =
            model->data(
                     model->index(ligne, 1)
                     ).toString();


        QString prenom =
            model->data(
                     model->index(ligne, 2)
                     ).toString();


        QString nomComplet =
            nom + " " + prenom;


        // texte affiché = nom + prénom
        // donnée cachée = ID_FORMATEUR
        ui->comboFormateur->addItem(
            nomComplet,
            id
            );
    }


    delete model;
}


// ==========================================
// COURS : AJOUTER
// ==========================================

void MainWindow::on_btnAjouterCours_clicked()
{
    if (ui->editIdCours->text().isEmpty() ||
        ui->editNomCours->text().trimmed().isEmpty() ||
        ui->editDuree->text().isEmpty() ||
        ui->editPrix->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Champs obligatoires",
            "Veuillez remplir tous les champs du cours."
            );

        return;
    }


    if (ui->comboFormateur->currentIndex() == -1)
    {
        QMessageBox::warning(
            this,
            "Formateur",
            "Veuillez choisir un formateur."
            );

        return;
    }


    int idCours =
        ui->editIdCours->text().toInt();

    QString nomCours =
        ui->editNomCours->text().trimmed();

    int duree =
        ui->editDuree->text().toInt();

    double prix =
        ui->editPrix->text().toDouble();

    int idFormateur =
        ui->comboFormateur
            ->currentData()
            .toInt();


    Cours cours(
        idCours,
        nomCours,
        duree,
        prix,
        idFormateur
        );


    if (cours.ajouter())
    {
        QMessageBox::information(
            this,
            "Succès",
            "Cours ajouté avec succès."
            );

        afficherCours();

        viderChampsCours();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible d'ajouter le cours."
            );
    }
}


// ==========================================
// COURS : MODIFIER
// ==========================================

void MainWindow::on_btnModifierCours_clicked()
{
    if (ui->editIdCours->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez saisir l'ID du cours."
            );

        return;
    }


    if (ui->comboFormateur->currentIndex() == -1)
    {
        QMessageBox::warning(
            this,
            "Formateur",
            "Veuillez choisir un formateur."
            );

        return;
    }


    int idCours =
        ui->editIdCours->text().toInt();

    QString nomCours =
        ui->editNomCours->text().trimmed();

    int duree =
        ui->editDuree->text().toInt();

    double prix =
        ui->editPrix->text().toDouble();

    int idFormateur =
        ui->comboFormateur
            ->currentData()
            .toInt();


    if (nomCours.isEmpty() ||
        duree <= 0 ||
        prix < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Les données du cours ne sont pas valides."
            );

        return;
    }


    Cours cours(
        idCours,
        nomCours,
        duree,
        prix,
        idFormateur
        );


    if (cours.modifier())
    {
        QMessageBox::information(
            this,
            "Succès",
            "Cours modifié avec succès."
            );

        afficherCours();

        viderChampsCours();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de modifier le cours."
            );
    }
}


// ==========================================
// COURS : SUPPRIMER
// ==========================================

void MainWindow::on_btnSupprimerCours_clicked()
{
    if (ui->editIdCours->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez saisir l'ID du cours à supprimer."
            );

        return;
    }


    int idCours =
        ui->editIdCours->text().toInt();


    QMessageBox::StandardButton confirmation;

    confirmation = QMessageBox::question(
        this,
        "Confirmation",
        "Voulez-vous vraiment supprimer ce cours ?",
        QMessageBox::Yes | QMessageBox::No
        );


    if (confirmation != QMessageBox::Yes)
    {
        return;
    }


    Cours cours;


    if (cours.supprimer(idCours))
    {
        QMessageBox::information(
            this,
            "Succès",
            "Cours supprimé avec succès."
            );

        afficherCours();

        viderChampsCours();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de supprimer le cours."
            );
    }
}


// ==========================================
// COURS : AFFICHER
// ==========================================

void MainWindow::on_btnAfficherCours_clicked()
{
    afficherCours();
}


void MainWindow::afficherCours()
{
    Cours cours;

    ui->tableCours->setModel(
        cours.afficher()
        );

    ui->tableCours
        ->resizeColumnsToContents();

    ui->tableCours
        ->horizontalHeader()
        ->setStretchLastSection(true);
}


// ==========================================
// COURS : VIDER LES CHAMPS
// ==========================================

void MainWindow::viderChampsCours()
{
    ui->editIdCours->clear();
    ui->editNomCours->clear();
    ui->editDuree->clear();
    ui->editPrix->clear();

    if (ui->comboFormateur->count() > 0)
    {
        ui->comboFormateur->setCurrentIndex(0);
    }

    ui->editIdCours->setFocus();
}