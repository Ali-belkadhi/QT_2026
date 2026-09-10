#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "formateur.h"
#include "cours.h"

#include "statformateur.h"
#include "statcours.h"

#include "emailservice.h"
#include "utilisateur.h"


#include <QMessageBox>

#include <QIntValidator>

#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>

#include <QHeaderView>
#include <QAbstractItemView>
#include <QAbstractItemModel>

#include <QSortFilterProxyModel>

#include <QDate>
#include <QDateTime>

#include <QSizePolicy>
#include <QPixmap>
#include <QIcon>
#include <QBuffer>
#include <QFile>


// =====================================================
// PDF
// =====================================================

#include <QPdfWriter>
#include <QFileDialog>
#include <QPageSize>
#include <QPageLayout>
#include <QTextDocument>
#include <QDir>
#include <QMarginsF>


// =====================================================
// MODELE DE TRI PERSONNALISE
// =====================================================

class TriProxyModel : public QSortFilterProxyModel
{
public:

    explicit TriProxyModel(
        QObject *parent = nullptr
        )
        : QSortFilterProxyModel(parent)
    {
    }


protected:

    bool lessThan(
        const QModelIndex &left,
        const QModelIndex &right
        ) const override
    {
        QVariant gauche =
            sourceModel()->data(
                left,
                Qt::DisplayRole
                );


        QVariant droite =
            sourceModel()->data(
                right,
                Qt::DisplayRole
                );


        // =================================================
        // TRI DES DATES
        // =================================================

        QDate dateGauche =
            QDate::fromString(
                gauche.toString(),
                "dd/MM/yyyy"
                );


        QDate dateDroite =
            QDate::fromString(
                droite.toString(),
                "dd/MM/yyyy"
                );


        if (dateGauche.isValid() &&
            dateDroite.isValid())
        {
            return dateGauche < dateDroite;
        }


        // =================================================
        // TRI NUMERIQUE
        // =================================================

        QString texteGauche =
            gauche.toString().trimmed();

        QString texteDroite =
            droite.toString().trimmed();

        texteGauche = texteGauche.remove("TND").remove("DT").replace(
            ",",
            "."
            ).trimmed();

        texteDroite = texteDroite.remove("TND").remove("DT").replace(
            ",",
            "."
            ).trimmed();


        bool okGauche = false;

        bool okDroite = false;


        double nombreGauche =
            texteGauche.toDouble(
                &okGauche
                );


        double nombreDroite =
            texteDroite.toDouble(
                &okDroite
                );


        if (okGauche &&
            okDroite)
        {
            return nombreGauche <
                   nombreDroite;
        }


        // =================================================
        // TRI DES DUREES (HH:MM)
        // =================================================

        if (texteGauche.contains(":") &&
            texteDroite.contains(":"))
        {
            QStringList pG = texteGauche.split(":");
            QStringList pD = texteDroite.split(":");

            if (pG.size() == 2 && pD.size() == 2)
            {
                int minG = pG[0].toInt() * 60 + pG[1].toInt();
                int minD = pD[0].toInt() * 60 + pD[1].toInt();
                return minG < minD;
            }
        }


        // =================================================
        // TRI TEXTE
        // =================================================

        return QString::localeAwareCompare(
                   gauche
                       .toString()
                       .toLower(),

                   droite
                       .toString()
                       .toLower()
                   ) < 0;
    }
};


// =====================================================
// OUTILS PDF
// =====================================================

namespace
{

void marquerErreur(QWidget *widget)
{
    if (widget)
    {
        widget->setStyleSheet(
            "border: 2px solid #e53935;"
            "background-color: #fff5f5;"
            "color: black;"
            "border-radius: 5px;"
            "padding: 6px;"
            );
    }
}


QPixmap getLogoPixmap()
{
    QPixmap pix(":/logo.png");
    if (pix.isNull())
        pix.load(QCoreApplication::applicationDirPath() + "/logo.png");
    if (pix.isNull())
        pix.load(QCoreApplication::applicationDirPath() + "/../logo.png");
    if (pix.isNull())
        pix.load(QCoreApplication::applicationDirPath() + "/../../logo.png");
    if (pix.isNull())
        pix.load("logo.png");
    if (pix.isNull())
        pix.load("../logo.png");
    if (pix.isNull())
        pix.load("c:/Users/Mega-Pc/Desktop/projetc++/centre/logo.png");
    return pix;
}


QString getLogoBase64()
{
    QPixmap pix = getLogoPixmap();
    if (!pix.isNull())
    {
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pix.save(&buffer, "PNG");
        return QString::fromLatin1(bytes.toBase64());
    }
    return QString();
}


QString nomFichierValide(
    QString nom
    )
{
    nom.replace(
        QRegularExpression(
            R"([\\/:*?"<>|])"
            ),
        "_"
        );


    return nom;
}


// =====================================================

QString valeurHtml(
    const QString &texte
    )
{
    if (texte.trimmed().isEmpty())
    {
        return "-";
    }


    return texte.toHtmlEscaped();
}


bool enregistrerPdf(
    QWidget *parent,
    const QString &nomParDefaut,
    const QString &titrePdf,
    const QString &html
    )
{
    QString chemin =
        QFileDialog::getSaveFileName(
            parent,

            "Enregistrer le document PDF",

            QDir::homePath()
                + "/"
                + nomFichierValide(
                    nomParDefaut
                    )
                + ".pdf",

            "Document PDF (*.pdf)"
            );


    if (chemin.isEmpty())
    {
        return false;
    }


    if (!chemin.endsWith(
            ".pdf",
            Qt::CaseInsensitive))
    {
        chemin += ".pdf";
    }


    QPdfWriter pdf(
        chemin
        );


    pdf.setPageLayout(
        QPageLayout(
            QPageSize(
                QPageSize::A4
                ),

            QPageLayout::Portrait,

            QMarginsF(
                12,
                12,
                12,
                12
                ),

            QPageLayout::Millimeter
            )
        );


    pdf.setResolution(
        150
        );


    pdf.setTitle(
        titrePdf
        );


    pdf.setCreator(
        "SmartSkills"
        );


    QTextDocument document;
    QPixmap logoPix = getLogoPixmap();
    if (!logoPix.isNull())
    {
        document.addResource(QTextDocument::ImageResource, QUrl("logo.png"), logoPix);
        document.addResource(QTextDocument::ImageResource, QUrl(":/logo.png"), logoPix);
        document.addResource(QTextDocument::ImageResource, QUrl("qrc:/logo.png"), logoPix);
    }

    document.setHtml(
        html
        );

    document.print(
        &pdf
        );

    return true;
}

}


// CONSTRUCTEUR


