#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtNetwork>
#include <QShortcut>

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

private slots:


    void on_pushButton_connexion_clicked();

    void on_pushButton_descendre_pressed();

    void on_pushButton_lever_pressed();

    void on_pushButton_avancer_pressed();

    void on_pushButton_gauche_pressed();

    void on_pushButton_droite_pressed();

    void on_pushButton_recule_pressed();

    void on_pushButton_deconnexion_clicked();

private:
    Ui::MainWindow *ui; //un pointeur sur l'interface graphique
    QTimer *pTimer;
    QTcpSocket *tcpSocket;



    const QByteArray avancer = "A";
    const QByteArray gauche = "G";
    const QByteArray droite = "D";
    const QByteArray reculer = "R";
    const QByteArray lever = "H";
    const QByteArray baisser = "B";
};

#endif // MAINWINDOW_H
