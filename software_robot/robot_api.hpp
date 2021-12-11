/**
 * @file robot_api.hpp
 * @brief header de la classe Robot::Api
 * @author arthus DORIATH
 * @date 07/12/2021
 * @version 0.5
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
         * ! this class cannot be copied
         */
        Api& operator=(const Api&) = delete;
            
        /**
         * ! this class cannot be copied
         */
        Api(const Api&) = delete;

        /**
         * @brief fonction qui execute l'ordre recu par le server
         * @param 
         * @return rien
         * @note should be run in async
         */
        void executeOrder(std::queue<char>* queueExec, std::mutex* mutexExec);

        /**
         * @brief fonction qui lis les données des capteurs et les met dans la std::queue
         * @param rien
         * @return rien
         * @note should be run in async
         */
        void readSensorData(std::queue<robot::SensorData>* queueSensor, std::mutex* mutexSensor) const;
    };

} //end namespace robot



#endif //ROBOT_API_HPP