MainWindow::MainWindow(
    QWidget *parent
    )
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // SERVICE EMAIL

    emailService = new EmailService(this);


    connect(
        emailService,
        &EmailService::emailEnvoye,
        this,
        &MainWindow::onEmailEnvoye
        );


    connect(
        emailService,
        &EmailService::erreurEmail,
        this,
        &MainWindow::onErreurEmail
        );


    setMinimumSize(
        1000,
        700
        );

    setWindowTitle("SmartSkills");


    // LOGO SMARTSKILLS


    QPixmap logoPix(":/logo.png");
    if (logoPix.isNull())
    {
        logoPix.load("logo.png");
    }
    if (logoPix.isNull())
    {
        logoPix.load("../logo.png");
    }

    if (!logoPix.isNull())
    {
        setWindowIcon(QIcon(logoPix));
        ui->labelLoginLogo->setPixmap(
            logoPix.scaled(260, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation)
            );
        ui->labelLoginLogo->setAlignment(Qt::AlignCenter);
    }

    // STYLE

    QString style =

        "QMainWindow {"
        " background-color: #172033;"
        "}"

        "QWidget {"
        " background-color: #172033;"
        " color: white;"
        "}"

        "QLabel {"
        " background-color: transparent;"
        " color: white;"
        "}"

        "QLineEdit,"
        "QDateEdit,"
        "QComboBox {"
        " background-color: white;"
        " color: black;"
        " border: 1px solid #b8c0cc;"
        " border-radius: 5px;"
        " padding: 6px;"
        "}"

        "QLineEdit:focus,"
        "QDateEdit:focus,"
        "QComboBox:focus {"
        " border: 2px solid #3498db;"
        "}"

        "QComboBox QAbstractItemView {"
        " background-color: white;"
        " color: black;"
        " selection-background-color: #3498db;"
        " selection-color: white;"
        "}"

        "QPushButton {"
        " background-color: white;"
        " color: black;"
        " border: 1px solid #c4cbd4;"
        " border-radius: 6px;"
        " padding: 7px;"
        "}"

        "QPushButton:hover {"
        " background-color: #eaf4ff;"
        " border: 1px solid #3498db;"
        "}"

        "QPushButton:pressed {"
        " background-color: #d8eaff;"
        "}"

        // Boutons de navigation : état actif
        "QPushButton#btnFormateurs[actif=\"true\"],"
        "QPushButton#btnCours[actif=\"true\"] {"
        " background-color: #1565c0;"
        " color: white;"
        " border: 2px solid #42a5f5;"
        " border-radius: 6px;"
        "}"

        "QPushButton#btnFormateurs[actif=\"true\"]:hover,"
        "QPushButton#btnCours[actif=\"true\"]:hover {"
        " background-color: #1976d2;"
        "}"

        "QTableView {"
        " background-color: white;"
        " alternate-background-color: #f4f6f8;"
        " color: black;"
        " gridline-color: #d0d5db;"
        " border: 1px solid #b8c0cc;"
        " selection-background-color: #3498db;"
        " selection-color: white;"
        "}"

        "QHeaderView::section {"
        " background-color: #f0f2f5;"
        " color: black;"
        " padding: 7px;"
        " border: 1px solid #d0d5db;"
        " font-weight: bold;"
        "}";


    setStyleSheet(
        style
        );



    // STYLE LOGIN CARD
    // Carte blanche centrée sur fond sombre


    // Fond de la pageLogin : identique au reste (#172033)
    // La carte loginCard reçoit un style blanc distinct

    ui->loginCard->setStyleSheet(

        // Carte blanche avec coins arrondis
        "QWidget#loginCard {"
        "  background-color: #ffffff;"
        "  border-radius: 16px;"
        "  border: 1px solid #e0e4ea;"
        "}"

        // Labels dans la carte : texte sombre
        "QWidget#loginCard QLabel {"
        "  background-color: transparent;"
        "  color: #1a2537;"
        "}"



        // Champs de saisie
        "QLineEdit#editLoginEmail,"
        "QLineEdit#editLoginPassword {"
        "  background-color: #f5f7fa;"
        "  color: #1a2537;"
        "  border: 1.5px solid #c8d0db;"
        "  border-radius: 8px;"
        "  padding: 8px 12px;"
        "  font-size: 13px;"
        "}"

        "QLineEdit#editLoginEmail:focus,"
        "QLineEdit#editLoginPassword:focus {"
        "  border: 2px solid #1565c0;"
        "  background-color: #eef4ff;"
        "}"

        // Checkbox
        "QCheckBox#checkAfficherPassword {"
        "  color: #546e8a;"
        "  background: transparent;"
        "  font-size: 12px;"
        "}"

        "QCheckBox#checkAfficherPassword::indicator {"
        "  width: 16px;"
        "  height: 16px;"
        "}"

        // Bouton Se connecter (bleu)
        "QPushButton#btnLogin {"
        "  background-color: #1565c0;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 10px;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "}"

        "QPushButton#btnLogin:hover {"
        "  background-color: #1976d2;"
        "}"

        "QPushButton#btnLogin:pressed {"
        "  background-color: #0d47a1;"
        "}"

        // Label erreur (rouge)
        "QLabel#labelLoginErreur {"
        "  color: #c62828;"
        "  background: transparent;"
        "  font-weight: bold;"
        "}"
        );





    // =================================================
    // TABLES : SELECTION LIGNE ENTIERE
    // =================================================

    ui->tableFormateurs->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );


    ui->tableCours->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );


    // =================================================
    // RECHERCHE
    // =================================================

    ui->editRechercheFormateur
        ->setPlaceholderText(
            "Rechercher : ID, nom, prénom, email, spécialité..."
            );


    ui->editRechercheCours
        ->setPlaceholderText(
            "Rechercher : ID, cours, niveau, prix, formateur..."
            );


    ui->editRechercheFormateur
        ->setClearButtonEnabled(
            true
            );


    ui->editRechercheCours
        ->setClearButtonEnabled(
            true
            );


    // =================================================
    // SPECIALITES
    // =================================================

    ui->comboSpecialite->clear();


    ui->comboSpecialite->addItem(
        "Informatique"
        );

    ui->comboSpecialite->addItem(
        "Mathématiques"
        );

    ui->comboSpecialite->addItem(
        "Français"
        );

    ui->comboSpecialite->addItem(
        "Anglais"
        );

    ui->comboSpecialite->addItem(
        "Physique"
        );

    ui->comboSpecialite->addItem(
        "Réseaux"
        );

    ui->comboSpecialite->addItem(
        "Base de données"
        );

    ui->comboSpecialite->addItem(
        "Développement Web"
        );

    ui->comboSpecialite->addItem(
        "Intelligence Artificielle"
        );

    ui->comboSpecialite->addItem(
        "Sécurité informatique"
        );


    // =================================================
    // NIVEAUX COURS
    // =================================================

    ui->comboNiveau->clear();


    ui->comboNiveau->addItem(
        "Débutant"
        );

    ui->comboNiveau->addItem(
        "Intermédiaire"
        );

    ui->comboNiveau->addItem(
        "Avancé"
        );


    // =================================================
    // DATES
    // =================================================

    ui->dateEditEmbauche
        ->setDate(
            QDate::currentDate()
            );


    ui->dateEditEmbauche
        ->setCalendarPopup(
            true
            );


    ui->dateEditDebut
        ->setDate(
            QDate::currentDate()
            );


    ui->dateEditDebut
        ->setCalendarPopup(
            true
            );


    // =================================================
    // CONTROLES SAISIE
    // =================================================

    configurerControlesSaisie();


    // =================================================
    // PAGE INITIALE : LOGIN
    // =================================================

    utilisateurConnecte = false;


    // Afficher la page de connexion au démarrage
    ui->stackedWidget
        ->setCurrentWidget(
            ui->pageLogin
            );


    // Initialiser le champ mot de passe
    ui->editLoginPassword
        ->setEchoMode(
            QLineEdit::Password
            );


    // Vider le message d'erreur
    ui->labelLoginErreur->clear();


    // Masquer la barre de navigation (Formateurs / Cours) sur la page de login
    ui->navWidget->setVisible(false);


    // NOTE : afficherFormateurs(), chargerFormateursCombo(), afficherCours()
    // ne sont PAS appelées ici — uniquement après connexion réussie.
}


// =====================================================
// DESTRUCTEUR
// =====================================================

MainWindow::~MainWindow()
{
    delete ui;
}


// =====================================================
// CALLBACKS EMAIL
// =====================================================

void MainWindow::onEmailEnvoye()
{
    qDebug()
        << "[MainWindow] ✅ Email de confirmation envoyé avec succès.";
}


void MainWindow::onErreurEmail(const QString &message)
{
    qWarning()
        << "[MainWindow] ⚠️  Email non envoyé :"
        << message;

    // On N'affiche PAS de QMessageBox à l'utilisateur :
    // l'ajout du formateur a déjà réussi — l'email est secondaire.
    // L'erreur est visible uniquement dans la console (qWarning).
}


// =====================================================
// BOUTON NAV ACTIF
// =====================================================

void MainWindow::activerBoutonNav(
    QPushButton *actif,
    QPushButton *inactif
    )
{
    // Style bouton ACTIF
    actif->setProperty(
        "actif",
        "true"
        );

    actif->setStyleSheet(
        "QPushButton {"
        "  background-color: #1565c0;"
        "  color: white;"
        "  border: 2px solid #42a5f5;"
        "  border-radius: 6px;"
        "  padding: 7px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1976d2;"
        "}"
        );


    // Style bouton INACTIF
    inactif->setProperty(
        "actif",
        "false"
        );

    inactif->setStyleSheet(
        "QPushButton {"
        "  background-color: #1e2d40;"
        "  color: #90a4ae;"
        "  border: 1px solid #2a3f58;"
        "  border-radius: 6px;"
        "  padding: 7px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #243550;"
        "  color: white;"
        "  border: 1px solid #3d5a80;"
        "}"
        );
}


