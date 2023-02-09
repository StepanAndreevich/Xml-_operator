#ifndef LOGMODEL_H
#define LOGMODEL_H
#include <QAbstractListModel>
#include <QDateTime>

class LogModel : public QAbstractListModel
{
public:
    LogModel(QWidget* parent = nullptr);
    ~LogModel();

    void log(const QString& text);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void addRow(const QString& str);
    void clear();

private:
    QStringList m_logItem;
};

#endif // LOGMODEL_H
