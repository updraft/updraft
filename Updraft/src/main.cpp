#include <QtGui/QApplication>
#include <QTranslator>
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator trans;
  trans.load("czech");
  a.installTranslator(&trans);

  MainWindow w;
  w.show();

  return a.exec();
}
