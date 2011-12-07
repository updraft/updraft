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

    MainWindow w;
    UpdraftParent parent;
    parent.setMainWindow(&w);
    w.show();

    return a.exec();
}
