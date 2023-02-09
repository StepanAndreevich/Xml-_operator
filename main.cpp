#include "mainwindow.h"
#include <QtSql>
#include <QApplication>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "ru");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("XML-file worcker");
    w.show();
    return a.exec();
}
