#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief Constructeur automatique de MainWindow
 * @param parent un pointeur sur la classe parent de MainWindow, nullptr par défaut
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * @brief Destructeur
 * @param rien
 */
MainWindow::~MainWindow()
{
    delete ui;
}

