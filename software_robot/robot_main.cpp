/**
 * @file robot_software.cpp
 * @brief le programme principale du robot
 * @author arthus DORIATH
 * @date 25/11/2021
 * @version 0.5
 */



#include <cstdlib>
#include <iostream>

#include <cerrno>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>

#include "robot.hpp"
//#include "robot_api.hpp"
//#include "robot_server.hpp"



//batterie 6.5 => 0%
//batterie ~8.23 => 100%
// (X - 6.5) * 100 / (8.23 - 6.5)



/**
 * @brief fonction d'entrée du programme
 * @param argc le nombre d'argument passé en lignes de commande
 * @param argv les arguments passés en lignes de commande
 * @return un code d'erreur
 */
int main(int argc, char** argv) {

    //par defaut on prend le port 50'000
    unsigned int PORT = 49'999;

    // Instanciation d'un objet ////Robot::api
    robot::Api robot_lego;

    // Initialisation des moteurs et des tachymètres à 0
    mon_robot.initialiserMoteurs();
    
    // Instanciation d'un objet Robot::Server
    //robot::Server Server(PORT, mon_robot);

    int _sd_serveur;
    struct sockaddr_in _cfg_serveur;
    int err = 0;

    // Création de la socket serveur
    _sd_serveur = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "STARTING SERVER" << std::endl;

    // Configuration de la socket, notamment le port d'écoute
    _cfg_serveur.sin_family = AF_INET;
    _cfg_serveur.sin_addr.s_addr = htonl(INADDR_ANY);

    // Attachement de la socket au premier port libre en partant de 50'000
    do{
        _cfg_serveur.sin_port = htons(++PORT);
        err = bind(_sd_serveur, (struct sockaddr *)&_cfg_serveur, sizeof(_cfg_serveur));
    } while(err < 0);

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

        //mon_robot.emettreSon(1000, 1000, true);

        //mon_robot.parler(reponse, true);

        // Envoi de la réponse au client
        std::string requete = reponse;
        send(sd_client, requete.c_str(), requete.size(), 0);
    }

    if(sd_client != -1){

        // Fermeture de la socket client
        close(sd_client);
        sd_client = -1;
    }

    //Robot.parler(argv[1], true);

    close(_sd_serveur);

    // Fin du programme
    return 0;
}