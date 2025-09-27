#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SearchableTable w;
    w.resize(600, 400);
    w.show();
    return a.exec();
}
