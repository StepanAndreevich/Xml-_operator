#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include "tablemodel.h"
#include <QDomDocument>
#include "progressbarwidget.h"
#include <QThread>
#include "parser.h"
#include "fileimporter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onImportData();
    void onClearTable();
    void customMenuRequested(QPoint pos);
    void onContextSave();
    //void onContextEdit();
    void onContextRemove();
    void updateDb();
signals:
    void startProcess(QStringList files);
    void addProgress(int val);


private:
    void initImporter();
    void hideProgressBar();

    Ui::MainWindow *m_ui;
    Database* m_db;
    TableModel* m_model;
    Parser* m_parser;
    FileImporter* m_importer;
    ProgressBarWidget* m_progressWidget = nullptr;
    QThread* m_thread;

};
#endif // MAINWINDOW_H
