#include <QtGui>
#include "window.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setApplicationName("StageMan");
    a.setOrganizationDomain("www.stageman.net");
    a.setOrganizationName("StageMan");
    Window w;
    w.show();
    return a.exec();
}
