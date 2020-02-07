#include "mainwindow.h"
//#include "sockettest.h"

#include <QCoreApplication>
#include <QApplication>



int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


