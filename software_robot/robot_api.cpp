/**
 * @file robot_api.cpp
 * @brief code de la classe robot::Api
 * @author arthus DORIATH
 * @date 12/12/2021
 * @version 0.4
 */



#include "robot_api.hpp"



/**
 * @brief le constructeur de l'API du robot
 * @param rien
 */
robot::Api::Api(){ //! potentiellement de la grosse merde, vérifier le fonctionnement de l'héritage des constructeurs
    
	std::cout << "INITIALISATION DU ROBOT" << std::endl;

	// Instanciations dynamiques des objets "moteurs" selon leurs ports de sortie respectifs
	pMoteurGauche = new ev3dev::large_motor("ev3-ports:outA");			// "outA"
	pMoteurCentral = new ev3dev::medium_motor("ev3-ports:outB");        // "outB"
	pMoteurDroit = new ev3dev::large_motor("ev3-ports:outC");			// "outC"

	// Vérifications de la présence des moteurs
	if (!pMoteurGauche->connected())
		std::cout << "ERREUR => Le moteur gauche ne semble pas présent (port A)" << std::endl;

	if (!pMoteurCentral->connected())
		std::cout << "ERREUR => Le moteur central ne semble pas présent (port B)" << std::endl;

	if (!pMoteurDroit->connected())
		std::cout << "ERREUR => Le moteur droit ne semble pas présent (port C)" << std::endl;

	// Initialisations des moteurs
	initialiserMoteurs();
	
	// Vérification de la présence des capteurs
	if (!capteurAngle.connected())
	{
		std::cout << "ERREUR => Le capteur d'angle ne semble pas présent" << std::endl;
	}
	else
	{
		initialiserGyroscope();
	}

	if (!capteurContact.connected())
	{
		std::cout << "ERREUR => Le capteur de contact ne semble pas présent" << std::endl;
	}
	if (!capteurCouleur.connected())
	{
		std::cout << "ERREUR => Le capteur de couleur ne semble pas présent" << std::endl;
	}
	if (!capteurDistance.connected())
	{
		std::cout << "ERREUR => Le capteur de distance ne semble pas présent" << std::endl;
	}

    std::cout << "ROBOT PRET" << std::endl;
}



/**
 * @brief le destructeur de l'API du robot
 * @param rien
 */
robot::Api::~Api(){ //! potentiellement de la merde, vérifier le fonctionnement de l'héritage des destructeurs

    // Destruction des objets "moteurs"
	delete pMoteurGauche;
	delete pMoteurCentral;
	delete pMoteurDroit;
}



/**
 * @brief fonction qui execute les ordres recu par le server
 * @param rien
 * @return rien
 * @note should be run in async
 */
void robot::Api::executeOrder(){


    while(_serverState != robot::State::STOP){

		char ordre = fetchOrder();

		if(ordre == robot::Ordres::NONE){ continue; }

        switch(ordre){

            case robot::Ordres::FORWARD : //si on demande au robot d'avancé

                changerPuissanceMoteurs(100, 0, 100);
            break;

            case robot::Ordres::LEFT : //si on demande au robot de tourné a gauche

                changerPuissanceMoteurs(-50, 0, 50);
                attendre(100);
                changerPuissanceMoteurs(0, 0, 0);
            break;

            case robot::Ordres::RIGHT : //si on demande au robot de tourné a droite

                changerPuissanceMoteurs(50, 0, -50);
                attendre(100);
                changerPuissanceMoteurs(0, 0, 0);
            break;

            case robot::Ordres::BACKWARD : //si on demande au robot de reculé

                emettreSon(1000, 400, false);
                changerPuissanceMoteurs(-100, 0, -100);
            break;

            case robot::Ordres::STOP : //si on demande au robot de s'arrêté

                changerPuissanceMoteurs(0, 0, 0);
            break;



            case robot::Ordres::UP : //si on demande au robot de levez le bras

                pMoteurCentral->set_duty_cycle_sp(100);
                pMoteurCentral->run_direct();
            break;

            case robot::Ordres::DOWN : //si on demande au robot de baisser le bras

                pMoteurCentral->set_duty_cycle_sp(-100);
                pMoteurCentral->run_direct();
            break;
        }
    }
}