#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QtSql>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_db(new Database)
    , m_parser(new Parser)
{
    m_ui->setupUi(this);
    m_db->connectToDatabase();
    m_ui->progressWidget->hide();

    auto dataTable =m_db->dataFromDb();
    m_model = new TableModel(dataTable);
    m_model->updateId(m_db->idRows());
    m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_ui->tableView->setModel(m_model);
    //m_ui->tableView->verticalHeader()->hide();
    m_ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    connect(m_ui->pbImportData, &QPushButton::clicked, this, &MainWindow::onImportData);
    connect(m_ui->pbClearTable, &QPushButton::clicked, this, &MainWindow::onClearTable);
    //connect(m_model, &QAbstractTableModel::dataChanged, this, &MainWindow::onContextEdit);
    connect(m_ui->progressWidget, &ProgressBarWidget::progressEnd, this, &MainWindow::hideProgressBar);
    connect(m_model, &TableModel::dataChanged, this, &MainWindow::updateDb);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::onImportData()
{
    QFileDialog fileDlg(this);
    auto filePaths = fileDlg.getOpenFileNames(this, tr("Open File"), "", tr("*.xml"));
    if(filePaths.isEmpty())
        return;

    initImporter();
    if(m_importer == nullptr)
    {
        qDebug() << "Импортер не инициализтрован.";
        return;
    }

    m_ui->progressWidget->setImporter(m_importer);
    m_ui->progressWidget->show();
    emit startProcess(filePaths);
}

void MainWindow::onClearTable()
{
    m_model->clear();
    m_db->clearTable();
}

void MainWindow::customMenuRequested(QPoint pos)
{
    //QModelIndex index = m_ui->tableView->indexAt(pos);

    QMenu* menu = new QMenu(this);
    QAction* save = new QAction(tr("Сохранить в xml"), this);
    connect(save, &QAction::triggered, this, &MainWindow::onContextSave);

//    QAction* editCell = new QAction(tr("Изменить"), this);
//    connect(editCell, &QAction::triggered, this, &MainWindow::onContextEdit);

    QAction* removeCell = new QAction(tr("Удалить"), this);
    connect(removeCell, &QAction::triggered, this, &MainWindow::onContextRemove);

    menu->addAction(save);
    //menu->addAction(editCell);
    menu->addAction(removeCell);
    menu->popup(m_ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::onContextSave()
{
    auto index = m_ui->tableView->selectionModel()->selectedIndexes();
    if(index.isEmpty())
        return;

    QVariantList res;
    auto curRow = index.first().row();
    auto columns = m_model->columnCount();
    for(auto i = 0; i < columns; i++)
    {
        auto ind = m_model->index(curRow, i);
        auto dataCell = m_model->data(ind, Qt::DisplayRole);
        res.append(dataCell);
    }

    m_parser->writeXml(res);
}

void MainWindow::onContextRemove()
{
    auto index = m_ui->tableView->selectionModel()->selectedIndexes();
    if(index.isEmpty())
        return;

    auto curRow = index.first().row();
    auto dbId = m_model->tableToDbIds().value(QString::number(curRow));
    m_model->removeRow(curRow);
    m_db->removeRow(dbId);
}

void MainWindow::updateDb()
{
    auto index = m_ui->tableView->selectionModel()->selectedIndexes();
    auto curRow = index.first().row();
    auto curCol = index.first().column();

    auto dataCell = m_model->data(index.first(), Qt::DisplayRole);
    auto colName = m_model->headerData(curCol,Qt::Horizontal, Qt::DisplayRole);

    auto dbColumnName = m_model->tableToDbHeaders().value(colName.toString());
    auto dbRow = m_model->tableToDbIds().value(QString::number(curRow));

    m_db->updateRow(dataCell, dbColumnName, dbRow);
}

void MainWindow::initImporter()
{
    m_thread = new QThread();
    connect(this, &MainWindow::destroyed, m_thread, &QThread::quit);

    m_importer = new FileImporter();
    m_importer->setDatabase(m_db);
    m_importer->setModel(m_model);
    connect(this, &MainWindow::startProcess, m_importer, &FileImporter::process);

    m_importer->moveToThread(m_thread);
    connect(m_importer, &FileImporter::importFinished, m_thread, &QThread::quit);
    m_thread->start();
}

void MainWindow::hideProgressBar()
{
    m_ui->progressWidget->hide();
    delete m_importer;
    m_importer = nullptr;
}



















