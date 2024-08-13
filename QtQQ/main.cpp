#include "CCMainWindow.h"
#include "titlebar.h"
#include "basicwindow.h"
#include "UserLogin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//     CCMainWindow w;
//     w.show();
    UserLogin* userlogin = new UserLogin;
    userlogin->show();
    return a.exec();
}
