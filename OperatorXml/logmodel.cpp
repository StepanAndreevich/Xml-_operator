#include "logmodel.h"

LogModel::LogModel(QWidget* parent) : QAbstractListModel()
{

}

LogModel::~LogModel()
{

}

void LogModel::log(const QString& text)
{
    auto row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_logItem.append(text);
    endInsertRows();
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    return m_logItem.size();
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return value = m_logItem.at(index.row());

    return QVariant();
}

void LogModel::addRow(const QString &str)
{
    auto row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_logItem.append(str);
    endInsertRows();
}

void LogModel::clear()
{
    beginResetModel();
    m_logItem.clear();
    endResetModel();
}
