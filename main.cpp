#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Function Drawer");
    w.setWindowIcon(QIcon("func.png"));
    w.show();
    return a.exec();
}
