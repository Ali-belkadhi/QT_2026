#include "mainwindow.h"
#include "database.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Drivers SQL disponibles :"
             << QSqlDatabase::drivers();

    if (!Database::connect())
    {
        QMessageBox::critical(
            nullptr,
            "Erreur",
            "Impossible de se connecter à Oracle."
            );

        return -1;
    }

    MainWindow w;
    w.show();

    int result = a.exec();

    Database::close();

    return result;
}