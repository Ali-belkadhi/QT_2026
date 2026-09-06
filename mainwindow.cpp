#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "formateur.h"
#include "cours.h"

#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QDate>


// =====================================================
// CONSTRUCTEUR
// =====================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // =================================================
    // PAGE PAR DEFAUT
    // =================================================

    ui->stackedWidget->setCurrentWidget(
        ui->pageFormateurs
        );


    // =================================================
    // VALIDATION FORMATEUR
    // =================================================

    // ID Formateur
    ui->editIdFormateur->setValidator(
        new QIntValidator(1, 999999, this)
        );


    // Salaire
    QDoubleValidator *salaireValidator =
        new QDoubleValidator(
            0,
            1000000,
            2,
            this
            );

    salaireValidator->setNotation(
        QDoubleValidator::StandardNotation
        );

    ui->editSalaire->setValidator(
        salaireValidator
        );


    // Téléphone : chiffres uniquement
    QRegularExpression telephoneRegex(
        "^[0-9]{0,20}$"
        );

    ui->editTelephone->setValidator(
        new QRegularExpressionValidator(
            telephoneRegex,
            this
            )
        );


    // Date embauche
    ui->dateEditEmbauche->setDate(
        QDate::currentDate()
        );

    ui->dateEditEmbauche->setCalendarPopup(true);


    // =================================================
    // VALIDATION COURS
    // =================================================

    // ID Cours
    ui->editIdCours->setValidator(
        new QIntValidator(1, 999999, this)
        );


    // Durée
    ui->editDuree->setValidator(
        new QIntValidator(1, 10000, this)
        );


    // Prix
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

    ui->editPrix->setValidator(
        prixValidator
        );


    // Date début
    ui->dateEditDebut->setDate(
        QDate::currentDate()
        );

    ui->dateEditDebut->setCalendarPopup(true);


    // =================================================
    // NIVEAUX
    // =================================================

    // Seulement si tu ne les as pas déjà ajoutés
    // dans Qt Designer

    if (ui->comboNiveau->count() == 0)
    {
        ui->comboNiveau->addItem("Débutant");
        ui->comboNiveau->addItem("Intermédiaire");
        ui->comboNiveau->addItem("Avancé");
    }


    // =================================================
    // CHARGEMENT INITIAL
    // =================================================

    afficherFormateurs();

    chargerFormateursCombo();
}


// =====================================================
// DESTRUCTEUR
// =====================================================

MainWindow::~MainWindow()
{
    delete ui;
}


// =====================================================
// NAVIGATION
// =====================================================

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



// =====================================================
// =====================================================
//                    FORMATEURS
// =====================================================
// =====================================================


// =====================================================
// AJOUTER FORMATEUR
// =====================================================

void MainWindow::on_btnAjouterFormateur_clicked()
{
    // Champs obligatoires Oracle
    if (ui->editIdFormateur->text().isEmpty() ||
        ui->editNom->text().trimmed().isEmpty() ||
        ui->editPrenom->text().trimmed().isEmpty() ||
        ui->editEmail->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "ID, Nom, Prénom et Email sont obligatoires."
            );

        return;
    }


    // -------------------------
    // Vérification email
    // -------------------------

    QString email =
        ui->editEmail->text().trimmed();

    QRegularExpression emailRegex(
        "^[\\w\\.%-]+@[\\w\\.-]+\\.[A-Za-z]{2,}$"
        );

    if (!emailRegex.match(email).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Email invalide",
            "Veuillez saisir une adresse email valide."
            );

        return;
    }


    // -------------------------
    // Récupération des champs
    // -------------------------

    int id =
        ui->editIdFormateur->text().toInt();

    QString nom =
        ui->editNom->text().trimmed();

    QString prenom =
        ui->editPrenom->text().trimmed();

    QString telephone =
        ui->editTelephone->text().trimmed();

    QString specialite =
        ui->editSpecialite->text().trimmed();


    QString texteSalaire =
        ui->editSalaire->text();

    texteSalaire.replace(",", ".");

    double salaire =
        texteSalaire.isEmpty()
            ? 0
            : texteSalaire.toDouble();


    QDate dateEmbauche =
        ui->dateEditEmbauche->date();


    // -------------------------
    // Objet Formateur
    // -------------------------

    Formateur formateur(
        id,
        nom,
        prenom,
        email,
        telephone,
        specialite,
        salaire,
        dateEmbauche
        );


    // -------------------------
    // Ajout
    // -------------------------

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


