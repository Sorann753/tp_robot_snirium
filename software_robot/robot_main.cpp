/**
 * @file robot_software.cpp
 * @brief le programme principale du robot
 * @author arthus DORIATH
 * @date 25/11/2021
 * @version 0.5
 */



#include <iostream>
#include <cstdlib>

//#include "robot_server.hpp"
//#include "robot_api.hpp"
#include <unistd.h>
#include <cerrno>
#include <sys/un.h>
#include <arpa/inet.h>
#include "robot.hpp"



void 



int main(int argc, char** argv) {

    //si un port est passé en argument on l'utilise, si non par defaut on prend le port 50000
    const int PORT = (argc > 1) ? atoi(argv[1]) : 50'000;

    // Instanciation d'un objet ////Robot::api
    Robot mon_robot;
    
    // Instanciation d'un objet Robot::Server
    //robot::Server Server();

    int _sd_serveur;
    struct sockaddr_in _cfg_serveur;
    int err = 0;

    // Création de la socket serveur
    _sd_serveur = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "STARTING SERVER" << std::endl;

    // Configuration de la socket, notamment le port d'écoute
    _cfg_serveur.sin_family = AF_INET;
    _cfg_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    _cfg_serveur.sin_port = htons(PORT);

    // Attachement de la socket au port défini
    err = bind(_sd_serveur, (struct sockaddr *)&_cfg_serveur, sizeof(_cfg_serveur));

    if(err < 0){
        std::cerr << "ERROR : can't open port " << PORT << std::endl;
        close(_sd_serveur);
        return err;
    }

    // Création d'une file d'attente de connexion
    listen(_sd_serveur, 5);

    int sd_client = -1;
    char buffer[1024];

    std::cout << "SERVER OPENED ON PORT " << PORT << std::endl;

    while(true) {
        // Dès qu’un nouveau client se connecte à notre serveur,
        // une nouvelle socket est créée pour gérer le client
        if(sd_client == -1){
            sd_client = accept(_sd_serveur, NULL, NULL);
        }

        // Réception de la requête du client
        memset(buffer, 0x00, 1024);
        int nbOctets = recv(sd_client, buffer, sizeof(buffer), 0);

        std::string reponse(buffer);
        std::cout << reponse << std::endl;

        mon_robot.emettreSon(1000, 1000, true);

        mon_robot.parler(reponse, true);

        // Envoi de la réponse au client
        std::string requete = "Hello world!";
        send(sd_client, requete.c_str(), requete.size(), 0);

    }

    if(sd_client != -1){

        // Fermeture de la socket client
        close(sd_client);
        sd_client = -1;
    }

    //Robot.parler(argv[1], true);

    // Initialisation des moteurs et des tachymètres à 0
    //Robot.initialiserMoteurs();

    close(_sd_serveur);

    // Fin du programme
    return 0;
}