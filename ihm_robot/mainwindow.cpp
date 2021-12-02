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

     pRobot = new QImage();
     pRobot ->load ("/home/etudiant/Documents/GitHub/tp_robot_snirium/ihm_robot/robot.png");
     ui->label_robot->setPixmap(QPixmap::fromImage(*pRobot));

     pMap = new QImage();
     pMap ->load ("/home/etudiant/Documents/GitHub/tp_robot_snirium/ihm_robot/map.png");
     ui->label_map->setPixmap(QPixmap::fromImage(*pMap));

     //pFont = new QImage();
     //pFont ->load ("/home/etudiant/Documents/image-blanche-800x548px.jpg");
     //ui->label_font->setPixmap(QPixmap::fromImage(*pFont));

    //bdd = QSqlDatabase::addDatabase("QSQLITE");

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

     // Récupération des paramètres
     QString adresse_ip = ui->lineEdit_ip->text();
     unsigned short port_tcp = ui->lineEdit_port->text().toInt();

     // Connexion au serveur
     tcpSocket->connectToHost(adresse_ip, port_tcp);

     // Lancement du timer avec un tick toutes les 1000 ms
     pTimer->start(1000);

}

void MainWindow::on_pushButton_deconnexion_clicked()
{
    // Déconnexion du serveur
    tcpSocket->close();
}




    /////////////////////////
    // bouton de direction //
    /////////////////////////



void MainWindow::on_pushButton_avancer_pressed()
{
    tcpSocket->write(avancer);
    placer_robot();
    damage(100.0);
}


void MainWindow::on_pushButton_gauche_pressed()
{
    tcpSocket->write(gauche);

    angle = (angle - 15) % 360;
    ui->label_robot->setPixmap(QPixmap::fromImage(pRobot->transformed(QTransform().rotate(angle))));
}


void MainWindow::on_pushButton_droite_pressed()
{
    tcpSocket->write(droite);

    angle = (angle + 15) % 360;
    ui->label_robot->setPixmap(QPixmap::fromImage(pRobot->transformed(QTransform().rotate(angle))));
}


void MainWindow::on_pushButton_recule_pressed()
{
    tcpSocket->write(reculer);
    ui->label_robot->move(ui->label_robot->x(), ui->label_robot->y() + 5);

}



    //////////////////////////
    // bouton du bras avant //
    //////////////////////////


void MainWindow::on_pushButton_descendre_pressed()
{
    tcpSocket->write(baisser);

}

void MainWindow::on_pushButton_lever_pressed()
{
    tcpSocket->write(lever);

}




    /////////////////////////////////
    //connexion à la base de donnée//
    /////////////////////////////////







    /////////////////////////////////
    //    reception des trames     //
    /////////////////////////////////

void MainWindow::gerer_donnees()
{
    // Réception des données
    QByteArray reponse = tcpSocket->readAll();
    QString trame(reponse);

    //décodage
    QStringList trameDecoupee = trame.split(',');
    if(trameDecoupee.size() < 3){

        return;
    }



    //progressebar batterie

    //float tauxbatterie = **************;
    //ui->progressBar_batterie->setValue(tauxbatterie);



    //progressBar_snirium

    //float tauxsnirium = **************;
    //ui->progressBar_snirium->setValue(tauxsnirium);





    /////////////////////////////////
    //       dessin du robot       //
    /////////////////////////////////



}

void MainWindow::placer_robot()
{

    ui->label_robot->move(ui->label_robot->x(), ui->label_robot->y() - 5);
    ui->Edit_position->setText(QString::number(ui->label_robot->x()));
}



void MainWindow::damage(float distance)
{

    //QRectF rectangle(x, y, 80.0, 60.0);

    //QPainter painter(pMap);

    //QPen stilot(Qt::black);
    //painter.setPen(stilot);
    //painter.drawEllipse(rectangle);

    //ui->label_map->setPixmap(QPixmap::fromImage(*pMap));

    int x_robot = ui->label_robot->x();
    int y_robot = ui->label_robot->y();
    float angle_robot = angle - 90;


    QLineF line(x_robot, y_robot, x_robot + (distance * cos(angle_robot*3.1415/180.0)), y_robot + (distance * sin(angle_robot*3.1415/180.0)) );
    QPainter painter(pMap);
    QPen stilot(Qt::white);
    stilot.setWidth(5);;
    painter.setPen(stilot);
    painter.drawLine(line);

    ui->label_map->setPixmap(QPixmap::fromImage(*pMap));
}