// =====================================================
// GENERATION ID ALEATOIRE FORMATEUR
// =====================================================

int MainWindow::genererIdFormateur()
{
    // Génère un ID aléatoire entre 10000 et 99999
    // unique en base (max 100 tentatives)

    const int MIN_ID = 10000;
    const int MAX_ID = 99999;


    for (int tentative = 0;
         tentative < 100;
         tentative++)
    {
        int candidat =
            static_cast<int>(
                QRandomGenerator::global()
                    ->bounded(MIN_ID, MAX_ID + 1)
                );


        // Vérifier que cet ID n'existe pas déjà
        QSqlQuery check;

        check.prepare(
            "SELECT COUNT(*) "
            "FROM FORMATEUR "
            "WHERE ID_FORMATEUR = ?"
            );

        check.addBindValue(candidat);


        if (check.exec() &&
            check.next() &&
            check.value(0).toInt() == 0)
        {
            // ID libre → on l'utilise
            return candidat;
        }
    }


    // Fallback : MAX + 1
    QSqlQuery fallback;

    fallback.prepare(
        "SELECT NVL(MAX(ID_FORMATEUR), 0) + 1 "
        "FROM FORMATEUR"
        );


    if (fallback.exec() &&
        fallback.next())
    {
        return fallback.value(0).toInt();
    }


    return MIN_ID;
}


// =====================================================
// GENERATION ID ALEATOIRE COURS
// =====================================================

int MainWindow::genererIdCours()
{
    // Génère un ID aléatoire entre 10000 et 99999
    // unique en base (max 100 tentatives)

    const int MIN_ID = 10000;
    const int MAX_ID = 99999;


    for (int tentative = 0;
         tentative < 100;
         tentative++)
    {
        int candidat =
            static_cast<int>(
                QRandomGenerator::global()
                    ->bounded(MIN_ID, MAX_ID + 1)
                );


        // Vérifier que cet ID n'existe pas déjà
        QSqlQuery check;

        check.prepare(
            "SELECT COUNT(*) "
            "FROM COURS "
            "WHERE ID_COURS = ?"
            );

        check.addBindValue(candidat);


        if (check.exec() &&
            check.next() &&
            check.value(0).toInt() == 0)
        {
            // ID libre → on l'utilise
            return candidat;
        }
    }


    // Fallback : MAX + 1
    QSqlQuery fallback;

    fallback.prepare(
        "SELECT NVL(MAX(ID_COURS), 0) + 1 "
        "FROM COURS"
        );


    if (fallback.exec() &&
        fallback.next())
    {
        return fallback.value(0).toInt();
    }


    return MIN_ID;
}


// =====================================================
// CONTROLE DE SAISIE
// =====================================================

void MainWindow::configurerControlesSaisie()
{
    // =================================================
    // FORMATEUR ID
    // =================================================

    ui->editIdFormateur
        ->setValidator(
            new QIntValidator(
                1,
                999999999,
                this
                )
            );


    // =================================================
    // NOM / PRENOM
    // =================================================

    QRegularExpression regexNom(
        "^[A-Za-zÀ-ÿ -]{0,50}$"
        );


    ui->editNom
        ->setValidator(
            new QRegularExpressionValidator(
                regexNom,
                this
                )
            );


    ui->editPrenom
        ->setValidator(
            new QRegularExpressionValidator(
                regexNom,
                this
                )
            );


    // =================================================
    // TELEPHONE
    // =================================================

    QRegularExpression regexTelephone(
        "^[0-9]{0,20}$"
        );


    ui->editTelephone
        ->setValidator(
            new QRegularExpressionValidator(
                regexTelephone,
                this
                )
            );


    // =================================================
    // NOMBRES DECIMAUX
    // =================================================

    QRegularExpression regexNombre(
        "^[0-9]{0,8}([\\.,][0-9]{0,3})?$"
        );


    ui->editSalaire
        ->setValidator(
            new QRegularExpressionValidator(
                regexNombre,
                this
                )
            );


    ui->editPrix
        ->setValidator(
            new QRegularExpressionValidator(
                regexNombre,
                this
                )
            );


    // =================================================
    // LONGUEURS
    // =================================================

    ui->editNom
        ->setMaxLength(
            50
            );


    ui->editPrenom
        ->setMaxLength(
            50
            );


    ui->editEmail
        ->setMaxLength(
            100
            );


    ui->editTelephone
        ->setMaxLength(
            20
            );


    // =================================================
    // COURS
    // =================================================

    ui->editIdCours
        ->setValidator(
            new QIntValidator(
                1,
                999999999,
                this
                )
            );


    ui->editNomCours
        ->setMaxLength(
            100
            );


    ui->editDescriptionCours
        ->setMaxLength(
            500
            );


    QRegularExpression regexDuree(
        "^[0-9]{0,5}(:[0-9]{0,2})?$"
        );

    ui->editDuree
        ->setValidator(
            new QRegularExpressionValidator(
                regexDuree,
                this
                )
            );

    ui->editDuree
        ->setMaxLength(
            8
            );

    ui->editDuree
        ->setPlaceholderText(
            "Ex: 10:30 ou 55:00"
            );


    // =================================================
    // REINITIALISATION DU STYLE LORS DE LA SAISIE
    // =================================================

    auto resetOnTyping = [](QLineEdit *edit) {
        QObject::connect(edit, &QLineEdit::textChanged, [edit]() {
            edit->setStyleSheet("");
        });
    };

    resetOnTyping(ui->editNom);
    resetOnTyping(ui->editPrenom);
    resetOnTyping(ui->editEmail);
    resetOnTyping(ui->editTelephone);
    resetOnTyping(ui->editSalaire);

    resetOnTyping(ui->editNomCours);
    resetOnTyping(ui->editDescriptionCours);
    resetOnTyping(ui->editDuree);
    resetOnTyping(ui->editPrix);

    connect(ui->comboFormateur, &QComboBox::currentTextChanged, [this]() {
        ui->comboFormateur->setStyleSheet("");
    });
}


// =====================================================
// VALIDATION EMAIL
// =====================================================

bool MainWindow::validerEmail(const QString &email)
{
    QString clean = email.trimmed();

    if (clean.isEmpty()) {
        return false;
    }

    // 1. Rejeter les doubles points consécutifs (ex: ali..test@gmail.com ou ali@gmail..com)
    if (clean.contains("..")) {
        return false;
    }

    // 2. Rejeter si commence ou se termine par un point ou arobase
    if (clean.startsWith('.') || clean.endsWith('.') ||
        clean.startsWith('@') || clean.endsWith('@')) {
        return false;
    }

    // 3. Expression régulière stricte (local-part @ domain . TLD)
    static const QRegularExpression regexStrict(
        "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"
    );

    return regexStrict.match(clean).hasMatch();
}


// =====================================================
// VALIDATION FORMATEUR
// =====================================================

