#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void paintEvent(){
        qDebug() << "main window paint event";
    }

public slots:
    void buttonClicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
