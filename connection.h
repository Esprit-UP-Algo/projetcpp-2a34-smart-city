#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
class Connection
{
public:
    Connection();
    ~Connection();
    static Connection& createInstance();
    bool createConnect();
    QSqlDatabase& getDatabase();
private:
    QSqlDatabase db;
};
#endif