// =====================================================
// MODIFIER FORMATEUR
// =====================================================

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


    if (ui->editNom->text().trimmed().isEmpty() ||
        ui->editPrenom->text().trimmed().isEmpty() ||
        ui->editEmail->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Nom, Prénom et Email sont obligatoires."
            );

        return;
    }


    QString email =
        ui->editEmail->text().trimmed();


    QRegularExpression emailRegex(
        "^[\\w\\.%-]+@[\\w\\.-]+\\.[A-Za-z]{2,}$"
        );


    if (!emailRegex.match(email).hasMatch())
    {
        QMessageBox::warning(
            this,
            "Email invalide",
            "Veuillez saisir une adresse email valide."
            );

        return;
    }


    int id =
        ui->editIdFormateur->text().toInt();

    QString nom =
        ui->editNom->text().trimmed();

    QString prenom =
        ui->editPrenom->text().trimmed();

    QString telephone =
        ui->editTelephone->text().trimmed();

    QString specialite =
        ui->editSpecialite->text().trimmed();


    QString texteSalaire =
        ui->editSalaire->text();

    texteSalaire.replace(",", ".");

    double salaire =
        texteSalaire.isEmpty()
            ? 0
            : texteSalaire.toDouble();


    QDate dateEmbauche =
        ui->dateEditEmbauche->date();


    Formateur formateur(
        id,
        nom,
        prenom,
        email,
        telephone,
        specialite,
        salaire,
        dateEmbauche
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


// =====================================================
// SUPPRIMER FORMATEUR
// =====================================================

void MainWindow::on_btnSupprimerFormateur_clicked()
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


    QMessageBox::StandardButton confirmation =
        QMessageBox::question(
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
            "Le formateur peut être associé à un cours."
            );
    }
}


// =====================================================
// BOUTON AFFICHER FORMATEURS
// =====================================================

void MainWindow::on_btnAfficherFormateurs_clicked()
{
    afficherFormateurs();
}


// =====================================================
// AFFICHAGE TABLE FORMATEURS
// =====================================================

void MainWindow::afficherFormateurs()
{
    Formateur formateur;

    QSqlQueryModel *model =
        formateur.afficher();


    if (model == nullptr)
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les formateurs."
            );

        return;
    }


    ui->tableFormateurs->setModel(model);


    // Sélection d'une ligne complète
    ui->tableFormateurs->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tableFormateurs->setSelectionMode(
        QAbstractItemView::SingleSelection
        );


    ui->tableFormateurs
        ->resizeColumnsToContents();


    ui->tableFormateurs
        ->horizontalHeader()
        ->setStretchLastSection(true);
}


// =====================================================
// VIDER CHAMPS FORMATEUR
// =====================================================

void MainWindow::viderChampsFormateur()
{
    ui->editIdFormateur->clear();

    ui->editNom->clear();

    ui->editPrenom->clear();

    ui->editEmail->clear();

    ui->editTelephone->clear();

    ui->editSpecialite->clear();

    ui->editSalaire->clear();


    ui->dateEditEmbauche->setDate(
        QDate::currentDate()
        );


    ui->editIdFormateur->setFocus();
}


// =====================================================
// CHARGER FORMATEURS DANS COMBOBOX
// =====================================================

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
         ++ligne)
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


        // Le nom est affiché
        // L'ID est stocké en donnée cachée
        ui->comboFormateur->addItem(
            nomComplet,
            id
            );
    }


    delete model;
}



// =====================================================
// =====================================================
//                       COURS
// =====================================================
// =====================================================


// =====================================================
// AJOUTER COURS
// =====================================================

