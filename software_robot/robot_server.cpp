/**
 * @file robot_serveur.cpp
 * @brief le code de la classe serveur du robot
 * @author arthus DORIATH
 * @date 24/11/2021
 * @version 0.1
 */



#include "robot_server.hpp"
    

    
/**
 * @brief constructeur du Server
 * @param PORT le port utiliser par le serveur
 */
robot::Server::Server(const int PORT){

    // Création de la socket serveur
    _sd_serveur = socket(AF_INET, SOCK_STREAM, 0);

    // Configuration de la socket, notamment le port d'écoute
    _cfg_serveur.sin_family = AF_INET;
    _cfg_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    _cfg_serveur.sin_port = htons(PORT);

    // Attachement de la socket au port défini
    bind(_sd_serveur, (struct sockaddr *)&_cfg_serveur, sizeof(_cfg_serveur));

    // Création d'une file d'attente de connexion
    listen(_sd_serveur, 5);
}



/**
 * @brief destructeur du Server
 * @param rien
 */
robot::Server::~Server(){

    // Fermeture de la socket serveur
    close(_sd_serveur);
}