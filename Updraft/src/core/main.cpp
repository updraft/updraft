#include <QtGui/QApplication>
#include <QTranslator>
#include "ui/mainwindow.h"
#include "updraftparent.h"

using Updraft::Core::UpdraftParent;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator trans;
    trans.load("translations/czech");
    a.installTranslator(&trans);

    Updraft::Core::MainWindow w(NULL);
    UpdraftParent parent(&w);
    w.show();

    return a.exec();
}
