#include "database.h"
#include <QSqlQuery>
#include <QDebug>
#include <QFileInfo>
#include <QSqlError>


Database::Database()
{

}

void Database::connectToDatabase()
{
    QString path = "./xmlData.sqlite";
    QFile dbFile(path);
    if(dbFile.exists())
        openDatabase();
    else
        restoreDatabase();
}

bool Database::createTable()
{
    QSqlQuery query;
    QString createQuery = QString("create table editor\
                                  (id integer primary key,\
                                   text_editor CHARACTER VARYING(30),\
                                   file_formats CHARACTER VARYING(30),\
                                   encoding CHARACTER VARYING(30),\
                                   has_intelli_sense BIT defualt 'FALSE',\
                                   has_plugins BIT default 'FALSE',\
                                   can_compile BIT default 'FALSE') ");
    if(!query.exec(createQuery))
    {
        auto err = query.lastError().text();
        qDebug() << "Не удалось создать таблицу. " << err;
        return false;
    }

    return true;
}

bool Database::openDatabase()
{
    QString path =
            QString("./xmlData.sqlite");
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if(!m_db.open())
    {
        auto err = m_db.lastError().text();
        qDebug() << "Не удалось открыть базу данных. " << err;
        return false;
    }

    qDebug() << "База данных открыта.";
    return true;
}

bool Database::restoreDatabase()
{
    if(openDatabase())
        return createTable();
    else
    {
        qDebug() << "Не удалось востановить базу данных.";
        return false;
    }
}

void Database::closeDatabase()
{
    m_db.close();
}

void Database::insertRow(QStringList str)
{
    QString quStr = QString("insert into editor(text_editor, file_formats, encoding, has_intelli_sense,\
                            has_plugins, can_compile)\
                            values('%1', '%2', '%3', %4, %5, %6)")
                            .arg(str.at(0)).arg(str.at(1)).arg(str.at(2)).arg(str.at(3)).arg(str.at(4)).arg(str.at(5));

    QSqlQuery qu(m_db);
    if(!qu.exec(quStr))
    {
        auto err = qu.lastError().text();
        qDebug() << "Не удалось вставить строку с таблицу. " << err;
        return;
    }
}

void Database::removeRow(const QString &str)
{
    QString quStr = QString("delete from editor where id = %1").arg(str);

    QSqlQuery qu(m_db);
    if(!qu.exec(quStr))
    {
        auto err = qu.lastError().text();
        qDebug() << "Не удалось вставить удалить строку таблицы. " << err;
        return;
    }
}

void Database::updateRow(QVariant data, const QString& columnName, const QString& row)
{
    QString quStr = QString("update editor set %1 = '%2' where id = %3").arg(columnName).arg(data.toString()).arg(row);

    QSqlQuery qu(m_db);
    if(!qu.exec(quStr))
    {
        auto err = qu.lastError().text();
        qDebug() << "Не удалось обновить строку таблицы. " << err;
        return;
    }
}

QVariantList Database::idRows()
{
    QString quStr = QString("select id \
                             from editor");
    QSqlQuery qu(m_db);
    if(!qu.exec(quStr))
    {
        auto err = qu.lastError().text();
        qDebug() << "Не удалось вставить строку с таблицу. " << err;
        return {};
    }

     QVariantList dataTable;
     while(qu.next())
     {
         auto id = qu.value(0);
         dataTable.append(id);
     }

     return dataTable;
}

void Database::clearTable()
{
    QString quStr = QString("delete from editor");

    QSqlQuery qu(m_db);
    if(!qu.exec(quStr))
    {
        auto err = qu.lastError().text();
        qDebug() << "Не удалось очистить таблицу. " << err;
        return;
    }
}

QList<QVariantList> Database::dataFromDb() const
{
    QString quStr = QString("select text_editor, file_formats, encoding, has_intelli_sense, has_plugins, can_compile \
                             from editor");
    QSqlQuery qu(m_db);
    if(!qu.exec(quStr))
    {
        auto err = qu.lastError().text();
        qDebug() << "Не удалось вставить строку с таблицу. " << err;
        return {};
    }

    QList<QVariantList> dataTable;
    while(qu.next())
    {
        QVariantList data;
        auto name = qu.value(0);
        data.append(name);

        auto format = qu.value(1);
        data.append(format);

        auto encoding = qu.value(2);
        data.append(encoding);

        auto intelliSence = qu.value(3);
        data.append(intelliSence);

        auto plugins = qu.value(4);
        data.append(plugins);

        auto canCompile = qu.value(5);
        data.append(canCompile);

        dataTable.append(data);
    }

    return dataTable;
}




