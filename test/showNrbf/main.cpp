#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft YaHei", 9));

    MainWindow w;
    w.show();

    return a.exec();
}