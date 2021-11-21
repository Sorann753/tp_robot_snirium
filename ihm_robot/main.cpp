#include "mainwindow.h"
#include <QApplication>

/**
 * @brief point d'entrée du programme
 * @param argc le nombre d'arguments passés au programme
 * @param argv les arguments passés au programme
 * @return un code d'erreur
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
