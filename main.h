#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
public:
    // Singleton moderne
    static Connection& createInstance()
    {
        static Connection instance;
        return instance;
    }

    bool createConnect();
    QSqlDatabase getDatabase() const { return db; }

private:
    Connection() {}
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    QSqlDatabase db;
};

#endif // CONNECTION_H