void MainWindow::on_btnAjouterCours_clicked()
{
    // ID + nom obligatoires dans Oracle
    if (ui->editIdCours->text().isEmpty() ||
        ui->editNomCours->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "ID et Nom du cours sont obligatoires."
            );

        return;
    }


    if (ui->comboFormateur->currentIndex() < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un formateur."
            );

        return;
    }


    int idCours =
        ui->editIdCours->text().toInt();


    QString nomCours =
        ui->editNomCours
            ->text()
            .trimmed();


    QString description =
        ui->editDescriptionCours
            ->text()
            .trimmed();


    QString niveau =
        ui->comboNiveau
            ->currentText();


    int duree =
        ui->editDuree
            ->text()
            .toInt();


    QString textePrix =
        ui->editPrix->text();

    textePrix.replace(",", ".");

    double prix =
        textePrix.isEmpty()
            ? 0
            : textePrix.toDouble();


    QDate dateDebut =
        ui->dateEditDebut
            ->date();


    int idFormateur =
        ui->comboFormateur
            ->currentData()
            .toInt();


    // Contrôles métier
    if (!ui->editDuree->text().isEmpty() &&
        duree <= 0)
    {
        QMessageBox::warning(
            this,
            "Durée invalide",
            "La durée doit être supérieure à zéro."
            );

        return;
    }


    if (prix < 0)
    {
        QMessageBox::warning(
            this,
            "Prix invalide",
            "Le prix ne peut pas être négatif."
            );

        return;
    }


    Cours cours(
        idCours,
        nomCours,
        description,
        niveau,
        duree,
        prix,
        dateDebut,
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


// =====================================================
// MODIFIER COURS
// =====================================================

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


    if (ui->editNomCours->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Le nom du cours est obligatoire."
            );

        return;
    }


    if (ui->comboFormateur->currentIndex() < 0)
    {
        QMessageBox::warning(
            this,
            "Attention",
            "Veuillez sélectionner un formateur."
            );

        return;
    }


    int idCours =
        ui->editIdCours->text().toInt();


    QString nomCours =
        ui->editNomCours
            ->text()
            .trimmed();


    QString description =
        ui->editDescriptionCours
            ->text()
            .trimmed();


    QString niveau =
        ui->comboNiveau
            ->currentText();


    int duree =
        ui->editDuree
            ->text()
            .toInt();


    QString textePrix =
        ui->editPrix->text();

    textePrix.replace(",", ".");

    double prix =
        textePrix.isEmpty()
            ? 0
            : textePrix.toDouble();


    QDate dateDebut =
        ui->dateEditDebut
            ->date();


    int idFormateur =
        ui->comboFormateur
            ->currentData()
            .toInt();


    Cours cours(
        idCours,
        nomCours,
        description,
        niveau,
        duree,
        prix,
        dateDebut,
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


// =====================================================
// SUPPRIMER COURS
// =====================================================

void MainWindow::on_btnSupprimerCours_clicked()
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


    int idCours =
        ui->editIdCours->text().toInt();


    QMessageBox::StandardButton confirmation =
        QMessageBox::question(
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


// =====================================================
// BOUTON AFFICHER COURS
// =====================================================

void MainWindow::on_btnAfficherCours_clicked()
{
    afficherCours();
}


// =====================================================
// AFFICHER TABLE COURS
// =====================================================

void MainWindow::afficherCours()
{
    Cours cours;


    QSqlQueryModel *model =
        cours.afficher();


    if (model == nullptr)
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Impossible de charger les cours."
            );

        return;
    }


    ui->tableCours->setModel(model);


    ui->tableCours->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tableCours->setSelectionMode(
        QAbstractItemView::SingleSelection
        );


    ui->tableCours
        ->resizeColumnsToContents();


    ui->tableCours
        ->horizontalHeader()
        ->setStretchLastSection(true);
}


// =====================================================
// VIDER CHAMPS COURS
// =====================================================

void MainWindow::viderChampsCours()
{
    ui->editIdCours->clear();

    ui->editNomCours->clear();

    ui->editDescriptionCours->clear();

    ui->editDuree->clear();

    ui->editPrix->clear();


    if (ui->comboNiveau->count() > 0)
    {
        ui->comboNiveau->setCurrentIndex(0);
    }


    ui->dateEditDebut->setDate(
        QDate::currentDate()
        );


    if (ui->comboFormateur->count() > 0)
    {
        ui->comboFormateur->setCurrentIndex(0);
    }


    ui->editIdCours->setFocus();
}