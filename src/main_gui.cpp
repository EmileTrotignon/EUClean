//
// Created by emile on 20/05/19.
//

#include "CleanConfig.h"
#include "EuCleanCentralWidget.h"

int main(int argc, char **argv)
{

    // Creates the process
    QApplication app(argc, argv);
    //std::cout << "Created app" << std::endl;
    //GUIWindowMainMenu mainMenu;
    //mainMenu.show();

    //CleanConfig config("EU Clean.cfg");

    QMainWindow mw(nullptr);
    EUCleanCentralWidget cw("EU Clean.cfg");
    mw.setCentralWidget(&cw);
    mw.show();

    return QApplication::exec();
}