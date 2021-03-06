#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QtNetwork>
#include <QShortcut>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

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

    void updateData(const QStringList& newData);

private slots:


    void on_pushButton_connexion_clicked();

    void on_pushButton_descendre_pressed();

    void on_pushButton_lever_pressed();

    void on_pushButton_avancer_pressed();

    void on_pushButton_gauche_pressed();

    void on_pushButton_droite_pressed();

    void on_pushButton_recule_pressed();

    void on_pushButton_deconnexion_clicked();

    void on_pushButton_stop_pressed();

    void gerer_donnees();

    void placer_robot();

    void tracerRayon(float distance);

private:
    Ui::MainWindow *ui; //un pointeur sur l'interface graphique
    QTimer *pTimer;
    QImage * pRobot; //pointeur sur l'image du robot
    QImage* pMap;
    QImage* pLogo; //pointer sur l'image "spaceX"
    QImage* pFond; //pointeur sur l'image de fond
    QTcpSocket *tcpSocket;
    
    int angle = 0; //l'angle en degree
    const QString _key = "davidLeBG"; //mot de passe du cryptage 


    QString encrypt(QString in_str);

    QString uncrypt(QString in_str);

    
    const QByteArray avancer = "A";
    const QByteArray gauche = "G";
    const QByteArray droite = "D";
    const QByteArray reculer = "R";
    const QByteArray lever = "H";
    const QByteArray baisser = "B";
    const QByteArray stop = "s";
    const QByteArray deconnection = "S";
};

#endif // MAINWINDOW_H