bool MainWindow::validerFormateur()
{
    ui->editNom->setStyleSheet("");
    ui->editPrenom->setStyleSheet("");
    ui->editEmail->setStyleSheet("");
    ui->editTelephone->setStyleSheet("");
    ui->editSalaire->setStyleSheet("");


    if (ui->editIdFormateur
            ->text()
            .trimmed()
            .isEmpty())
    {
        marquerErreur(ui->editIdFormateur);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "L'ID du formateur est obligatoire."
            );

        ui->editIdFormateur->setFocus();
        return false;
    }


    if (ui->editNom
            ->text()
            .trimmed()
            .length() < 2)
    {
        marquerErreur(ui->editNom);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "Le nom doit contenir au moins 2 caractères."
            );

        ui->editNom->setFocus();
        return false;
    }


    if (ui->editPrenom
            ->text()
            .trimmed()
            .length() < 2)
    {
        marquerErreur(ui->editPrenom);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "Le prénom doit contenir au moins 2 caractères."
            );

        ui->editPrenom->setFocus();
        return false;
    }


    QString email =
        ui->editEmail
            ->text()
            .trimmed();


    if (!validerEmail(email))
    {
        marquerErreur(ui->editEmail);

        QMessageBox::warning(
            this,
            "Validation",
            "Adresse email invalide. Veuillez vérifier l'adresse saisie."
            );

        ui->editEmail->setFocus();
        return false;
    }


    QString telephone =
        ui->editTelephone
            ->text()
            .trimmed();


    if (telephone.isEmpty() ||
        telephone.length() < 8)
    {
        marquerErreur(ui->editTelephone);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "Le téléphone doit contenir au moins 8 chiffres."
            );

        ui->editTelephone->setFocus();
        return false;
    }


    QString salaire =
        ui->editSalaire
            ->text()
            .trimmed();


    if (!salaire.isEmpty())
    {
        salaire.replace(
            ",",
            "."
            );


        bool ok = false;


        double valeur =
            salaire.toDouble(
                &ok
                );


        if (!ok ||
            valeur < 0)
        {
            marquerErreur(ui->editSalaire);

            QMessageBox::warning(
                this,
                "Contrôle de saisie",
                "Le salaire doit être un nombre positif."
                );

            ui->editSalaire->setFocus();
            return false;
        }
    }


    return true;
}


// =====================================================
// VALIDATION COURS
// =====================================================

bool MainWindow::validerCours()
{
    ui->editNomCours->setStyleSheet("");
    ui->editDescriptionCours->setStyleSheet("");
    ui->editDuree->setStyleSheet("");
    ui->editPrix->setStyleSheet("");
    ui->comboFormateur->setStyleSheet("");


    if (ui->editIdCours
            ->text()
            .trimmed()
            .isEmpty())
    {
        marquerErreur(ui->editIdCours);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "L'ID du cours est obligatoire."
            );

        ui->editIdCours->setFocus();
        return false;
    }


    if (ui->editNomCours
            ->text()
            .trimmed()
            .length() < 2)
    {
        marquerErreur(ui->editNomCours);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "Le nom du cours doit contenir au moins 2 caractères."
            );

        ui->editNomCours->setFocus();
        return false;
    }


    QString description =
        ui->editDescriptionCours
            ->text()
            .trimmed();

    if (description.length() < 5)
    {
        marquerErreur(ui->editDescriptionCours);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "La description doit contenir au moins 5 caractères."
            );

        ui->editDescriptionCours->setFocus();
        return false;
    }


    QString duree =
        ui->editDuree
            ->text()
            .trimmed();

    // Deux-points ':' obligatoires avec minutes valides (00-59)
    static const QRegularExpression regexDureeValide(
        "^[0-9]{1,5}:[0-5][0-9]$"
        );

    if (duree.isEmpty() || !regexDureeValide.match(duree).hasMatch())
    {
        marquerErreur(ui->editDuree);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "La durée est obligatoire et doit obligatoirement contenir les deux-points ':' (ex: 10:30 ou 55:00)."
            );

        ui->editDuree->setFocus();
        return false;
    }


    QString prix =
        ui->editPrix
            ->text()
            .trimmed();


    prix.replace(
        ",",
        "."
        );


    bool ok = false;


    double valeur =
        prix.toDouble(
            &ok
            );


    if (prix.isEmpty() ||
        !ok ||
        valeur < 0)
    {
        marquerErreur(ui->editPrix);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "Le prix est obligatoire et doit être un nombre positif."
            );

        ui->editPrix->setFocus();
        return false;
    }


    if (ui->comboFormateur
            ->currentIndex() < 0 ||
        ui->comboFormateur->currentData().toInt() <= 0)
    {
        marquerErreur(ui->comboFormateur);

        QMessageBox::warning(
            this,
            "Contrôle de saisie",
            "Veuillez sélectionner un formateur."
            );

        ui->comboFormateur->setFocus();
        return false;
    }


    return true;
}


// =====================================================
// INSTALLER MODELE FORMATEURS
// =====================================================

void MainWindow::installerModeleFormateurs(
    QSqlQueryModel *model
    )
{
    if (!model)
    {
        return;
    }


    QAbstractItemModel *ancien =
        ui->tableFormateurs
            ->model();


    TriProxyModel *proxy =
        new TriProxyModel(
            ui->tableFormateurs
            );


    model->setParent(
        proxy
        );


    proxy->setSourceModel(
        model
        );


    proxy->setDynamicSortFilter(
        true
        );


    proxy->setSortCaseSensitivity(
        Qt::CaseInsensitive
        );


    ui->tableFormateurs
        ->setModel(
            proxy
            );


    if (ancien &&
        ancien != proxy)
    {
        ancien->deleteLater();
    }


    ui->tableFormateurs
        ->setSelectionBehavior(
            QAbstractItemView::SelectRows
            );


    ui->tableFormateurs
        ->setSelectionMode(
            QAbstractItemView::SingleSelection
            );


    ui->tableFormateurs
        ->setEditTriggers(
            QAbstractItemView::NoEditTriggers
            );


    ui->tableFormateurs
        ->setAlternatingRowColors(
            true
            );


    ui->tableFormateurs
        ->verticalHeader()
        ->setVisible(
            false
            );


    ui->tableFormateurs
        ->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch
            );


    // =================================================
    // TRI
    // =================================================

    ui->tableFormateurs
        ->setSortingEnabled(
            true
            );


    ui->tableFormateurs
        ->horizontalHeader()
        ->setSectionsClickable(
            true
            );


    ui->tableFormateurs
        ->horizontalHeader()
        ->setSortIndicatorShown(
            true
            );


    ui->tableFormateurs
        ->sortByColumn(
            0,
            Qt::AscendingOrder
            );
}


// =====================================================
// INSTALLER MODELE COURS
// =====================================================

void MainWindow::installerModeleCours(
    QSqlQueryModel *model
    )
{
    if (!model)
    {
        return;
    }


    QAbstractItemModel *ancien =
        ui->tableCours
            ->model();


    TriProxyModel *proxy =
        new TriProxyModel(
            ui->tableCours
            );


    model->setParent(
        proxy
        );


    proxy->setSourceModel(
        model
        );


    proxy->setDynamicSortFilter(
        true
        );


    proxy->setSortCaseSensitivity(
        Qt::CaseInsensitive
        );


    ui->tableCours
        ->setModel(
            proxy
            );


    if (ancien &&
        ancien != proxy)
    {
        ancien->deleteLater();
    }


    // Colonne technique ID_FORMATEUR
    if (model->columnCount() > 8)
    {
        ui->tableCours
            ->hideColumn(
                8
                );
    }


    ui->tableCours
        ->setSelectionBehavior(
            QAbstractItemView::SelectRows
            );


    ui->tableCours
        ->setSelectionMode(
            QAbstractItemView::SingleSelection
            );


    ui->tableCours
        ->setEditTriggers(
            QAbstractItemView::NoEditTriggers
            );


    ui->tableCours
        ->setAlternatingRowColors(
            true
            );


    ui->tableCours
        ->verticalHeader()
        ->setVisible(
            false
            );


    ui->tableCours
        ->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch
            );


    // =================================================
    // TRI
    // =================================================

    ui->tableCours
        ->setSortingEnabled(
            true
            );


    ui->tableCours
        ->horizontalHeader()
        ->setSectionsClickable(
            true
            );


    ui->tableCours
        ->horizontalHeader()
        ->setSortIndicatorShown(
            true
            );


    ui->tableCours
        ->sortByColumn(
            0,
            Qt::AscendingOrder
            );
}


// =====================================================
// NAVIGATION FORMATEURS
// =====================================================

void MainWindow::on_btnFormateurs_clicked()
{
    // Sécurité : connexion obligatoire
    if (!utilisateurConnecte)
    {
        ui->stackedWidget
            ->setCurrentWidget(
                ui->pageLogin
                );

        return;
    }


    activerBoutonNav(
        ui->btnFormateurs,
        ui->btnCours
        );


    ui->stackedWidget
        ->setCurrentWidget(
            ui->pageFormateurs
            );


    afficherFormateurs();
}


