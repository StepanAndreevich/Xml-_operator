#ifndef PARSER_H
#define PARSER_H
#include <QObject>
#include <QDomDocument>

class Parser : public QObject
{
    Q_OBJECT
public:
    Parser();
    ~Parser();

    QStringList readXml(const QString& path);
    bool writeXml(QVariantList xmlData);
    QList<QStringList> curreuntRead() const;

private:
    QList<QStringList> m_currentRead;
    QMap<QString, QVariant> prepareFileData(QDomDocument dom, QVariantList xmlData);

};

#endif // PARSER_H
