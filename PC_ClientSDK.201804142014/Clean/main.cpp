#include "Clean.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Clean w;
    w.setWindowFlags((w.windowFlags()&~Qt::WindowMinMaxButtonsHint) | Qt::WindowStaysOnTopHint);

    w.show();
    return a.exec();
}