// =====================================================
// NAVIGATION COURS
// =====================================================

void MainWindow::on_btnCours_clicked()
{
    // Sécurité : connexion obligatoire
    if (!utilisateurConnecte)
    {
        ui->stackedWidget
            ->setCurrentWidget(
                ui->pageLogin
                );

        return;
    }


    activerBoutonNav(
        ui->btnCours,
        ui->btnFormateurs
        );


    chargerFormateursCombo();


    ui->stackedWidget
        ->setCurrentWidget(
            ui->pageCours
            );


    afficherCours();


    // Générer l'ID cours automatiquement
    viderChampsCours();
}


// =====================================================
// AJOUTER FORMATEUR
// =====================================================

void MainWindow::on_btnAjouterFormateur_clicked()
{
    if (!validerFormateur())
    {
        return;
    }


    QString salaire =
        ui->editSalaire
            ->text();


    salaire.replace(
        ",",
        "."
        );


    Formateur formateur(

        ui->editIdFormateur
            ->text()
            .toInt(),

        ui->editNom
            ->text()
            .trimmed(),

        ui->editPrenom
            ->text()
            .trimmed(),

        ui->editEmail
            ->text()
            .trimmed(),

        ui->editTelephone
            ->text()
            .trimmed(),

        ui->comboSpecialite
            ->currentText(),

        salaire.isEmpty()
            ? 0
            : salaire.toDouble(),

        ui->dateEditEmbauche
            ->date()
        );


    if (formateur.ajouter())
    {
        QMessageBox::information(
            this,
            "Succès",
            "Formateur ajouté avec succès."
            );


        // =============================================
        // ENVOI EMAIL DE CONFIRMATION (asynchrone)
        // =============================================

        qDebug() << "[MainWindow] Destinataire email :" << formateur.getEmail();

        emailService->envoyerConfirmationFormateur(
            formateur.getPrenom(),
            formateur.getNom(),
            formateur.getEmail(),
            formateur.getSpecialite(),
            formateur.getDateEmbauche()
            );


        afficherFormateurs();


        chargerFormateursCombo();


        viderChampsFormateur();


        // =============================================
        // ACTUALISATION STATISTIQUES
        // =============================================

        if (fenetreStatFormateur)
        {
            fenetreStatFormateur
                ->actualiser();
        }
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
// SELECTION FORMATEUR
// =====================================================

void MainWindow::on_tableFormateurs_clicked(
    const QModelIndex &index
    )
{
    if (!index.isValid())
    {
        return;
    }


    int row =
        index.row();


    QAbstractItemModel *model =
        ui->tableFormateurs
            ->model();


    ui->editIdFormateur
        ->setText(
            model
                ->index(row, 0)
                .data()
                .toString()
            );


    ui->editNom
        ->setText(
            model
                ->index(row, 1)
                .data()
                .toString()
            );


    ui->editPrenom
        ->setText(
            model
                ->index(row, 2)
                .data()
                .toString()
            );


    ui->editEmail
        ->setText(
            model
                ->index(row, 3)
                .data()
                .toString()
            );


    ui->editTelephone
        ->setText(
            model
                ->index(row, 4)
                .data()
                .toString()
            );


    // =================================================
    // SPECIALITE
    // =================================================

    QString specialite =
        model
            ->index(row, 5)
            .data()
            .toString();


    int position =
        ui->comboSpecialite
            ->findText(
                specialite,
                Qt::MatchFixedString
                );


    if (position < 0 &&
        !specialite.isEmpty())
    {
        ui->comboSpecialite
            ->addItem(
                specialite
                );


        position =
            ui->comboSpecialite
                ->count() - 1;
    }


    if (position >= 0)
    {
        ui->comboSpecialite
            ->setCurrentIndex(
                position
                );
    }


    QString salaireTexte =
        model
            ->index(row, 6)
            .data()
            .toString();

    salaireTexte = salaireTexte.remove("TND").remove("DT").trimmed();

    ui->editSalaire
        ->setText(
            salaireTexte
            );


    // =================================================
    // DATE
    // =================================================

    QDate date =
        QDate::fromString(

            model
                ->index(row, 7)
                .data()
                .toString(),

            "dd/MM/yyyy"
            );


    if (date.isValid())
    {
        ui->dateEditEmbauche
            ->setDate(
                date
                );
    }


    ui->editIdFormateur
        ->setReadOnly(
            true
            );
}


// =====================================================
// MODIFIER FORMATEUR
// =====================================================

void MainWindow::on_btnModifierFormateur_clicked()
{
    QModelIndex index =
        ui->tableFormateurs
            ->currentIndex();


    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "Sélection",
            "Veuillez sélectionner un formateur."
            );

        return;
    }


    if (!validerFormateur())
    {
        return;
    }


    int id =
        ui->tableFormateurs
            ->model()
            ->index(
                index.row(),
                0
                )
            .data()
            .toInt();


    QString salaire =
        ui->editSalaire
            ->text();


    salaire.replace(
        ",",
        "."
        );


    Formateur formateur(

        id,

        ui->editNom
            ->text()
            .trimmed(),

        ui->editPrenom
            ->text()
            .trimmed(),

        ui->editEmail
            ->text()
            .trimmed(),

        ui->editTelephone
            ->text()
            .trimmed(),

        ui->comboSpecialite
            ->currentText(),

        salaire.isEmpty()
            ? 0
            : salaire.toDouble(),

        ui->dateEditEmbauche
            ->date()
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


        if (fenetreStatFormateur)
        {
            fenetreStatFormateur
                ->actualiser();
        }
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Modification impossible."
            );
    }
}


// =====================================================
// SUPPRIMER FORMATEUR
// =====================================================

void MainWindow::on_btnSupprimerFormateur_clicked()
{
    QModelIndex index =
        ui->tableFormateurs
            ->currentIndex();


    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "Sélection",
            "Veuillez sélectionner un formateur."
            );

        return;
    }


    int row =
        index.row();


    QAbstractItemModel *model =
        ui->tableFormateurs
            ->model();


    int id =
        model
            ->index(row, 0)
            .data()
            .toInt();


    QString nom =
        model
            ->index(row, 1)
            .data()
            .toString();


    QString prenom =
        model
            ->index(row, 2)
            .data()
            .toString();


    QMessageBox::StandardButton confirmation =
        QMessageBox::question(
            this,

            "Confirmation",

            "Voulez-vous supprimer "
                + nom
                + " "
                + prenom
                + " ?",

            QMessageBox::Yes |
                QMessageBox::No
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
            "Formateur supprimé."
            );


        afficherFormateurs();


        chargerFormateursCombo();


        viderChampsFormateur();


        if (fenetreStatFormateur)
        {
            fenetreStatFormateur
                ->actualiser();
        }
    }
    else
    {
        QMessageBox::critical(
            this,

            "Erreur",

            "Suppression impossible.\n"
            "Ce formateur peut être associé à un cours."
            );
    }
}


// =====================================================
// AFFICHER FORMATEURS
// =====================================================

void MainWindow::on_btnAfficherFormateurs_clicked()
{
    if (!ui->editRechercheFormateur
             ->text()
             .isEmpty())
    {
        ui->editRechercheFormateur
            ->clear();

        return;
    }


    afficherFormateurs();
}


// =====================================================

void MainWindow::afficherFormateurs()
{
    Formateur formateur;


    QSqlQueryModel *model =
        formateur.afficher();


    installerModeleFormateurs(
        model
        );
}


// =====================================================
// RECHERCHE FORMATEUR
// =====================================================

void MainWindow::on_editRechercheFormateur_textChanged(
    const QString &texte
    )
{
    if (texte.trimmed().isEmpty())
    {
        afficherFormateurs();

        return;
    }


    Formateur formateur;


    QSqlQueryModel *model =
        formateur.rechercher(
            texte
            );


    installerModeleFormateurs(
        model
        );
}


// =====================================================
// VIDER FORMATEUR
// =====================================================

