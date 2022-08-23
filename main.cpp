#include "Heuristic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Heuristic w;
    w.show();
    return a.exec();
}
