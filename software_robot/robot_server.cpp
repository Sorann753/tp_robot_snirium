/**
 * @file robot_serveur.cpp
 * @brief le code de la classe serveur du robot
 * @author arthus DORIATH
 * @date 28/11/2021
 * @version 0.3
 */



#include "robot_server.hpp"



/**
 * @brief constructeur du Server
 * @param api une reference vers l'api du robot
 */
robot::Server::Server(robot::Api& api){

    int PORT = 49'999;
    _connexionUsed = false;
    _Robot = api;

    // Création de la socket serveur
    _sd_serveur = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "STARTING SERVER" << std::endl;

    // Configuration de la socket, notamment le port d'écoute
    _cfg_serveur.sin_family = AF_INET;
    _cfg_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    _cfg_serveur.sin_port = htons(PORT);

    // Attachement de la socket au premier port libre en partant de 50'000
    do{
        _cfg_serveur.sin_port = htons(++PORT);
        err = bind(_sd_serveur, (struct sockaddr *)&_cfg_serveur, sizeof(_cfg_serveur));
    } while(err < 0);

    std::cout << "SERVER OPENED ON PORT " << PORT << std::endl;

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



/**
 * @brief methode qui écoute les transmission qui arrivent sur le serveur
 * @param rien
 * @return rien
 * @note should be run in async
 */
void robot::Server::ecouter(){

    int sd_client;
    char buffer[1024];
    _connexionUsed = false;

    while(true) {
        // si un client se connecte au robot et qu'il n'y a pas encore de client,
        // une nouvelle socket est créée pour gérer le client
        if(_clientConnected.try_lock()){
            sd_client = accept(_sd_serveur, NULL, NULL);
            _connexionUsed = true;
        }

        // Réception de la requête du client
        memset(buffer, 0x00, 1024);
        int nbOctets = recv(sd_client, buffer, sizeof(buffer), 0);

        std::string in_trame(buffer);
        std::cout << "=> " << in_trame << std::endl;

        for(auto& i : in_trame){

            if(i == Ordres::CONNECT_STOP && _connexionUsed){ //si on ressoit un ordre de fermeture du client

                // Fermeture de la socket client
                close(sd_client);
                _connexionUsed = false;

                // ouverture du mutex
                _clientConnected.unlock();
                continue;
            }
            else if(i == Ordres::CONNECT_CLOSE && _connexionUsed){ //si on ressoit un ordre de fermeture du serveur

                // Fermeture de la socket client
                close(sd_client);
                _connexionUsed = false;

                // ouverture du mutex
                _clientConnected.unlock();

                // sortie de la boucle d'écoute
                break;
            }

            _Robot.executeOrder(in_trame)
        }

        // Envoi de la réponse au client
        std::string answer = "OK";
        send(sd_client, answer.c_str(), answer.size(), 0);
    }
}