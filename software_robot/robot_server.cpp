/**
 * @file robot_serveur.cpp
 * @brief le code de la classe serveur du robot
 * @author arthus DORIATH
 * @date 12/12/2021
 * @version 0.6
 */



#include "robot_server.hpp"



/**
 * @brief constructeur du Server
 * @param robot_lego l'api du robot
 */
robot::Server::Server(robot::Api& robot_lego) : _apiRobot(robot_lego)
{
    int PORT = 49'999;
    int err = 0;

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

    // Création d'une file d'attente de connexion
    listen(_sd_serveur, 5);

    std::cout << "SERVER OPENED ON PORT " << PORT << std::endl;

    _serverOpen = true;
    _apiRobot.setServerState(robot::State::WAITING);
}



/**
 * @brief destructeur du Server
 * @param rien
 */
robot::Server::~Server(){

    if(_connexionUsed){ //si le client n'a pas été fermé

        // Fermeture de la socket client
        shutdown(_sd_client, SHUT_RDWR);
        close(_sd_client);
    }

    if(_serverOpen){ //si le server n'a pas été fermé

        // Fermeture de la socket serveur
        close(_sd_serveur);
    }
}



/**
 * @brief methode qui écoute les transmission qui arrivent sur le serveur
 * @param rien
 * @return rien
 * @note should be run in async
 */
void robot::Server::ecouter()
{
    char buffer[1024];
    int compteur = 0; //compteur pour rallentir la fréquence de ping
    std::string ping = "!";

    while(_serverOpen) {
        // si un client se connecte au robot et qu'il n'y a pas encore de client,
        // une nouvelle socket est créée pour gérer le client
        if(!_connexionUsed){
            std::cout << "AWAITING FOR CLIENT" << std::endl;
            _sd_client = accept(_sd_serveur, NULL, NULL);
            std::cout << "CLIENT CONNECTION" << std::endl;

            _connexionUsed = true;
            _apiRobot.setServerState(robot::State::OPEN);
        }
        else if(compteur % 10 == 0){ //si un client est connecté on teste qu'il soit toujours joignable

            int err = write(_sd_client, ping.c_str(), ping.size());
            
            if(err < 0){

                std::cerr << "CONNECTION LOST" << std::endl;

                // Fermeture de la socket client
                shutdown(_sd_client, SHUT_RDWR);
                close(_sd_client);
                _connexionUsed = false;
                _apiRobot.setServerState(robot::State::WAITING);

            }
            if(compteur > 1'000'000){ compteur = 0; } //si le compteur est trop haut on le réinitialise
        }

        // Réception de la requête du client
        memset(buffer, 0x00, 1024);
        int nbOctets = recv(_sd_client, buffer, sizeof(buffer), MSG_DONTWAIT);

        if(nbOctets > 0){ //si l'on a reçus une trame on la traite

            std::string in_trame(buffer);
            in_trame = uncrypt(in_trame);
            std::cout << "=> " << in_trame << std::endl;

            for(auto& i : in_trame){

                if(i == Ordres::CONNECT_STOP){ //si on ressoit un ordre de fermeture du client

                    std::cout << "CLIENT DISCONECTION" << std::endl;

                    // Fermeture de la socket client
                    shutdown(_sd_client, SHUT_RDWR);
                    close(_sd_client);
                    _connexionUsed = false;
                    _apiRobot.setServerState(robot::State::WAITING);

                    break;
                }
                else if(i == Ordres::CONNECT_CLOSE){ //si on ressoit un ordre de fermeture du serveur

                    std::cout << "CLOSING SERVER" << std::endl;

                    // Fermeture de la socket client
                    shutdown(_sd_client, SHUT_RDWR);
                    close(_sd_client);
                    _connexionUsed = false;

                    // sortie de la boucle d'écoute
                    _serverOpen = false;
                    _apiRobot.setServerState(robot::State::CLOSED);
                    return;
                }
                else{

                    //add order to queue
                    _apiRobot.pushOrder(i);
                }
            }
        }



        if(_connexionUsed){ //si l'on est toujours connecté a un client

            //on récupère une mesure des capteurs
            robot::SensorData data = _apiRobot.fetchSensorData();

            if(data.isEmpty){ continue; }

            std::stringstream ss;
            ss << "#;" << std::to_string(data.angle) << ";"
                       << std::to_string(data.angle_Mleft) << ";"
                       << std::to_string(data.angle_Mcenter) << ";"
                       << std::to_string(data.angle_Mright) << ";"
                       << std::to_string(data.light) << ";"
                       << std::to_string(data.dist) << ";"
                       << std::to_string(data.batterie);

            std::string answer = ss.str();
            std::cout << "<= " << answer << std::endl;

            //chiffrage des données
            answer = encrypt(answer);
            
            // Envoi des données a l'IHM
            send(_sd_client, answer.c_str(), answer.size(), 0);
        }



        //système d'extinction d'urgence
        if(_apiRobot.getStateBouttonOFF()){

            std::cout << "CLOSING SERVER" << std::endl;

            // Fermeture de la socket client
            if(_connexionUsed){

                shutdown(_sd_client, SHUT_RDWR);
                close(_sd_client);
                _connexionUsed = false;
            }

            // sortie de la boucle d'écoute
            _serverOpen = false;
            _apiRobot.setServerState(robot::State::CLOSED);
            break;
        }

        compteur++;
    }
}