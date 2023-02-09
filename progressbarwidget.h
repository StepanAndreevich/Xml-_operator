#ifndef PROGRESSBARWIDGET_H
#define PROGRESSBARWIDGET_H

#include <QWidget>
#include <QDialog>
#include "logmodel.h"
#include <QAbstractButton>
#include "fileimporter.h"

namespace Ui {
class ProgressBarWidget;
}

class ProgressBarWidget : public QWidget
{
    Q_OBJECT

public:
    ProgressBarWidget(QWidget *parent = nullptr);
    ~ProgressBarWidget();
    void setImporter(FileImporter* importer);

public slots:
    void changeValue(double value);
    void addLog(const QString& str);
    void setLable(const QString& str);
    void onOkey();

signals:
    void progressEnd();

private:
    int m_min = 0;
    int m_max = 100;

    Ui::ProgressBarWidget *m_ui;
    LogModel* m_model;
    FileImporter* m_importer;
};

#endif // PROGRESSBARWIDGET_H
