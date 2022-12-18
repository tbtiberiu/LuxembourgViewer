#include "LuxembourgViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LuxembourgViewer w;
    w.show();
    return a.exec();
}
