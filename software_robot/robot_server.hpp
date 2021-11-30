/**
 * @file robot_serveur.hpp
 * @brief header de la classe serveur du robot
 * @author arthus DORIATH
 * @date 27/11/2021
 * @version 05
 */



#ifndef ROBOT_SERVER_HPP
#define ROBOT_SERVER_HPP

#include <iostream>
#include <mutex>

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
         * @note should be run in async
         */
        void ecouter();



        /**
         * @brief methode qui permet de crypter une trame
         * @param input_str le string a crypter
         * @return le string qui une fois crypte
         */
        std::string crypt(std::string input_str){

            return input_str;
        }



        /**
         * @brief methode qui permet de decrypter une trame
         * @param input_str le string a decrypter
         * @return le string une fois decrypte
         */
        std::string decrypt(std::string input_str){

            return input_str;
        }



    private :

        int _sd_serveur; // l'identifiant du serveur
        std::mutex _clientConnected; // un mutex pour empêcher de multiples connexions sur le robot
        bool _connexionUsed; // 
        struct sockaddr_in _cfg_serveur; // une structure qui représente la config du serveur
        robot::Api& _Robot; // une reference vers l'api du robot
        
    };

} //end namespace robot



#endif //ROBOT_SERVER_HPP