#include "prise.h"
#include "PRISESetting.h"
#include <QCoreApplication>
#include <QSctpSocket>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pwd = a.applicationDirPath();
//#ifdef MAC
    pwd += "/../../";
//#endif

//    QMessageBox::about(0,"test",pwd);
    PRISE w;
    w.show();

    return a.exec();
}
