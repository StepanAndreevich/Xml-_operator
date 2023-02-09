#include "parser.h"
#include <QFileDialog>
#include <QMap>
#include "mainwindow.h"
#include <QTextStream>
#include <QDebug>

Parser::Parser()
{

}

Parser::~Parser()
{
    m_currentRead.clear();
}

QStringList Parser::readXml(const QString& path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        return {};

    QDomDocument dom("mydocument");
    if(!dom.setContent(&file))
    {
        file.close();
        qDebug() << "Не удалось прочитать исходный файл.";
        return {};
    }

    QStringList res;

    QDomElement docElem = dom.documentElement();
    QDomNode docNode = docElem.firstChild();
    while(!docNode.isNull())
    {
        QDomElement nodeElem = docNode.toElement();
        if(nodeElem.isNull())
            continue;
        auto nodeName = nodeElem.toElement().tagName();
        auto dataText = nodeElem.text();

        if(nodeName == "texteditor")
            res.append(nodeElem.text());

        if(nodeName == "fileformats")
            res.append(nodeElem.text());

        if(nodeName == "encoding")
            res.append(nodeElem.text());

        if(nodeName == "hasintellisense")
            res.append(nodeElem.text());

        if(nodeName == "hasplugins")
            res.append(nodeElem.text());

        if(nodeName == "cancompile")
            res.append(nodeElem.text());

        docNode = docNode.nextSibling();
    }

    file.close();
    m_currentRead.append(res);
    return res;
}

bool Parser::writeXml(QVariantList xmlData)
{
    if(xmlData.isEmpty())
        return false;

    QDomDocument dom;
    QDomElement root = dom.createElement("root");
    auto tagNameToValue = prepareFileData(dom, xmlData);
    dom.appendChild(root);
    for(auto it = tagNameToValue.begin(); it != tagNameToValue.end(); it++)
    {
        auto tagName = dom.createElement(it.key());
        root.appendChild(tagName);

        auto value = dom.createTextNode(it.value().toString());
        tagName.appendChild(value);
    }

    QFileDialog saveFile;
    auto path = saveFile.getSaveFileName(nullptr, tr("Save file"), "", tr("*.xml"));
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream out(&file);
    dom.save(out, 4);
    file.close();

    return true;
}

QList<QStringList> Parser::curreuntRead() const
{
    return m_currentRead;
}

QMap<QString, QVariant> Parser::prepareFileData(QDomDocument dom, QVariantList xmlData)
{
    QMap<QString, QVariant> tagNameToValue;

    QDomElement name = dom.createElement("texteditor");
    tagNameToValue.insert("texteditor", xmlData.at(0));

    QDomElement format = dom.createElement("fileformats");
    tagNameToValue.insert("fileformats", xmlData.at(1));

    QDomElement encoding = dom.createElement("encoding");
    tagNameToValue.insert("encoding", xmlData.at(2));

    QDomElement hasIntelli = dom.createElement("hasintellisense");
    tagNameToValue.insert("hasintellisense", xmlData.at(3));

    QDomElement hasPlugins = dom.createElement("hasplugins");
    tagNameToValue.insert("hasplugins", xmlData.at(4));

    QDomElement canCompile = dom.createElement("cancompile");
    tagNameToValue.insert("cancompile", xmlData.at(5));

    return tagNameToValue;
}
