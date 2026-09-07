/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *navLayout;
    QPushButton *btnFormateurs;
    QPushButton *btnCours;
    QStackedWidget *stackedWidget;
    QWidget *pageCours;
    QVBoxLayout *coursOuterLayout;
    QLabel *label_12;
    QGridLayout *coursFormLayout;
    QLabel *label_7;
    QLineEdit *editIdCours;
    QLabel *label_17;
    QComboBox *comboNiveau;
    QPushButton *btnAjouterCours;
    QLabel *label_8;
    QLineEdit *editNomCours;
    QLabel *label_9;
    QLineEdit *editDuree;
    QPushButton *btnModifierCours;
    QLabel *label_15;
    QLineEdit *editDescriptionCours;
    QLabel *label_10;
    QLineEdit *editPrix;
    QPushButton *btnSupprimerCours;
    QLabel *label_16;
    QDateEdit *dateEditDebut;
    QLabel *label_11;
    QComboBox *comboFormateur;
    QPushButton *btnAfficherCours;
    QHBoxLayout *coursToolbarLayout;
    QPushButton *btnPdfCours;
    QPushButton *btnStatCours;
    QSpacerItem *spacerItem;
    QPushButton *btnRechercherCours;
    QLineEdit *editRechercheCours;
    QTableView *tableCours;
    QWidget *pageFormateurs;
    QVBoxLayout *formOuterLayout;
    QLabel *labelTitreFormateurs;
    QGridLayout *formFormLayout;
    QLabel *label;
    QLineEdit *editIdFormateur;
    QLabel *label_6;
    QLineEdit *editEmail;
    QPushButton *btnAjouterFormateur;
    QLabel *label_2;
    QLineEdit *editNom;
    QLabel *label_5;
    QLineEdit *editTelephone;
    QPushButton *btnModifierFormateur;
    QLabel *label_3;
    QLineEdit *editPrenom;
    QLabel *label_4;
    QComboBox *comboSpecialite;
    QPushButton *btnSupprimerFormateur;
    QLabel *label_13;
    QLineEdit *editSalaire;
    QLabel *label_14;
    QDateEdit *dateEditEmbauche;
    QPushButton *btnAfficherFormateurs;
    QHBoxLayout *formToolbarLayout;
    QPushButton *btnPdfFormateur;
    QPushButton *btnStatFormateur;
    QSpacerItem *spacerItem1;
    QPushButton *btnRechercherFormateur;
    QLineEdit *editRechercheFormateur;
    QTableView *tableFormateurs;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1086, 715);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setSpacing(6);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(10, 8, 10, 8);
        navLayout = new QHBoxLayout();
        navLayout->setSpacing(6);
        navLayout->setObjectName("navLayout");
        btnFormateurs = new QPushButton(centralwidget);
        btnFormateurs->setObjectName("btnFormateurs");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnFormateurs->sizePolicy().hasHeightForWidth());
        btnFormateurs->setSizePolicy(sizePolicy);
        btnFormateurs->setMinimumSize(QSize(0, 48));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        btnFormateurs->setFont(font);

        navLayout->addWidget(btnFormateurs);

        btnCours = new QPushButton(centralwidget);
        btnCours->setObjectName("btnCours");
        sizePolicy.setHeightForWidth(btnCours->sizePolicy().hasHeightForWidth());
        btnCours->setSizePolicy(sizePolicy);
        btnCours->setMinimumSize(QSize(0, 48));
        btnCours->setFont(font);

        navLayout->addWidget(btnCours);


        mainLayout->addLayout(navLayout);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy1);
        pageCours = new QWidget();
        pageCours->setObjectName("pageCours");
        coursOuterLayout = new QVBoxLayout(pageCours);
        coursOuterLayout->setSpacing(8);
        coursOuterLayout->setObjectName("coursOuterLayout");
        coursOuterLayout->setContentsMargins(6, 10, 6, 6);
        label_12 = new QLabel(pageCours);
        label_12->setObjectName("label_12");
        QFont font1;
        font1.setBold(true);
        font1.setUnderline(true);
        label_12->setFont(font1);
        label_12->setAlignment(Qt::AlignCenter);

        coursOuterLayout->addWidget(label_12);

        coursFormLayout = new QGridLayout();
        coursFormLayout->setSpacing(8);
        coursFormLayout->setObjectName("coursFormLayout");
        label_7 = new QLabel(pageCours);
        label_7->setObjectName("label_7");

        coursFormLayout->addWidget(label_7, 0, 0, 1, 1);

        editIdCours = new QLineEdit(pageCours);
        editIdCours->setObjectName("editIdCours");

        coursFormLayout->addWidget(editIdCours, 0, 1, 1, 1);

        label_17 = new QLabel(pageCours);
        label_17->setObjectName("label_17");

        coursFormLayout->addWidget(label_17, 0, 2, 1, 1);

        comboNiveau = new QComboBox(pageCours);
        comboNiveau->setObjectName("comboNiveau");
        sizePolicy.setHeightForWidth(comboNiveau->sizePolicy().hasHeightForWidth());
        comboNiveau->setSizePolicy(sizePolicy);

        coursFormLayout->addWidget(comboNiveau, 0, 3, 1, 1);

        btnAjouterCours = new QPushButton(pageCours);
        btnAjouterCours->setObjectName("btnAjouterCours");
        btnAjouterCours->setMinimumSize(QSize(90, 30));

        coursFormLayout->addWidget(btnAjouterCours, 0, 4, 1, 1);

        label_8 = new QLabel(pageCours);
        label_8->setObjectName("label_8");

        coursFormLayout->addWidget(label_8, 1, 0, 1, 1);

        editNomCours = new QLineEdit(pageCours);
        editNomCours->setObjectName("editNomCours");

        coursFormLayout->addWidget(editNomCours, 1, 1, 1, 1);

        label_9 = new QLabel(pageCours);
        label_9->setObjectName("label_9");

        coursFormLayout->addWidget(label_9, 1, 2, 1, 1);

        editDuree = new QLineEdit(pageCours);
        editDuree->setObjectName("editDuree");

        coursFormLayout->addWidget(editDuree, 1, 3, 1, 1);

        btnModifierCours = new QPushButton(pageCours);
        btnModifierCours->setObjectName("btnModifierCours");
        btnModifierCours->setMinimumSize(QSize(90, 30));

        coursFormLayout->addWidget(btnModifierCours, 1, 4, 1, 1);

        label_15 = new QLabel(pageCours);
        label_15->setObjectName("label_15");

        coursFormLayout->addWidget(label_15, 2, 0, 1, 1);

        editDescriptionCours = new QLineEdit(pageCours);
        editDescriptionCours->setObjectName("editDescriptionCours");

        coursFormLayout->addWidget(editDescriptionCours, 2, 1, 1, 1);

        label_10 = new QLabel(pageCours);
        label_10->setObjectName("label_10");

        coursFormLayout->addWidget(label_10, 2, 2, 1, 1);

        editPrix = new QLineEdit(pageCours);
        editPrix->setObjectName("editPrix");

        coursFormLayout->addWidget(editPrix, 2, 3, 1, 1);

        btnSupprimerCours = new QPushButton(pageCours);
        btnSupprimerCours->setObjectName("btnSupprimerCours");
        btnSupprimerCours->setMinimumSize(QSize(90, 30));

        coursFormLayout->addWidget(btnSupprimerCours, 2, 4, 1, 1);

        label_16 = new QLabel(pageCours);
        label_16->setObjectName("label_16");

        coursFormLayout->addWidget(label_16, 3, 0, 1, 1);

        dateEditDebut = new QDateEdit(pageCours);
        dateEditDebut->setObjectName("dateEditDebut");
        sizePolicy.setHeightForWidth(dateEditDebut->sizePolicy().hasHeightForWidth());
        dateEditDebut->setSizePolicy(sizePolicy);

        coursFormLayout->addWidget(dateEditDebut, 3, 1, 1, 1);

        label_11 = new QLabel(pageCours);
        label_11->setObjectName("label_11");

        coursFormLayout->addWidget(label_11, 3, 2, 1, 1);

        comboFormateur = new QComboBox(pageCours);
        comboFormateur->setObjectName("comboFormateur");
        sizePolicy.setHeightForWidth(comboFormateur->sizePolicy().hasHeightForWidth());
        comboFormateur->setSizePolicy(sizePolicy);

        coursFormLayout->addWidget(comboFormateur, 3, 3, 1, 1);

        btnAfficherCours = new QPushButton(pageCours);
        btnAfficherCours->setObjectName("btnAfficherCours");
        btnAfficherCours->setMinimumSize(QSize(90, 30));

        coursFormLayout->addWidget(btnAfficherCours, 3, 4, 1, 1);


        coursOuterLayout->addLayout(coursFormLayout);

        coursToolbarLayout = new QHBoxLayout();
        coursToolbarLayout->setSpacing(8);
        coursToolbarLayout->setObjectName("coursToolbarLayout");
        btnPdfCours = new QPushButton(pageCours);
        btnPdfCours->setObjectName("btnPdfCours");
        btnPdfCours->setMinimumSize(QSize(90, 30));

        coursToolbarLayout->addWidget(btnPdfCours);

        btnStatCours = new QPushButton(pageCours);
        btnStatCours->setObjectName("btnStatCours");
        btnStatCours->setMinimumSize(QSize(140, 30));

        coursToolbarLayout->addWidget(btnStatCours);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        coursToolbarLayout->addItem(spacerItem);

        btnRechercherCours = new QPushButton(pageCours);
        btnRechercherCours->setObjectName("btnRechercherCours");
        btnRechercherCours->setMinimumSize(QSize(110, 30));

        coursToolbarLayout->addWidget(btnRechercherCours);

        editRechercheCours = new QLineEdit(pageCours);
        editRechercheCours->setObjectName("editRechercheCours");
        sizePolicy.setHeightForWidth(editRechercheCours->sizePolicy().hasHeightForWidth());
        editRechercheCours->setSizePolicy(sizePolicy);

        coursToolbarLayout->addWidget(editRechercheCours);


        coursOuterLayout->addLayout(coursToolbarLayout);

        tableCours = new QTableView(pageCours);
        tableCours->setObjectName("tableCours");
        sizePolicy1.setHeightForWidth(tableCours->sizePolicy().hasHeightForWidth());
        tableCours->setSizePolicy(sizePolicy1);

        coursOuterLayout->addWidget(tableCours);

        stackedWidget->addWidget(pageCours);
        pageFormateurs = new QWidget();
        pageFormateurs->setObjectName("pageFormateurs");
        sizePolicy1.setHeightForWidth(pageFormateurs->sizePolicy().hasHeightForWidth());
        pageFormateurs->setSizePolicy(sizePolicy1);
        formOuterLayout = new QVBoxLayout(pageFormateurs);
        formOuterLayout->setSpacing(8);
        formOuterLayout->setObjectName("formOuterLayout");
        formOuterLayout->setContentsMargins(6, 10, 6, 6);
        labelTitreFormateurs = new QLabel(pageFormateurs);
        labelTitreFormateurs->setObjectName("labelTitreFormateurs");
        labelTitreFormateurs->setFont(font1);
        labelTitreFormateurs->setAlignment(Qt::AlignCenter);

        formOuterLayout->addWidget(labelTitreFormateurs);

        formFormLayout = new QGridLayout();
        formFormLayout->setSpacing(8);
        formFormLayout->setObjectName("formFormLayout");
        label = new QLabel(pageFormateurs);
        label->setObjectName("label");

        formFormLayout->addWidget(label, 0, 0, 1, 1);

        editIdFormateur = new QLineEdit(pageFormateurs);
        editIdFormateur->setObjectName("editIdFormateur");

        formFormLayout->addWidget(editIdFormateur, 0, 1, 1, 1);

        label_6 = new QLabel(pageFormateurs);
        label_6->setObjectName("label_6");

        formFormLayout->addWidget(label_6, 0, 2, 1, 1);

        editEmail = new QLineEdit(pageFormateurs);
        editEmail->setObjectName("editEmail");
        sizePolicy.setHeightForWidth(editEmail->sizePolicy().hasHeightForWidth());
        editEmail->setSizePolicy(sizePolicy);

        formFormLayout->addWidget(editEmail, 0, 3, 1, 1);

        btnAjouterFormateur = new QPushButton(pageFormateurs);
        btnAjouterFormateur->setObjectName("btnAjouterFormateur");
        btnAjouterFormateur->setMinimumSize(QSize(90, 30));

        formFormLayout->addWidget(btnAjouterFormateur, 0, 4, 1, 1);

        label_2 = new QLabel(pageFormateurs);
        label_2->setObjectName("label_2");

        formFormLayout->addWidget(label_2, 1, 0, 1, 1);

        editNom = new QLineEdit(pageFormateurs);
        editNom->setObjectName("editNom");

        formFormLayout->addWidget(editNom, 1, 1, 1, 1);

        label_5 = new QLabel(pageFormateurs);
        label_5->setObjectName("label_5");

        formFormLayout->addWidget(label_5, 1, 2, 1, 1);

        editTelephone = new QLineEdit(pageFormateurs);
        editTelephone->setObjectName("editTelephone");

        formFormLayout->addWidget(editTelephone, 1, 3, 1, 1);

        btnModifierFormateur = new QPushButton(pageFormateurs);
        btnModifierFormateur->setObjectName("btnModifierFormateur");
        btnModifierFormateur->setMinimumSize(QSize(90, 30));

        formFormLayout->addWidget(btnModifierFormateur, 1, 4, 1, 1);

        label_3 = new QLabel(pageFormateurs);
        label_3->setObjectName("label_3");

        formFormLayout->addWidget(label_3, 2, 0, 1, 1);

        editPrenom = new QLineEdit(pageFormateurs);
        editPrenom->setObjectName("editPrenom");

        formFormLayout->addWidget(editPrenom, 2, 1, 1, 1);

        label_4 = new QLabel(pageFormateurs);
        label_4->setObjectName("label_4");

        formFormLayout->addWidget(label_4, 2, 2, 1, 1);

        comboSpecialite = new QComboBox(pageFormateurs);
        comboSpecialite->setObjectName("comboSpecialite");
        sizePolicy.setHeightForWidth(comboSpecialite->sizePolicy().hasHeightForWidth());
        comboSpecialite->setSizePolicy(sizePolicy);

        formFormLayout->addWidget(comboSpecialite, 2, 3, 1, 1);

        btnSupprimerFormateur = new QPushButton(pageFormateurs);
        btnSupprimerFormateur->setObjectName("btnSupprimerFormateur");
        btnSupprimerFormateur->setMinimumSize(QSize(90, 30));

        formFormLayout->addWidget(btnSupprimerFormateur, 2, 4, 1, 1);

        label_13 = new QLabel(pageFormateurs);
        label_13->setObjectName("label_13");

        formFormLayout->addWidget(label_13, 3, 0, 1, 1);

        editSalaire = new QLineEdit(pageFormateurs);
        editSalaire->setObjectName("editSalaire");

        formFormLayout->addWidget(editSalaire, 3, 1, 1, 1);

        label_14 = new QLabel(pageFormateurs);
        label_14->setObjectName("label_14");

        formFormLayout->addWidget(label_14, 3, 2, 1, 1);

        dateEditEmbauche = new QDateEdit(pageFormateurs);
        dateEditEmbauche->setObjectName("dateEditEmbauche");
        sizePolicy.setHeightForWidth(dateEditEmbauche->sizePolicy().hasHeightForWidth());
        dateEditEmbauche->setSizePolicy(sizePolicy);

        formFormLayout->addWidget(dateEditEmbauche, 3, 3, 1, 1);

        btnAfficherFormateurs = new QPushButton(pageFormateurs);
        btnAfficherFormateurs->setObjectName("btnAfficherFormateurs");
        btnAfficherFormateurs->setMinimumSize(QSize(90, 30));

        formFormLayout->addWidget(btnAfficherFormateurs, 3, 4, 1, 1);


        formOuterLayout->addLayout(formFormLayout);

        formToolbarLayout = new QHBoxLayout();
        formToolbarLayout->setSpacing(8);
        formToolbarLayout->setObjectName("formToolbarLayout");
        btnPdfFormateur = new QPushButton(pageFormateurs);
        btnPdfFormateur->setObjectName("btnPdfFormateur");
        btnPdfFormateur->setMinimumSize(QSize(90, 30));

        formToolbarLayout->addWidget(btnPdfFormateur);

        btnStatFormateur = new QPushButton(pageFormateurs);
        btnStatFormateur->setObjectName("btnStatFormateur");
        btnStatFormateur->setMinimumSize(QSize(160, 30));

        formToolbarLayout->addWidget(btnStatFormateur);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        formToolbarLayout->addItem(spacerItem1);

        btnRechercherFormateur = new QPushButton(pageFormateurs);
        btnRechercherFormateur->setObjectName("btnRechercherFormateur");
        btnRechercherFormateur->setMinimumSize(QSize(110, 30));

        formToolbarLayout->addWidget(btnRechercherFormateur);

        editRechercheFormateur = new QLineEdit(pageFormateurs);
        editRechercheFormateur->setObjectName("editRechercheFormateur");
        sizePolicy.setHeightForWidth(editRechercheFormateur->sizePolicy().hasHeightForWidth());
        editRechercheFormateur->setSizePolicy(sizePolicy);

        formToolbarLayout->addWidget(editRechercheFormateur);


        formOuterLayout->addLayout(formToolbarLayout);

        tableFormateurs = new QTableView(pageFormateurs);
        tableFormateurs->setObjectName("tableFormateurs");
        sizePolicy1.setHeightForWidth(tableFormateurs->sizePolicy().hasHeightForWidth());
        tableFormateurs->setSizePolicy(sizePolicy1);

        formOuterLayout->addWidget(tableFormateurs);

        stackedWidget->addWidget(pageFormateurs);

        mainLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1086, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Centre de Formation", nullptr));
        btnFormateurs->setText(QCoreApplication::translate("MainWindow", "\360\237\221\250\342\200\215\360\237\217\253 FORMATEURS", nullptr));
        btnCours->setText(QCoreApplication::translate("MainWindow", "\360\237\223\232 COURS", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "GESTION DES COURS", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "ID cours", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Niveau", nullptr));
        btnAjouterCours->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Dur\303\251e (h)", nullptr));
        btnModifierCours->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Description", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Prix (TND)", nullptr));
        btnSupprimerCours->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Date d\303\251but", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Formateur", nullptr));
        btnAfficherCours->setText(QCoreApplication::translate("MainWindow", "Afficher les cours", nullptr));
        btnPdfCours->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 PDF", nullptr));
        btnStatCours->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques cours", nullptr));
        btnRechercherCours->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Rechercher", nullptr));
        labelTitreFormateurs->setText(QCoreApplication::translate("MainWindow", "GESTION DES FORMATEURS", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID Formateur", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        btnAjouterFormateur->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        btnModifierFormateur->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251", nullptr));
        btnSupprimerFormateur->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Date embauche", nullptr));
        btnAfficherFormateurs->setText(QCoreApplication::translate("MainWindow", "Afficher", nullptr));
        btnPdfFormateur->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 PDF", nullptr));
        btnStatFormateur->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques formateurs", nullptr));
        btnRechercherFormateur->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Rechercher", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
