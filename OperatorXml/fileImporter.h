#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "database.h"
#include "tablemodel.h"

class FileImporter : public QObject
{
    Q_OBJECT
public:
    FileImporter(QObject* parent = nullptr);
    ~FileImporter();
    void setDatabase(Database* db);
    void setModel(TableModel* model);

public slots:
    void process(QStringList files);

signals:
    void importFinished();
    void newLog(const QString& str);
    void changeValue(double val);
    void changeLable(const QString& str);

private:
    Database* m_db;
    TableModel* m_model;
};

#endif // WORKER_H
