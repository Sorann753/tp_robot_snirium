/**
 * @file robot_software.cpp
 * @brief le programme principale du robot
 * @author arthus DORIATH
 * @date 25/11/2021
 * @version 0.5
 */



#include <cstdlib>
#include <iostream>

// #include "robot.hpp"
#include "robot_api.hpp"
#include "robot_server.hpp"



//batterie 6.5 => 0%
//batterie ~8.23 => 100%
// (X - 6.5) * 100 / 1.73



/**
 * @brief fonction d'entrée du programme
 * @param argc le nombre d'argument passé en lignes de commande
 * @param argv les arguments passés en lignes de commande
 * @return un code d'erreur
 */
int main(int argc, char** argv) {

    // Instanciation d'un objet robot::Api
    robot::Api robot_lego;
    
    // Instanciation d'un objet robot::Server
    robot::Server Server(mon_robot);


    //lancement des threads



    // Fin du programme
    return 0;
}