#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <QAbstractTableModel>


class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TableModel(QList<QVariantList> data, QObject* parent = nullptr);
    ~TableModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

    void addData(QStringList data);
    void removeRow(int row);
    void updateId(QVariantList data);
    void clear();

    QMap<QString, QString> tableToDbIds();
    QMap<QString, QString> tableToDbHeaders();

private:
    enum Column
    {
        NAME = 0,
        FORMAT,
        ENCODING,
        HASINTELLI,
        HASPLUGINS,
        CANCOMPILE,
        LAST
    };

    QMap<QString, QString> m_tableOnDbHeaders = {
        {"Name", "text_editor"},
        {"Format", "file_formats"},
        {"Encoding", "encoding"},
        {"Has intelli sense", "has_intelli_sense"},
        {"Has plugins", "has_plugins"},
        {"Can compile", "can_compile"}
    };


    QMap<QString, QString> m_tableToDbIds;

    typedef QHash<Column, QVariant> EditorData;
    typedef QList<EditorData> Editor;
    Editor m_editor;
};

#endif // TABLEMODEL_H
