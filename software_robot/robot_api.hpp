/**
 * @file robot_api.hpp
 * @brief header de la classe Robot::Api
 * @author arthus DORIATH
 * @date 25/11/2021
 * @version 0.2
 */



#ifndef ROBOT_API_HPP
#define ROBOT_API_HPP

#include "robot.hpp"



namespace robot{

/* @brief la classe pour interagir avec le robot */
class Api : public Robot{

public:

    /**
     * @brief fonction qui execute l'ordre recu par le server
     * @param ordre l'ordre qui a été envoyé par le server
     * @return rien
     */
    void executeOrder(std::string ordre){

        if(ordre == "A"){ //si on demande au robot d'avancé

            changerPuissanceMoteurs(100, 0, 100);
            attendre(500);
            changerPuissanceMoteurs(0, 0, 0);
        }

        else if(ordre == "G"){ //si on demande au robot de tourné a gauche

            changerPuissanceMoteurs(-50, 0, 50);
            attendre(100);
            changerPuissanceMoteurs(0, 0, 0);
        }

        else if(ordre == "D"){ //si on demande au robot de tourné a droite

            changerPuissanceMoteurs(50, 0, -50);
            attendre(100);
            changerPuissanceMoteurs(0, 0, 0);
        }

        else if(ordre == "R"){ //si on demande au robot de reculé

            emettreSon(1000, 400, false);
            changerPuissanceMoteurs(-100, 0, -100);
            attendre(500);
            changerPuissanceMoteurs(0, 0, 0);
        }
    }
};

} //end namespace robot



#endif //ROBOT_API_HPP