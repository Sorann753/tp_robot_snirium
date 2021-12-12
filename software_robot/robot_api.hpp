/**
 * @file robot_api.hpp
 * @brief header de la classe Robot::Api
 * @author arthus DORIATH
 * @date 12/12/2021
 * @version 0.6
 */



#ifndef ROBOT_API_HPP
#define ROBOT_API_HPP

#include "robot.hpp"
#include <queue>
#include <mutex>



namespace robot{

    /**
     * @brief enum des différentes commandes possibles
     */
    enum Ordres {

        //pas d'ordre
        NONE = 0,

        //mouvements
        FORWARD = 'A',
        BACKWARD = 'R',
        LEFT = 'G',
        RIGHT = 'D',
        STOP = 's',

        //bras robot
        UP = 'H',
        DOWN = 'B',

        //server
        CONNECT_STOP = 'S', // ferme le client
        CONNECT_CLOSE = 'C' // ferme le server
    };



    /**
     * @brief enum des différents états possibles du server
     */
    enum State {
    
        STOP = -1,  //server is closed
        WAITING,    //server is waiting for client
        OPEN,       //server is connected to client
    };



    /**
     * @brief une structure qui contiens une mesure des capteurs
     */
    struct SensorData {

        bool isEmpty = true;
        //FR
    };



    /**
     * @brief la classe pour interagir avec le robot
     */
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
         * @brief fonction qui execute les ordres recu par le server
         * @param rien
         * @return rien
         * @note should be run in async
         */
        void executeOrder();



        /**
         * @brief fonction qui lis les données des capteurs et les met dans la std::queue
         * @param rien
         * @return rien
         * @note should be run in async
         */
        void readSensorData();





        /**
         * @brief fonction qui ajoute une valeur dans la queue Sensor
         * @param newData
         * @return rien
         */
        void pushSensorData(robot::SensorData newData);



        /**
         * @brief fonction qui lis une valeur dans la queue Sensor
         * @param rien
         * @return une mesure des capteur
         */
        robot::SensorData fetchSensorData();



        /**
         * @brief fonction qui ajoute une valeur dans la queue Exec
         * @param newOrder
         * @return rien
         */
        void pushOrder(char newOrder);



        /**
         * @brief fonction qui lis une valeur dans la queue Exec
         * @param rien
         * @return un Ordre a executer
         */
        char fetchOrder();



        /**
         * @brief fonction qui lis une valeur dans la queue Exec
         * @param newState le nouvel état du server
         * @return un Ordre a executer
         */
        inline void setServerState(char newState);



    private:

        std::queue<char> _queueExec; // une std::queue qui enregistre les ordres a executé par le robot
        std::mutex _mutexExec; // le mutex qui protège _queueExec

        std::queue<robot::SensorData> _queueSensor; // une std::queue qui enregistre les données des capteurs lues
        std::mutex _mutexSensor; // le mutex qui protège _queueSensor

        char _serverState = robot::State::WAITING; // un char qui indique si le server est connecté a un client
    };

} //end namespace robot



#endif //ROBOT_API_HPP