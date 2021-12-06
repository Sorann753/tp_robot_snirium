/**
 * @file robot_api.hpp
 * @brief header de la classe Robot::Api
 * @author arthus DORIATH
 * @date 29/11/2021
 * @version 0.4
 */



#ifndef ROBOT_API_HPP
#define ROBOT_API_HPP

#include "robot.hpp"
#include <queue>
#include <mutex>



namespace robot{

    /* @brief enum des différentes commandes possibles */
    enum Ordres {

        //mouvements
        FORWARD = 'A',
        BACKWARD = 'R',
        LEFT = 'G',
        RIGHT = 'D',
        STOP = 's',

        //bras robot
        UP = 'H',
        DOWN = 'B',

        //connection
        CONNECT_STOP = 'S', // ferme le client
        CONNECT_CLOSE = 'C' // ferme le server
    };



    /* @brief une structure qui contiens une mesure des capteurs */
    struct SensorData {

        //Fr
    };



    /* @brief la classe pour interagir avec le robot */
    class Api : protected Robot{

    public:

        /**
         * @brief le constructeur de l'API du robot
         * @param rien
         */
        Api();

        /**
         * @brief le destructeur de l'API du robot
         * @param rien
         */
        ~Api();

        /**
         * @brief fonction qui execute l'ordre recu par le server
         * @param ordre l'ordre qui a été envoyé par le server
         * @return rien
         * @note should be run in async
         */
        void executeOrder(std::string ordre);

        /**
         * @brief fonction qui lis les données des capteurs et les met dans la std::queue
         * @param rien
         * @return rien
         * @note should be run in async
         */
        void readSensorData(); const
    };

} //end namespace robot



#endif //ROBOT_API_HPP