void MainWindow::viderChampsFormateur()
{
    // Auto-génération de l'ID
    int nouvelId =
        genererIdFormateur();


    ui->editIdFormateur
        ->setText(
            QString::number(nouvelId)
            );


    // Champ ID en lecture seule (auto-géré)
    ui->editIdFormateur
        ->setReadOnly(
            true
            );


    ui->editIdFormateur
        ->setStyleSheet(
            "background-color: #e8f0fe;"
            "color: #1565c0;"
            "border: 1px solid #90caf9;"
            "border-radius: 5px;"
            "padding: 6px;"
            "font-weight: bold;"
            );


    ui->editNom
        ->clear();


    ui->editPrenom
        ->clear();


    ui->editEmail
        ->clear();


    ui->editTelephone
        ->clear();


    ui->editSalaire
        ->clear();


    if (ui->comboSpecialite
            ->count() > 0)
    {
        ui->comboSpecialite
            ->setCurrentIndex(
                0
                );
    }


    ui->dateEditEmbauche
        ->setDate(
            QDate::currentDate()
            );


    ui->tableFormateurs
        ->clearSelection();
}


// =====================================================
// CHARGER FORMATEURS COMBO
// =====================================================

void MainWindow::chargerFormateursCombo()
{
    ui->comboFormateur
        ->clear();


    Formateur formateur;


    QSqlQueryModel *model =
        formateur.listePourCombo();


    if (!model)
    {
        return;
    }


    for (int row = 0;
         row < model->rowCount();
         ++row)
    {
        int id =
            model
                ->data(
                    model->index(
                        row,
                        0
                        )
                    )
                .toInt();


        QString nom =
            model
                ->data(
                    model->index(
                        row,
                        1
                        )
                    )
                .toString();


        QString prenom =
            model
                ->data(
                    model->index(
                        row,
                        2
                        )
                    )
                .toString();


        ui->comboFormateur
            ->addItem(
                nom + " " + prenom,
                id
                );
    }


    delete model;
}


// =====================================================
// AJOUTER COURS
// =====================================================

void MainWindow::on_btnAjouterCours_clicked()
{
    if (!validerCours())
    {
        return;
    }


    QString prix =
        ui->editPrix
            ->text();


    prix.replace(
        ",",
        "."
        );


    Cours cours(

        ui->editIdCours
            ->text()
            .toInt(),

        ui->editNomCours
            ->text()
            .trimmed(),

        ui->editDescriptionCours
            ->text()
            .trimmed(),

        ui->comboNiveau
            ->currentText(),

        ui->editDuree
            ->text()
            .trimmed(),

        prix.toDouble(),

        ui->dateEditDebut
            ->date(),

        ui->comboFormateur
            ->currentData()
            .toInt()
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


        if (fenetreStatCours)
        {
            fenetreStatCours
                ->actualiser();
        }
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
// SELECTION COURS
// =====================================================

void MainWindow::on_tableCours_clicked(
    const QModelIndex &index
    )
{
    if (!index.isValid())
    {
        return;
    }


    int row =
        index.row();


    QAbstractItemModel *model =
        ui->tableCours
            ->model();


    ui->editIdCours
        ->setText(
            model
                ->index(row, 0)
                .data()
                .toString()
            );


    ui->editNomCours
        ->setText(
            model
                ->index(row, 1)
                .data()
                .toString()
            );


    ui->editDescriptionCours
        ->setText(
            model
                ->index(row, 2)
                .data()
                .toString()
            );


    // =================================================
    // NIVEAU
    // =================================================

    QString niveau =
        model
            ->index(row, 3)
            .data()
            .toString();


    int niveauIndex =
        ui->comboNiveau
            ->findText(
                niveau
                );


    if (niveauIndex >= 0)
    {
        ui->comboNiveau
            ->setCurrentIndex(
                niveauIndex
                );
    }


    ui->editDuree
        ->setText(
            model
                ->index(row, 4)
                .data()
                .toString()
            );


    QString prixTexte =
        model
            ->index(row, 5)
            .data()
            .toString();

    prixTexte = prixTexte.remove("TND").remove("DT").trimmed();

    ui->editPrix
        ->setText(
            prixTexte
            );


    // =================================================
    // DATE
    // =================================================

    QDate date =
        QDate::fromString(

            model
                ->index(row, 6)
                .data()
                .toString(),

            "dd/MM/yyyy"
            );


    if (date.isValid())
    {
        ui->dateEditDebut
            ->setDate(
                date
                );
    }


    // =================================================
    // FORMATEUR
    // =================================================

    if (model->columnCount() > 8)
    {
        int idFormateur =
            model
                ->index(row, 8)
                .data()
                .toInt();


        int position =
            ui->comboFormateur
                ->findData(
                    idFormateur
                    );


        if (position >= 0)
        {
            ui->comboFormateur
                ->setCurrentIndex(
                    position
                    );
        }
    }


    ui->editIdCours
        ->setReadOnly(
            true
            );
}


// =====================================================
// MODIFIER COURS
// =====================================================

void MainWindow::on_btnModifierCours_clicked()
{
    QModelIndex index =
        ui->tableCours
            ->currentIndex();


    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "Sélection",
            "Veuillez sélectionner un cours."
            );

        return;
    }


    if (!validerCours())
    {
        return;
    }


    int id =
        ui->tableCours
            ->model()
            ->index(
                index.row(),
                0
                )
            .data()
            .toInt();


    QString prix =
        ui->editPrix
            ->text();


    prix.replace(
        ",",
        "."
        );


    Cours cours(

        id,

        ui->editNomCours
            ->text()
            .trimmed(),

        ui->editDescriptionCours
            ->text()
            .trimmed(),

        ui->comboNiveau
            ->currentText(),

        ui->editDuree
            ->text()
            .trimmed(),

        prix.toDouble(),

        ui->dateEditDebut
            ->date(),

        ui->comboFormateur
            ->currentData()
            .toInt()
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


        if (fenetreStatCours)
        {
            fenetreStatCours
                ->actualiser();
        }
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Modification impossible."
            );
    }
}


// =====================================================
// SUPPRIMER COURS
// =====================================================

void MainWindow::on_btnSupprimerCours_clicked()
{
    QModelIndex index =
        ui->tableCours
            ->currentIndex();


    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "Sélection",
            "Veuillez sélectionner un cours."
            );

        return;
    }


    int row =
        index.row();


    QAbstractItemModel *model =
        ui->tableCours
            ->model();


    int id =
        model
            ->index(row, 0)
            .data()
            .toInt();


    QString nom =
        model
            ->index(row, 1)
            .data()
            .toString();


    if (QMessageBox::question(
            this,

            "Confirmation",

            "Voulez-vous supprimer le cours "
                + nom
                + " ?",

            QMessageBox::Yes |
                QMessageBox::No
            ) != QMessageBox::Yes)
    {
        return;
    }


    Cours cours;


    if (cours.supprimer(id))
    {
        QMessageBox::information(
            this,
            "Succès",
            "Cours supprimé."
            );


        afficherCours();


        viderChampsCours();


        if (fenetreStatCours)
        {
            fenetreStatCours
                ->actualiser();
        }
    }
    else
    {
        QMessageBox::critical(
            this,
            "Erreur",
            "Suppression impossible."
            );
    }
}


// =====================================================
// AFFICHER COURS
// =====================================================

void MainWindow::on_btnAfficherCours_clicked()
{
    if (!ui->editRechercheCours
             ->text()
             .isEmpty())
    {
        ui->editRechercheCours
            ->clear();

        return;
    }


    afficherCours();
}


// =====================================================

void MainWindow::afficherCours()
{
    Cours cours;


    QSqlQueryModel *model =
        cours.afficher();


    installerModeleCours(
        model
        );
}


// =====================================================
// RECHERCHE COURS
// =====================================================

void MainWindow::on_editRechercheCours_textChanged(
    const QString &texte
    )
{
    if (texte.trimmed().isEmpty())
    {
        afficherCours();

        return;
    }


    Cours cours;


    QSqlQueryModel *model =
        cours.rechercher(
            texte
            );


    installerModeleCours(
        model
        );
}


// =====================================================
// VIDER COURS
// =====================================================

