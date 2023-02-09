#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QVariant>


class Database
{
public:
    Database();
    ~Database();
    void connectToDatabase();
    void insertRow(QStringList str);
    void removeRow(const QString& str);
    void updateRow(QVariant data, const QString& columnName, const QString& row);
    QVariantList idRows();
    void clearTable();
    QList<QVariantList> dataFromDb() const;

private:
    bool createTable();
    bool openDatabase();
    bool restoreDatabase();
    void closeDatabase();

    QSqlDatabase m_db;
};

#endif // DATABASE_H
