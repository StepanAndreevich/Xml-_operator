#include "tablemodel.h"
#include <QDebug>

TableModel::TableModel(QList<QVariantList> dataTable, QObject* parent) : QAbstractTableModel(parent)
{
    for(auto data : dataTable)
    {
        EditorData editor;
        editor[NAME] = data.at(0).toString();
        editor[FORMAT] = data.at(1).toString();
        editor[ENCODING] = data.at(2).toString();
        editor[HASINTELLI] = data.at(3).toBool();
        editor[HASPLUGINS] = data.at(4).toBool();
        editor[CANCOMPILE] = data.at(5).toBool();

        m_editor.append(editor);
    }
}

TableModel::~TableModel()
{

}

QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return createIndex(row, column);
    }
    return QModelIndex();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return m_editor.count();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    int column = LAST;
    return column;
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(m_editor.count() <= index.row() || (role != Qt::DisplayRole && role != Qt::EditRole ))
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case NAME: return m_editor[index.row()][Column(index.column())];
        case FORMAT: return m_editor[index.row()][Column(index.column())];
        case ENCODING: return m_editor[index.row()][Column(index.column())];
        }
    }

    return m_editor[index.row()][Column(index.column())];
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role != Qt::EditRole || m_editor.count() <= index.row())
        return false;

    m_editor[index.row()][Column(index.column())] = value;
    emit dataChanged(index, index);
    return true;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Vertical)
        return section;

    switch(section)
    {
    case NAME:
        return trUtf8("Name");
    case FORMAT:
        return trUtf8("Format");
    case ENCODING:
        return trUtf8("Encoding");
    case HASINTELLI:
        return trUtf8("Has intelli sense");
    case HASPLUGINS:
        return trUtf8("Has plugins");
    case CANCOMPILE:
        return trUtf8("Can compile");
    }

    return QVariant();

}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    if(index.isValid())
//        if(index.column() == NAME || index.column() == FORMAT || index.column() == ENCODING ||
//                index.column() == HASINTELLI || index.column() == HASPLUGINS || index.column() == CANCOMPILE)
            flags |= Qt::ItemIsEditable;

    return flags;
}

void TableModel::addData(QStringList data)
{
    if(data.isEmpty())
    {
        qDebug() << "Не удалось добавить данные в таблицу. Входные данные пустые.";
        return;
    }

    EditorData editor;
    editor[NAME] = data.at(0);
    editor[FORMAT] = data.at(1);
    editor[ENCODING] = data.at(2);
    editor[HASINTELLI] = QVariant(data.at(3)).toBool();
    editor[HASPLUGINS] = QVariant(data.at(4)).toBool();
    editor[CANCOMPILE] = QVariant(data.at(5)).toBool();

    int row = m_editor.count();
    beginInsertRows(QModelIndex(), row, row);
    m_editor.append(editor);
    endInsertRows();
}

void TableModel::removeRow(int row)
{
    beginRemoveRows( QModelIndex(), row, row );
    m_editor.removeAt(row);
    endRemoveRows();
}

void TableModel::updateId(QVariantList data)
{
    m_tableToDbIds.clear();
    if(data.isEmpty())
    {
        qDebug() << "Не обновить данные в таблице. Входные данные пустые.";
        return;
    }

    int row = m_editor.count();
    for(auto i = 0; i < row; i++)
        m_tableToDbIds.insert(QString::number(i), data.at(i).toString());
}

void TableModel::clear()
{
    beginResetModel();
    m_editor.clear();
    endResetModel();
}

QMap<QString, QString> TableModel::tableToDbIds()
{
    return m_tableToDbIds;
}

QMap<QString, QString> TableModel::tableToDbHeaders()
{
    return m_tableOnDbHeaders;
}