void MainWindow::viderChampsCours()
{
    // Auto-génération de l'ID
    int nouvelId =
        genererIdCours();


    ui->editIdCours
        ->setText(
            QString::number(nouvelId)
            );


    // Champ ID en lecture seule (auto-géré)
    ui->editIdCours
        ->setReadOnly(
            true
            );


    ui->editIdCours
        ->setStyleSheet(
            "background-color: #e8f0fe;"
            "color: #1565c0;"
            "border: 1px solid #90caf9;"
            "border-radius: 5px;"
            "padding: 6px;"
            "font-weight: bold;"
            );


    ui->editNomCours
        ->clear();


    ui->editDescriptionCours
        ->clear();


    ui->editDuree
        ->clear();


    ui->editPrix
        ->clear();


    if (ui->comboNiveau
            ->count() > 0)
    {
        ui->comboNiveau
            ->setCurrentIndex(
                0
                );
    }


    ui->dateEditDebut
        ->setDate(
            QDate::currentDate()
            );


    if (ui->comboFormateur
            ->count() > 0)
    {
        ui->comboFormateur
            ->setCurrentIndex(
                0
                );
    }


    ui->tableCours
        ->clearSelection();
}


// =====================================================
// PDF FORMATEUR
// =====================================================

void MainWindow::on_btnPdfFormateur_clicked()
{
    QModelIndex index =
        ui->tableFormateurs
            ->currentIndex();


    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "PDF",
            "Veuillez sélectionner un formateur."
            );

        return;
    }


    int row =
        index.row();


    QAbstractItemModel *model =
        ui->tableFormateurs
            ->model();


    QString id =
        model->index(row, 0)
            .data()
            .toString();


    QString nom =
        model->index(row, 1)
            .data()
            .toString();


    QString prenom =
        model->index(row, 2)
            .data()
            .toString();


    QString email =
        model->index(row, 3)
            .data()
            .toString();


    QString telephone =
        model->index(row, 4)
            .data()
            .toString();


    QString specialite =
        model->index(row, 5)
            .data()
            .toString();


    QString salaire =
        model->index(row, 6)
            .data()
            .toString();


    QString date =
        model->index(row, 7)
            .data()
            .toString();


    QString generation =
        QDateTime::currentDateTime()
            .toString(
                "dd/MM/yyyy à HH:mm"
                );

    QString logoB64 = getLogoBase64();
    QString logoHtml;
    if (!logoB64.isEmpty())
    {
        logoHtml = QString("<div style=\"text-align:center; margin-bottom: 10px;\"><img src=\"data:image/png;base64,%1\" height=\"60\" /></div>").arg(logoB64);
    }
    else
    {
        logoHtml = "<div style=\"text-align:center; margin-bottom: 10px;\"><img src=\"logo.png\" height=\"60\" /></div>";
    }

    QString salaireAffiche = salaire.trimmed();
    while (salaireAffiche.endsWith(" TND", Qt::CaseInsensitive) || salaireAffiche.endsWith("TND", Qt::CaseInsensitive) || salaireAffiche.endsWith(" DT", Qt::CaseInsensitive) || salaireAffiche.endsWith("DT", Qt::CaseInsensitive))
    {
        if (salaireAffiche.endsWith(" TND", Qt::CaseInsensitive)) salaireAffiche.chop(4);
        else if (salaireAffiche.endsWith("TND", Qt::CaseInsensitive)) salaireAffiche.chop(3);
        else if (salaireAffiche.endsWith(" DT", Qt::CaseInsensitive)) salaireAffiche.chop(3);
        else if (salaireAffiche.endsWith("DT", Qt::CaseInsensitive)) salaireAffiche.chop(2);
        salaireAffiche = salaireAffiche.trimmed();
    }
    salaireAffiche += " TND";


    QString html =
        QString(
            R"(

<html>

<head>

<meta charset="utf-8">

<style>

body
{
    font-family: Arial;
    color: #1f2937;
    font-size: 11pt;
}

.header
{
    background-color: #172033;
    color: white;
    text-align: center;
    padding: 22px;
}

.header h1
{
    font-size: 22pt;
    margin: 5px 0 0 0;
    letter-spacing: 2px;
}

.header h2
{
    font-size: 13pt;
    margin: 6px 0 0 0;
    color: #90caf9;
    font-weight: normal;
}

.meta
{
    color: #6b7280;
    margin-top: 20px;
}

.nom
{
    text-align: center;
    color: #172033;
    font-size: 20pt;
    font-weight: bold;
    margin: 25px;
}

table
{
    width: 100%;
    border-collapse: collapse;
}

td
{
    padding: 11px;
    border-bottom: 1px solid #d1d5db;
}

.label
{
    width: 32%;
    font-weight: bold;
    color: #172033;
}

.info
{
    margin-top: 30px;
    background-color: #f3f4f6;
    border: 1px solid #d1d5db;
    padding: 18px;
}

.signature
{
    margin-top: 60px;
    text-align: right;
}

.footer
{
    margin-top: 70px;
    text-align: center;
    color: #6b7280;
    font-size: 9pt;
}

</style>

</head>

<body>

<div class="header">

%10

<h1>
SMARTSKILLS
</h1>

<h2>
FICHE DU FORMATEUR
</h2>

</div>


<div class="meta">

Document généré le : %1

<br>

Référence : FORM-%2

</div>


<div class="nom">

%3 %4

</div>


<table>

<tr>
<td class="label">ID Formateur</td>
<td>%2</td>
</tr>

<tr>
<td class="label">Nom</td>
<td>%4</td>
</tr>

<tr>
<td class="label">Prénom</td>
<td>%3</td>
</tr>

<tr>
<td class="label">Email</td>
<td>%5</td>
</tr>

<tr>
<td class="label">Téléphone</td>
<td>%6</td>
</tr>

<tr>
<td class="label">Spécialité</td>
<td>%7</td>
</tr>

<tr>
<td class="label">Salaire</td>
<td>%8</td>
</tr>

<tr>
<td class="label">Date d'embauche</td>
<td>%9</td>
</tr>

</table>


<div class="info">

<b>
Informations administratives
</b>

<br><br>

Cette fiche personnalisée a été générée automatiquement
par l'application SmartSkills.

</div>


<div class="signature">

<b>
Signature de l'administration
</b>

<br><br><br>

____________________________

</div>


<div class="footer">

SmartSkills • Document administratif

</div>

</body>

</html>

)"
            )
            .arg(
                valeurHtml(generation),
                valeurHtml(id),
                valeurHtml(prenom.toUpper()),
                valeurHtml(nom.toUpper()),
                valeurHtml(email),
                valeurHtml(telephone),
                valeurHtml(specialite),
                valeurHtml(salaireAffiche),
                valeurHtml(date),
                logoHtml
                );


    if (enregistrerPdf(
            this,

            "Formateur_"
                + nom
                + "_"
                + prenom,

            "Fiche Formateur - "
                + nom
                + " "
                + prenom,

            html
            ))
    {
        QMessageBox::information(
            this,
            "PDF",
            "Le PDF du formateur a été créé avec succès."
            );
    }
}


// =====================================================
// PDF COURS
// =====================================================

