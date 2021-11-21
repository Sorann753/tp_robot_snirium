#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/* @brief Classe de la fenêtre principal du programme */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur automatique de MainWindow
     * @param parent un pointeur sur la classe parent de MainWindow, nullptr par défaut
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructeur
     * @param rien
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; //un pointeur sur l'interface graphique
};

#endif // MAINWINDOW_H