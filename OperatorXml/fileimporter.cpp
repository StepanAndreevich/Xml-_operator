#include "fileimporter.h"
#include <QFile>
#include <QDebug>
#include "parser.h"

FileImporter::FileImporter(QObject* parent) : QObject(parent)
{

}

FileImporter::~FileImporter()
{

}

void FileImporter::setDatabase(Database *db)
{
    m_db = db;
}

void FileImporter::setModel(TableModel *model)
{
    m_model = model;
}

void FileImporter::process(QStringList files)
{
    Parser* parser = new Parser;
    double count = 0;
    double filesCount = files.count();
    double progressVal = 0;
    for(auto f : files)
    {
        QFile file(f);
        if(!file.exists())
        {
            qDebug() << "Файла " << f << " не существует.";
            QString logStr = QString("Файл: %1, не существует.").arg(file.fileName());
            emit newLog(logStr);
            return;
        }

        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Не удалось открыть файл.";
            QString logStr = QString("Не удалось открыть файл: %1").arg(file.fileName());
            emit newLog(logStr);
            return;
        }

        auto data = parser->readXml(f);
        m_model->addData(data);

        file.close();

        m_db->insertRow(data);

        m_model->updateId(m_db->idRows());

        count++;
        progressVal = count / filesCount * 100;
        QString logStr = QString("Файл: %1, загружен.").arg(file.fileName());

        emit changeValue(progressVal);
        emit newLog(logStr);
        emit changeLable(QString("%0/%1").arg(QString::number(count)).arg(QString::number(files.count())));
    }

    emit importFinished();
}