void MainWindow::on_btnPdfCours_clicked()
{
    QModelIndex index =
        ui->tableCours
            ->currentIndex();


    if (!index.isValid())
    {
        QMessageBox::warning(
            this,
            "PDF",
            "Veuillez sélectionner un cours."
            );

        return;
    }


    int row =
        index.row();


    QAbstractItemModel *model =
        ui->tableCours
            ->model();


    QString id =
        model->index(row, 0)
            .data()
            .toString();


    QString nom =
        model->index(row, 1)
            .data()
            .toString();


    QString description =
        model->index(row, 2)
            .data()
            .toString();


    QString niveau =
        model->index(row, 3)
            .data()
            .toString();


    QString duree =
        model->index(row, 4)
            .data()
            .toString();


    QString prix =
        model->index(row, 5)
            .data()
            .toString();


    QString date =
        model->index(row, 6)
            .data()
            .toString();


    QString formateur =
        model->index(row, 7)
            .data()
            .toString();


    QString generation =
        QDateTime::currentDateTime()
            .toString(
                "dd/MM/yyyy à HH:mm"
                );

    QString logoB64 = getLogoBase64();
    QString logoHtml;
    if (!logoB64.isEmpty())
    {
        logoHtml = QString("<div style=\"text-align:center; margin-bottom: 10px;\"><img src=\"data:image/png;base64,%1\" height=\"60\" /></div>").arg(logoB64);
    }
    else
    {
        logoHtml = "<div style=\"text-align:center; margin-bottom: 10px;\"><img src=\"logo.png\" height=\"60\" /></div>";
    }

    QString prixAffiche = prix.trimmed();
    while (prixAffiche.endsWith(" TND", Qt::CaseInsensitive) || prixAffiche.endsWith("TND", Qt::CaseInsensitive) || prixAffiche.endsWith(" DT", Qt::CaseInsensitive) || prixAffiche.endsWith("DT", Qt::CaseInsensitive))
    {
        if (prixAffiche.endsWith(" TND", Qt::CaseInsensitive)) prixAffiche.chop(4);
        else if (prixAffiche.endsWith("TND", Qt::CaseInsensitive)) prixAffiche.chop(3);
        else if (prixAffiche.endsWith(" DT", Qt::CaseInsensitive)) prixAffiche.chop(3);
        else if (prixAffiche.endsWith("DT", Qt::CaseInsensitive)) prixAffiche.chop(2);
        prixAffiche = prixAffiche.trimmed();
    }
    prixAffiche += " TND";


    QString html =
        QString(
            R"(

<html>

<head>

<meta charset="utf-8">

<style>

body
{
    font-family: Arial;
    color: #1f2937;
    font-size: 11pt;
}

.header
{
    background-color: #172033;
    color: white;
    text-align: center;
    padding: 22px;
}

.header h1
{
    font-size: 22pt;
    margin: 5px 0 0 0;
    letter-spacing: 2px;
}

.header h2
{
    font-size: 13pt;
    margin: 6px 0 0 0;
    color: #90caf9;
    font-weight: normal;
}

.meta
{
    color: #6b7280;
    margin-top: 20px;
}

.nom
{
    text-align: center;
    color: #172033;
    font-size: 20pt;
    font-weight: bold;
    margin: 25px;
}

table
{
    width: 100%;
    border-collapse: collapse;
}

td
{
    padding: 11px;
    border-bottom: 1px solid #d1d5db;
}

.label
{
    width: 32%;
    color: #172033;
    font-weight: bold;
}

.description
{
    margin-top: 30px;
    background-color: #f3f4f6;
    border: 1px solid #d1d5db;
    padding: 18px;
}

.signature
{
    margin-top: 60px;
    text-align: right;
}

.footer
{
    margin-top: 70px;
    text-align: center;
    color: #6b7280;
    font-size: 9pt;
}

</style>

</head>


<body>


<div class="header">

%10

<h1>
SMARTSKILLS
</h1>

<h2>
FICHE PÉDAGOGIQUE DU COURS
</h2>

</div>


<div class="meta">

Document généré le : %1

<br>

Référence : CRS-%2

</div>


<div class="nom">

%3

</div>


<table>

<tr>
<td class="label">ID Cours</td>
<td>%2</td>
</tr>

<tr>
<td class="label">Nom du cours</td>
<td>%3</td>
</tr>

<tr>
<td class="label">Niveau</td>
<td>%4</td>
</tr>

<tr>
<td class="label">Durée</td>
<td>%5</td>
</tr>

<tr>
<td class="label">Prix</td>
<td>%6</td>
</tr>

<tr>
<td class="label">Date de début</td>
<td>%7</td>
</tr>

<tr>
<td class="label">Formateur</td>
<td>%8</td>
</tr>

</table>


<div class="description">

<b>
Description du cours
</b>

<br><br>

%9

</div>


<div class="signature">

<b>
Signature de l'administration
</b>

<br><br><br>

____________________________

</div>


<div class="footer">

SmartSkills • Fiche pédagogique personnalisée

</div>


</body>

</html>

)"
            )
            .arg(
                valeurHtml(generation),
                valeurHtml(id),
                valeurHtml(nom.toUpper()),
                valeurHtml(niveau),
                valeurHtml(duree),
                valeurHtml(prixAffiche),
                valeurHtml(date),
                valeurHtml(formateur),
                valeurHtml(description),
                logoHtml
                );


    if (enregistrerPdf(
            this,

            "Cours_" + nom,

            "Fiche Cours - " + nom,

            html
            ))
    {
        QMessageBox::information(
            this,
            "PDF",
            "Le PDF du cours a été créé avec succès."
            );
    }
}


// =====================================================
// STATISTIQUES FORMATEUR
// =====================================================

void MainWindow::on_btnStatFormateur_clicked()
{
    if (!fenetreStatFormateur)
    {
        fenetreStatFormateur =
            new StatFormateur(
                this
                );
    }


    // Recharge les valeurs depuis Oracle
    fenetreStatFormateur
        ->actualiser();


    fenetreStatFormateur
        ->show();


    fenetreStatFormateur
        ->raise();


    fenetreStatFormateur
        ->activateWindow();
}


// =====================================================
// STATISTIQUES COURS
// =====================================================

void MainWindow::on_btnStatCours_clicked()
{
    if (!fenetreStatCours)
    {
        fenetreStatCours =
            new StatCours(
                this
                );
    }


    // Recharge les valeurs depuis Oracle
    fenetreStatCours
        ->actualiser();


    fenetreStatCours
        ->show();


    fenetreStatCours
        ->raise();


    fenetreStatCours
        ->activateWindow();
}


// =====================================================
// SLOT : BOUTON LOGIN
// =====================================================

void MainWindow::on_btnLogin_clicked()
{
    // -------------------------------------------------
    // 1. Récupérer les champs
    // -------------------------------------------------

    QString email =
        ui->editLoginEmail
            ->text()
            .trimmed();


    QString password =
        ui->editLoginPassword
            ->text();


    // -------------------------------------------------
    // 2. Validation : champs vides
    // -------------------------------------------------

    if (email.isEmpty() ||
        password.isEmpty())
    {
        ui->labelLoginErreur
            ->setText(
                "Veuillez saisir votre email et votre mot de passe."
                );

        return;
    }


    // -------------------------------------------------
    // 3. Authentification
    // -------------------------------------------------

    Utilisateur utilisateur;


    bool ok = utilisateur.authentifier(
        email,
        password
        );


    // -------------------------------------------------
    // 4. Succès
    // -------------------------------------------------

    if (ok)
    {
        utilisateurConnecte = true;


        // Vider le message d'erreur
        ui->labelLoginErreur->clear();


        // Vider le champ mot de passe
        ui->editLoginPassword->clear();


        // Afficher la barre de navigation (Formateurs / Cours)
        ui->navWidget->setVisible(true);


        // Charger les données
        afficherFormateurs();
        viderChampsFormateur();
        chargerFormateursCombo();


        // Bouton Formateurs actif
        activerBoutonNav(
            ui->btnFormateurs,
            ui->btnCours
            );


        // Aller sur la page Formateurs
        ui->stackedWidget
            ->setCurrentWidget(
                ui->pageFormateurs
                );


        QMessageBox::information(
            this,
            "Connexion",
            "Connexion réussie."
            );
    }


    // -------------------------------------------------
    // 5. Échec
    // -------------------------------------------------

    else
    {
        utilisateurConnecte = false;


        ui->labelLoginErreur
            ->setText(
                "Email ou mot de passe incorrect."
                );


        ui->editLoginPassword->clear();


        ui->editLoginPassword->setFocus();
    }
}


// =====================================================
// SLOT : AFFICHER / MASQUER MOT DE PASSE
// =====================================================

void MainWindow::on_checkAfficherPassword_toggled(
    bool checked
    )
{
    if (checked)
    {
        ui->editLoginPassword
            ->setEchoMode(
                QLineEdit::Normal
                );
    }
    else
    {
        ui->editLoginPassword
            ->setEchoMode(
                QLineEdit::Password
                );
    }
}




