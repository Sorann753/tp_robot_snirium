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

     pRobot = new QImage();
     pRobot ->load ("/home/etudiant/Documents/GitHub/tp_robot_snirium/ihm_robot/images/robot.png");
     ui->label_robot->setPixmap(QPixmap::fromImage(*pRobot));

     pMap = new QImage();
     pMap ->load ("/home/etudiant/Documents/GitHub/tp_robot_snirium/ihm_robot/images/map.png");
     ui->label_map->setPixmap(QPixmap::fromImage(*pMap));
      
     pLogo = new QImage();
     pLogo -> load("/home/etudiant/Bureau/spacex-logo.png");
     ui->label_logo->setPixmap(QPixmap::fromImage(*pLogo));

     pFond = new QImage();
     pFond -> load("/home/etudiant/Bureau/FOND-UNIVERS-2.jpg");
     ui->label_fond->setPixmap(QPixmap::fromImage(*pFond));
}

/**
 * @brief Destructeur
 * @param rien
 */
MainWindow::~MainWindow()
{
    tcpSocket->write(encrypt(deconnection).toUtf8());
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
}

void MainWindow::on_pushButton_deconnexion_clicked()
{
    // Déconnexion du serveur
    tcpSocket->write(encrypt(deconnection).toUtf8());
    tcpSocket->close();
}




    /////////////////////////
    // bouton de direction //
    /////////////////////////
void MainWindow::on_pushButton_stop_pressed()
{
    tcpSocket->write(encrypt(stop).toUtf8());
}


void MainWindow::on_pushButton_avancer_pressed()
{
    tcpSocket->write(encrypt(avancer).toUtf8());
    placer_robot();
}


void MainWindow::on_pushButton_gauche_pressed()
{
    tcpSocket->write(encrypt(gauche).toUtf8());
}


void MainWindow::on_pushButton_droite_pressed()
{
    tcpSocket->write(encrypt(droite).toUtf8());
}


void MainWindow::on_pushButton_recule_pressed()
{
    tcpSocket->write(encrypt(reculer).toUtf8());
    ui->label_robot->move(ui->label_robot->x(), ui->label_robot->y() + 5);

}



    //////////////////////////
    // bouton du bras avant //
    //////////////////////////


void MainWindow::on_pushButton_descendre_pressed()
{
    tcpSocket->write(encrypt(baisser).toUtf8());

}

void MainWindow::on_pushButton_lever_pressed()
{
    tcpSocket->write(encrypt(lever).toUtf8());

}




    /////////////////////////////////
    //connexion à la base de donnée//
    /////////////////////////////////







    /////////////////////////////////
    //    reception des trames     //
    /////////////////////////////////

QString MainWindow::encrypt(QString in_str){

    QString out_str = "";

    for(int i = 0; i < in_str.size(); i++){

        out_str += in_str[i].toLatin1() ^ _key[i % _key.size()].toLatin1();
    }

    return out_str;
}

QString MainWindow::uncrypt(QString in_str){

    QString out_str = "";

    for(int i = 0; i < in_str.size(); i++){

        out_str += in_str[i].toLatin1() ^ _key[i % _key.size()].toLatin1();
    }

    return out_str;
}



void MainWindow::updateData(const QStringList& newData){

    qDebug() << "receiving data" << Qt::endl;
    int i = 0;
    for(auto& data : newData){

        qDebug() << "=> " << i << " : " << data << Qt::endl;
        i++;
    }
    qDebug() << "end data" << Qt::endl;

    angle = newData[1].toInt();
    ui->lineEdit_angle->setText(QString(angle));
    ui->label_robot->setPixmap(QPixmap::fromImage(pRobot->transformed(QTransform().rotate(angle))));

    int tauxSnirium = newData[5].toInt();
    ui->progressBar_snirium->setValue(tauxSnirium);

    if(tauxSnirium > 50){

        QPainter painter(pMap);
        QPen stilot(Qt::green);
        QBrush pinceau(Qt::green);
        painter.setBrush(pinceau);
        painter.setPen(stilot);
        painter.drawEllipse(ui->label_robot->x(), ui->label_robot->y(), 15, 15);
        ui->label_map->setPixmap(QPixmap::fromImage(*pMap));
    }

    float distance = newData[6].toFloat();
    ui->lineEdit_distance->setText(newData[6]);
    tracerRayon(distance);
    ui->progressBar_batterie->setValue(newData[7].toFloat());
}



void MainWindow::gerer_donnees()
{
    // Réception des données
    QByteArray reponse = tcpSocket->readAll();
    QString trame(reponse);
    trame = uncrypt(trame);

    //décodage
    QStringList trameDecoupee = trame.split(';');
    if(trameDecoupee.size() < 8){

        return;
    }

    if(trameDecoupee[0].contains("#")){
        updateData(trameDecoupee);
    }
}





    /////////////////////////////////
    //       dessin du robot       //
    /////////////////////////////////





void MainWindow::placer_robot()
{

    ui->label_robot->move(ui->label_robot->x(), ui->label_robot->y() - 5);
    ui->Edit_position->setText(QString::number(ui->label_robot->x()));
}



void MainWindow::tracerRayon(float distance)
{
    int x_robot = ui->label_robot->x() + ui->label_robot->width()/5;
    int y_robot = ui->label_robot->y() + ui->label_robot->height()/2;
    float angle_robot = angle - 90;


    QLineF line(x_robot, y_robot, x_robot + (distance * cos(angle_robot*3.1415/180.0)), y_robot + (distance * sin(angle_robot*3.1415/180.0)) );
    QPainter painter(pMap);
    QPen stilot(Qt::white);
    stilot.setWidth(5);
    painter.setPen(stilot);
    painter.drawLine(line);

    ui->label_map->setPixmap(QPixmap::fromImage(*pMap));
}







