/**
 * @file robot_serveur.cpp
 * @brief le code de la classe serveur du robot
 * @author arthus DORIATH
 * @date 06/12/2021
 * @version 0.5
 */



#include "robot_server.hpp"



/**
 * @brief constructeur du Server
 * @param rien
 */
robot::Server::Server() {

    int PORT = 49'999;
    int err = 0;
    _connexionUsed = false;

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
 * @param queueExec la queue qui permet de transmettre les ordres
 * @param mutexExec le mutex utilisé pour protegé la queueExec
 * @param queueSensor la queue qui permet de recevoir les données des capteurs
 * @param mutexSensor le mutex utilisé pour protegé la queueSensor
 * @return rien
 * @note should be run in async
 */
void ecouter(std::queue<char>* queueExec, std::mutex* mutexExec,
                     std::queue<robot::SensorData>* queueSensor,
                     std::mutex* mutexSensor)
{
    int sd_client;
    char buffer[1024];

    while(_serverOpen) {
        // si un client se connecte au robot et qu'il n'y a pas encore de client,
        // une nouvelle socket est créée pour gérer le client
        if(!_connexionUsed){
            std::cout << "AWAITING FOR CLIENT" << std::endl;
            sd_client = accept(_sd_serveur, NULL, NULL);
            std::cout << "CLIENT CONECTION" << std::endl;
            _connexionUsed = true;
        }

        // Réception de la requête du client
        memset(buffer, 0x00, 1024);
        int nbOctets = recv(sd_client, buffer, sizeof(buffer), MSG_DONTWAIT);

        if(nbOctets > 0){ //si l'on a reçus une trame on la traite

            std::string in_trame(buffer);
            std::cout << "=> " << in_trame << std::endl;

            for(auto& i : in_trame){

                if(i == Ordres::CONNECT_STOP){ //si on ressoit un ordre de fermeture du client

                    std::cout << "CLIENT DISCONECTION" << std::endl;

                    // Fermeture de la socket client
                    close(sd_client);
                    _connexionUsed = false;

                    break;
                }
                else if(i == Ordres::CONNECT_CLOSE){ //si on ressoit un ordre de fermeture du serveur

                    std::cout << "CLOSING SERVER" << std::endl;

                    // Fermeture de la socket client
                    close(sd_client);
                    _connexionUsed = false;

                    // sortie de la boucle d'écoute
                    _serverOpen = false;
                    break;
                }
                else{

                    //verouille le mutex
                    mutexExec->lock();

                    //add order to queue
                    queueExec->push(i);
                    
                    //déverouille le mutex
                    mutexExec->unlock();
                }
            }
        }

        if(_connexionUsed){ //si l'on est toujours connecter a un client

            //verouille le mutex
            mutexSensor->lock();
            
            if(queueSensor->empty()){ //si la file est vide on saute l'envoie de données a l'IHM
                
                //déverouille le mutex
                mutexSensor->unlock();
                continue;
            }

            //on sort un élément de la file
            robot::SensorData answer = queueSensor->front();
            queueSensor->pop();

            //déverouille le mutex
            mutexSensor->unlock();

            //Formater les données des capteurs
            std::string answer = "OK";
            
            // Envoi des données a l'IHM
            send(sd_client, answer.c_str(), answer.size(), 0);
            std::cout << "<= " << answer << std::endl;
        }
    }
}