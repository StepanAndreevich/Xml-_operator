#include "progressbarwidget.h"
#include "ui_progressbarwidget.h"

ProgressBarWidget::ProgressBarWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ProgressBarWidget)
{
    m_ui->setupUi(this);
    m_model = new LogModel();
    m_ui->listView->setModel(m_model);
    m_ui->progressBar->setRange(m_min, m_max);
    connect(m_ui->pbOkey, &QPushButton::clicked, this, &ProgressBarWidget::onOkey);
}

ProgressBarWidget::~ProgressBarWidget()
{
    delete m_model;
    delete m_ui;
}

void ProgressBarWidget::setImporter(FileImporter* importer)
{
    m_importer = importer;
    connect(m_importer, &FileImporter::changeValue, this, &ProgressBarWidget::changeValue);
    connect(m_importer, &FileImporter::newLog, this, &ProgressBarWidget::addLog);
    connect(m_importer, &FileImporter::changeLable, this, &ProgressBarWidget::setLable);
}

void ProgressBarWidget::changeValue(double value)
{
    m_ui->progressBar->setValue(value);
}

void ProgressBarWidget::addLog(const QString &str)
{
    m_model->addRow(str);
}

void ProgressBarWidget::setLable(const QString &str)
{
    m_ui->lbLoaded->setText(str);
}

void ProgressBarWidget::onOkey()
{
    m_model->clear();
    emit progressEnd();
}
