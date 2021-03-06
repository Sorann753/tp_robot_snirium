/**
 * @file robot_software.cpp
 * @brief le programme principale du robot
 * @author arthus DORIATH
 * @date 12/12/2021
 * @version 0.7
 */



#include <cstdlib>
#include <iostream>
#include <queue>
#include <mutex>
#include <signal.h>

#include "robot_api.hpp"
#include "robot_server.hpp"



/**
 * @brief fonction d'entrée du programme
 * @param argc le nombre d'argument passé en lignes de commande
 * @param argv les arguments passés en lignes de commande
 * @return un code d'erreur
 */
int main(int argc, char** argv) {

    //empèche le programme de ce fermé lors d'une deconnection inprévus du client
    signal(SIGPIPE,SIG_IGN);

    // Instanciation d'un objet robot::Api
    robot::Api robot_lego;
    
    // Instanciation d'un objet robot::Server
    robot::Server Server(robot_lego);



    //lancement des threads
    std::thread server_thread (&robot::Server::ecouter, &Server);
    std::thread executing_thread (&robot::Api::executeOrder, &robot_lego);
    std::thread sensor_thread (&robot::Api::readSensorData, &robot_lego);



    server_thread.join();
    executing_thread.join();
    sensor_thread.join();


    // Fin du programme
    return 0;
}