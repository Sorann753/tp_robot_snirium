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
    // Instanciation de la socket
    tcpSocket = new QTcpSocket(this);

    // Attachement d'un slot qui sera appelé à chaque fois que des données arrivent (mode asynchrone)
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(gerer_donnees()));

    // Idem pour les erreurs
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(afficher_erreur(QAbstractSocket::SocketError)));


    pTimer = new QTimer();
    connect(pTimer, SIGNAL(timeout()), this, SLOT(faire_qqchose()));
}

/**
 * @brief Destructeur
 * @param rien
 */
MainWindow::~MainWindow()
{
    delete ui;
}
    ///////////////////////////////////
    //bouton connexion & deconnexion //
    ///////////////////////////////////

void MainWindow::on_pushButton_connexion_clicked()
{
    //récupération des parametres de connexions
     QString adresse_ip = ui->lineEdit_ip->text();
}

void MainWindow::on_pushButton_deconnexion_clicked()
{

}




    /////////////////////////
    // bouton de direction //
    /////////////////////////



void MainWindow::on_pushButton_avancer_pressed()
{
    //tcpSocket->write(avancer);
    ui->lineEdit_ip->setText(avancer);

}


void MainWindow::on_pushButton_gauche_pressed()
{
    ui->lineEdit_ip->setText(gauche);
}


void MainWindow::on_pushButton_droite_pressed()
{
    ui->lineEdit_ip->setText(droite);
}


void MainWindow::on_pushButton_recule_pressed()
{
    ui->lineEdit_ip->setText(reculer);
}



    //////////////////////////
    // bouton du bras avant //
    //////////////////////////


void MainWindow::on_pushButton_descendre_pressed()
{
    ui->lineEdit_ip->setText(baisser);
}

void MainWindow::on_pushButton_lever_pressed()
{
    ui->lineEdit_ip->setText(lever);
}
