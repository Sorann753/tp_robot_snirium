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
    
        WAITING,    //server is waiting for client
        OPEN,       //server is connected to client
        CLOSED,     //server is closed
    };



    /**
     * @brief une structure qui contiens une mesure des capteurs
     */
    struct SensorData {

        bool isEmpty = true;    //indique si la structure est remplie ou vide
        
        int angle = 0;          //l'orientation du robot en degree

        int angle_Mleft = 0;    //l'angle du moteur gauche en degree
        int angle_Mcenter = 0;  //l'angle du moteur centrale (bras) en degree
        int angle_Mright = 0;   //l'angle du moteur droit en degree

        int light = 0;          //la lumière réfléchie par la surface 0 => clair   100 => sombre
        float dist = 0;         //la distance avec un obstacle en cm

        float batterie = 0;     //le pourcentage de batterie du robot
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
         * @brief methode qui execute les ordres recu par le server
         * @param rien
         * @return rien
         * @note should be run in a thread
         */
        void executeOrder();



        /**
         * @brief methode qui lis les données des capteurs et les met dans la std::queue
         * @param rien
         * @return rien
         * @note should be run in a thread
         */
        void readSensorData();





        /**
         * @brief methode qui ajoute une valeur dans la queue Sensor
         * @param newData
         * @return rien
         */
        void pushSensorData(const robot::SensorData& newData);



        /**
         * @brief methode qui lis une valeur dans la queue Sensor
         * @param rien
         * @return une mesure des capteur ou une mesure vide si la queue est vide
         */
        robot::SensorData fetchSensorData();



        /**
         * @brief methode qui ajoute une valeur dans la queue Exec
         * @param newOrder
         * @return rien
         */
        void pushOrder(char newOrder);



        /**
         * @brief methode qui lis une valeur dans la queue Exec
         * @param rien
         * @return un Ordre a executer
         */
        char fetchOrder();



        /**
         * @brief permet d'indiquer a l'Api que le server a changer d'etat
         * @param newState le nouvel état du server
         * @return rien
         */
        void setServerState(char newState);





        /**
         * @brief methode qui fait avancer le robot
         * @param rien
         * @return rien
         */
        void goForward();



        /**
         * @brief methode qui fait tourné le robot sur la gauche
         * @param rien
         * @return rien
         */
        void turnLeft();



        /**
         * @brief methode qui fait tourné le robot sur la droite
         * @param rien
         * @return rien
         */
        void turnRight();



        /**
         * @brief methode qui fait reculer le robot
         * @param rien
         * @return rien
         */
        void goBackward();



        /**
         * @brief methode qui stope le déplacement robot
         * @param rien
         * @return rien
         */
        void stopMoving();



        /**
         * @brief methode qui fait lever le bras du robot
         * @param rien
         * @return rien
         */
        void leverBras();
        
        
        
        /**
         * @brief methode qui fait baisser le bras du robot
         * @param rien
         * @return rien
         */
        void baisserBras();



        /**
         * @brief methode qui renvoie l'etat du bouton retour
         * @param rien
         * @return true si le bouton est pressé, false si non
         */
        bool getStateBouttonOFF();



    private:

        std::queue<char> _queueExec; // une std::queue qui enregistre les ordres a executé par le robot
        std::mutex _mutexExec; // le mutex qui protège _queueExec

        std::queue<robot::SensorData>* _queueSensor; // une std::queue qui enregistre les données des capteurs lues
        std::mutex _mutexSensor; // le mutex qui protège _queueSensor

        char _serverState = robot::State::WAITING; // un char qui indique si le server est connecté a un client
    };

} //end namespace robot



#endif //ROBOT_API_HPP