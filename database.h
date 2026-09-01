#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    static bool connect();
    static void close();
};

#endif // DATABASE_H