#include "StdAfx.h"
#include <QApplication>
#include "Ui/WordCounterWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    WordCounterWindow window;
    window.show();
    return application.exec();
}
