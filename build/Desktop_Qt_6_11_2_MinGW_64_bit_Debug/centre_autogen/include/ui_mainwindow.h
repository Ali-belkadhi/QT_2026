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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *navigationWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnFormateurs;
    QPushButton *btnCours;
    QStackedWidget *stackedWidget;
    QWidget *pageCours;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *editIdCours;
    QLineEdit *editNomCours;
    QComboBox *comboFormateur;
    QLineEdit *editDuree;
    QLineEdit *editPrix;
    QPushButton *btnAjouterCours;
    QPushButton *btnModifierCours;
    QPushButton *btnSupprimerCours;
    QPushButton *btnAfficherCours;
    QTableView *tableCours;
    QLabel *label_12;
    QLabel *label_15;
    QLabel *label_16;
    QDateEdit *dateEditDebut;
    QLineEdit *editDescriptionCours;
    QLabel *label_17;
    QComboBox *comboNiveau;
    QWidget *pageFormateurs;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *editIdFormateur;
    QLineEdit *editNom;
    QLineEdit *editPrenom;
    QLineEdit *editEmail;
    QLineEdit *editTelephone;
    QLineEdit *editSpecialite;
    QPushButton *btnAjouterFormateur;
    QPushButton *btnModifierFormateur;
    QPushButton *btnSupprimerFormateur;
    QPushButton *btnAfficherFormateurs;
    QTableView *tableFormateurs;
    QLabel *label_13;
    QLineEdit *editSalaire;
    QLabel *label_14;
    QDateEdit *dateEditEmbauche;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1086, 715);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(10, 0, 1061, 681));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        navigationWidget = new QWidget(verticalLayoutWidget);
        navigationWidget->setObjectName("navigationWidget");
        navigationWidget->setEnabled(true);
        horizontalLayoutWidget = new QWidget(navigationWidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 1021, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnFormateurs = new QPushButton(horizontalLayoutWidget);
        btnFormateurs->setObjectName("btnFormateurs");

        horizontalLayout->addWidget(btnFormateurs);

        btnCours = new QPushButton(horizontalLayoutWidget);
        btnCours->setObjectName("btnCours");

        horizontalLayout->addWidget(btnCours);

        stackedWidget = new QStackedWidget(navigationWidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setEnabled(true);
        stackedWidget->setGeometry(QRect(20, 110, 1011, 551));
        pageCours = new QWidget();
        pageCours->setObjectName("pageCours");
        label_7 = new QLabel(pageCours);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(40, 30, 63, 20));
        label_8 = new QLabel(pageCours);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(50, 100, 63, 20));
        label_9 = new QLabel(pageCours);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(380, 120, 63, 20));
        label_10 = new QLabel(pageCours);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(380, 150, 63, 20));
        label_11 = new QLabel(pageCours);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(370, 200, 111, 20));
        editIdCours = new QLineEdit(pageCours);
        editIdCours->setObjectName("editIdCours");
        editIdCours->setGeometry(QRect(150, 40, 113, 26));
        editNomCours = new QLineEdit(pageCours);
        editNomCours->setObjectName("editNomCours");
        editNomCours->setGeometry(QRect(150, 100, 113, 26));
        comboFormateur = new QComboBox(pageCours);
        comboFormateur->setObjectName("comboFormateur");
        comboFormateur->setGeometry(QRect(510, 190, 201, 26));
        editDuree = new QLineEdit(pageCours);
        editDuree->setObjectName("editDuree");
        editDuree->setGeometry(QRect(510, 100, 113, 26));
        editPrix = new QLineEdit(pageCours);
        editPrix->setObjectName("editPrix");
        editPrix->setGeometry(QRect(520, 160, 113, 26));
        btnAjouterCours = new QPushButton(pageCours);
        btnAjouterCours->setObjectName("btnAjouterCours");
        btnAjouterCours->setGeometry(QRect(890, 40, 93, 29));
        btnModifierCours = new QPushButton(pageCours);
        btnModifierCours->setObjectName("btnModifierCours");
        btnModifierCours->setGeometry(QRect(900, 80, 93, 29));
        btnSupprimerCours = new QPushButton(pageCours);
        btnSupprimerCours->setObjectName("btnSupprimerCours");
        btnSupprimerCours->setGeometry(QRect(900, 140, 93, 29));
        btnAfficherCours = new QPushButton(pageCours);
        btnAfficherCours->setObjectName("btnAfficherCours");
        btnAfficherCours->setGeometry(QRect(840, 190, 161, 29));
        tableCours = new QTableView(pageCours);
        tableCours->setObjectName("tableCours");
        tableCours->setGeometry(QRect(20, 290, 1001, 261));
        label_12 = new QLabel(pageCours);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(370, 0, 301, 20));
        label_15 = new QLabel(pageCours);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(40, 160, 63, 20));
        label_16 = new QLabel(pageCours);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(40, 210, 63, 20));
        dateEditDebut = new QDateEdit(pageCours);
        dateEditDebut->setObjectName("dateEditDebut");
        dateEditDebut->setGeometry(QRect(170, 210, 110, 26));
        editDescriptionCours = new QLineEdit(pageCours);
        editDescriptionCours->setObjectName("editDescriptionCours");
        editDescriptionCours->setGeometry(QRect(170, 160, 113, 26));
        label_17 = new QLabel(pageCours);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(390, 50, 63, 20));
        comboNiveau = new QComboBox(pageCours);
        comboNiveau->setObjectName("comboNiveau");
        comboNiveau->setGeometry(QRect(520, 40, 151, 26));
        stackedWidget->addWidget(pageCours);
        pageFormateurs = new QWidget();
        pageFormateurs->setObjectName("pageFormateurs");
        label = new QLabel(pageFormateurs);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 30, 101, 20));
        label_2 = new QLabel(pageFormateurs);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 70, 63, 20));
        label_3 = new QLabel(pageFormateurs);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 120, 63, 20));
        label_4 = new QLabel(pageFormateurs);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(360, 120, 91, 20));
        label_5 = new QLabel(pageFormateurs);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(360, 70, 81, 20));
        label_6 = new QLabel(pageFormateurs);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(360, 30, 63, 20));
        editIdFormateur = new QLineEdit(pageFormateurs);
        editIdFormateur->setObjectName("editIdFormateur");
        editIdFormateur->setGeometry(QRect(180, 30, 113, 26));
        editNom = new QLineEdit(pageFormateurs);
        editNom->setObjectName("editNom");
        editNom->setGeometry(QRect(190, 70, 113, 26));
        editPrenom = new QLineEdit(pageFormateurs);
        editPrenom->setObjectName("editPrenom");
        editPrenom->setGeometry(QRect(190, 120, 113, 26));
        editEmail = new QLineEdit(pageFormateurs);
        editEmail->setObjectName("editEmail");
        editEmail->setGeometry(QRect(470, 30, 271, 26));
        editTelephone = new QLineEdit(pageFormateurs);
        editTelephone->setObjectName("editTelephone");
        editTelephone->setGeometry(QRect(470, 80, 181, 26));
        editSpecialite = new QLineEdit(pageFormateurs);
        editSpecialite->setObjectName("editSpecialite");
        editSpecialite->setGeometry(QRect(470, 130, 271, 26));
        btnAjouterFormateur = new QPushButton(pageFormateurs);
        btnAjouterFormateur->setObjectName("btnAjouterFormateur");
        btnAjouterFormateur->setGeometry(QRect(850, 20, 93, 29));
        btnModifierFormateur = new QPushButton(pageFormateurs);
        btnModifierFormateur->setObjectName("btnModifierFormateur");
        btnModifierFormateur->setGeometry(QRect(850, 70, 93, 29));
        btnSupprimerFormateur = new QPushButton(pageFormateurs);
        btnSupprimerFormateur->setObjectName("btnSupprimerFormateur");
        btnSupprimerFormateur->setGeometry(QRect(850, 120, 93, 29));
        btnAfficherFormateurs = new QPushButton(pageFormateurs);
        btnAfficherFormateurs->setObjectName("btnAfficherFormateurs");
        btnAfficherFormateurs->setGeometry(QRect(850, 170, 93, 29));
        tableFormateurs = new QTableView(pageFormateurs);
        tableFormateurs->setObjectName("tableFormateurs");
        tableFormateurs->setGeometry(QRect(10, 200, 1001, 341));
        label_13 = new QLabel(pageFormateurs);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(20, 160, 63, 20));
        editSalaire = new QLineEdit(pageFormateurs);
        editSalaire->setObjectName("editSalaire");
        editSalaire->setGeometry(QRect(190, 160, 113, 26));
        label_14 = new QLabel(pageFormateurs);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(360, 170, 111, 20));
        dateEditEmbauche = new QDateEdit(pageFormateurs);
        dateEditEmbauche->setObjectName("dateEditEmbauche");
        dateEditEmbauche->setGeometry(QRect(490, 170, 110, 26));
        stackedWidget->addWidget(pageFormateurs);

        verticalLayout_2->addWidget(navigationWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1086, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnFormateurs->setText(QCoreApplication::translate("MainWindow", "\360\237\221\250\342\200\215\360\237\217\253 FORMATEURS", nullptr));
        btnCours->setText(QCoreApplication::translate("MainWindow", "\360\237\223\232 COURS", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "ID cours", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Dur\303\251e", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Prix", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Formateur", nullptr));
        btnAjouterCours->setText(QCoreApplication::translate("MainWindow", "Ajouter ", nullptr));
        btnModifierCours->setText(QCoreApplication::translate("MainWindow", "Modifier ", nullptr));
        btnSupprimerCours->setText(QCoreApplication::translate("MainWindow", "Supprimer ", nullptr));
        btnAfficherCours->setText(QCoreApplication::translate("MainWindow", "Afficher les cours", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", " GESTION DES FORMATEURS", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Description     ", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Date d\303\251but ", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Niveau       ", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "ID Formateur", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Nom          ", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Pr\303\251nom       ", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251   ", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone    ", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Email        ", nullptr));
        btnAjouterFormateur->setText(QCoreApplication::translate("MainWindow", "Ajouter ", nullptr));
        btnModifierFormateur->setText(QCoreApplication::translate("MainWindow", "Modifier ", nullptr));
        btnSupprimerFormateur->setText(QCoreApplication::translate("MainWindow", "Supprimer ", nullptr));
        btnAfficherFormateurs->setText(QCoreApplication::translate("MainWindow", "Afficher", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Salaire", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Date embauche", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
