/**
 * @file robot_serveur.hpp
 * @brief header de la classe serveur du robot
 * @author arthus DORIATH
 * @date 26/11/2021
 * @version 0.4
 */



#ifndef ROBOT_SERVER_HPP
#define ROBOT_SERVER_HPP

#include <iostream>

#include <unistd.h>
#include <cerrno>
#include <sys/un.h>
#include <arpa/inet.h>

#include "robot_api.hpp"



namespace robot {

/* @brief la classe qui gère les communication TCP entre le robot et l'IHM */
class Server {

public :

    /**
     * @brief constructeur du Server
     * @param api une reference vers l'api du robot
     */
    Server(robot::Api& api);



    /**
     * @brief destructeur du Server
     * @param rien
     */
    ~Server();



    /**
     * @brief methode qui écoute les transmission qui arrivent sur le serveur
     * @param rien
     * @return rien
     */
    void ecouter(){

        int sd_client = -1;
        char buffer[1024];

        while(true) {
            // Dès qu’un nouveau client se connecte à notre serveur,
            // une nouvelle socket est créée pour gérer le client
            if(sd_client == -1){
                sd_client = accept(_sd_serveur, NULL, NULL);
            }

            // Réception de la requête du client
            memset(buffer, 0x00, 1024);
            int nbOctets = recv(sd_client, buffer, sizeof(buffer), 0);

            std::string trame(buffer);
            std::cout << "=> " << trame << std::endl;

            _Robot.executeOrder(trame)

            // Envoi de la réponse au client
            std::string requete = "Hello world!";
            send(sd_client, requete.c_str(), requete.size(), 0);

            if(sd_client != -1){

                // Fermeture de la socket client
                close(sd_client);
            }

        }
    }



    /**
     * @brief methode qui permet de crypter une trame
     * @param input_str le string a crypter
     * @return rien
     */
    void crypt(std::string input_str){

        std::cout << input_str << std::endl;
    }



    /**
     * @brief methode qui permet de decrypter une trame
     * @param input_str le string a decrypter
     * @return rien
     */
    void decrypt(std::string input_str){

        std::cout << input_str << std::endl;
    }



private :

    int _sd_serveur; // l'identifiant du serveur
    struct sockaddr_in _cfg_serveur; // une structure qui représente la config du serveur
    robot::Api& _Robot; // une reference vers l'api du robot
    
};

} //end namespace robot



#endif //ROBOT_SERVER_HPP