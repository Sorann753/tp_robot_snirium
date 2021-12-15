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
robot::Api::Api(){
    
	_queueSensor = new std::queue<robot::SensorData>();

    std::cout << "ROBOT PRET" << std::endl;
}





/**
 * @brief le destructeur de l'API du robot
 * @param rien
 */
robot::Api::~Api(){

	delete _queueSensor;
}





/**
 * @brief fonction qui execute les ordres recu par le server
 * @param rien
 * @return rien
 * @note should be run in async
 */
void robot::Api::executeOrder(){

    while(_serverState != robot::State::CLOSED){

		char ordre = fetchOrder();

		if(ordre == robot::Ordres::NONE){

			//fait reculé le robot automatiquement si il cogne un mur
			if(recupererEtatCapteurContact()){

				goBackward();
				attendre(500);
				stopMoving();
			}

			attendre(500);
			continue;
		}

		std::cout << "excuting order" << std::endl;
        switch(ordre){
			
            case robot::Ordres::FORWARD : //si on demande au robot d'avancé

				goForward();
            break;

            case robot::Ordres::LEFT : //si on demande au robot de tourné a gauche

                turnLeft();
            break;

            case robot::Ordres::RIGHT : //si on demande au robot de tourné a droite

                turnRight();
            break;

            case robot::Ordres::BACKWARD : //si on demande au robot de reculé

                goBackward();
            break;

            case robot::Ordres::STOP : //si on demande au robot de s'arrêté

                stopMoving();
            break;



            case robot::Ordres::UP : //si on demande au robot de levez le bras

                leverBras();
            break;

            case robot::Ordres::DOWN : //si on demande au robot de baisser le bras

                baisserBras();
            break;
        }

		//fait reculé le robot automatiquement si il cogne un mur
		if(recupererEtatCapteurContact()){

			goBackward();
			attendre(500);
			stopMoving();
		}

		//on ralentis l'execution du thread
		attendre(100);
    }

	stopMoving();
}





/**
 * @brief fonction qui lis les données des capteurs et les met dans la std::queue
 * @param rien
 * @return rien
 * @note should be run in async
 */
void robot::Api::readSensorData(){

	while(_serverState != robot::State::CLOSED){

		if(_serverState == robot::State::WAITING){

			//si il n'y a pas de client on attend 100ms avant de revérifier si un client est arrivé
			attendre(500);
			continue;
		}

		//* le server est connecté a un client

		// une structure qui vas enregistré les donnés des capteur au moment de la mesure
		robot::SensorData Data;

		//lecture des capteurs
		try{
			Data.angle 		 = recupererGyroscopeAngle();

			recupererPositionsDesMoteurs(Data.angle_Mleft, Data.angle_Mcenter, Data.angle_Mright);

			Data.light 		 = recupererLumiereReflechie();
			Data.dist 		 = recupererDistance();
			Data.batterie 	 = recupererBatterieTension();
		}
		catch(std::system_error e){

			std::cerr << "CRITICAL ERROR " << e.code() << " : " << e.what() << std::endl;
			continue;
		}

		//traitement des données
		//on arondis les angles au 360 près
		Data.angle 			%= 360;
		while(Data.angle < 0){ Data.angle += 360; }

		Data.angle_Mleft 	%= 360;
		while(Data.angle_Mleft < 0){ Data.angle_Mleft += 360; }

		Data.angle_Mcenter 	%= 360;
		while(Data.angle_Mcenter < 0){ Data.angle_Mcenter += 360; }

		Data.angle_Mright 	%= 360;
		while(Data.angle_Mright < 0){ Data.angle_Mright += 360; }

		//on inverse les valeurs, 0 => pas de snirium   100 => beaucoup de snirium
		Data.light = (Data.light-100) * (-1);

		//on exprime la batterie en %
		Data.batterie = (Data.batterie - 6.5) * 100.0 / 1.73;

		Data.isEmpty = false;

		//ajout des données dans la queue
		pushSensorData(Data);

		//on ralentis l'execution du thread
		attendre(100);
	}
}





/**
 * @brief fonction qui ajoute une valeur dans la queue Sensor
 * @param newData
 * @return rien
 */
void robot::Api::pushSensorData(const robot::SensorData& newData){

	_mutexSensor.lock();
	_queueSensor->push(newData);
	_mutexSensor.unlock();
}



/**
 * @brief fonction qui lis une valeur dans la queue Sensor
 * @param rien
 * @return une mesure des capteur ou une mesure vide si la queue est vide
 */
robot::SensorData robot::Api::fetchSensorData(){

	_mutexSensor.lock();

	if(_queueSensor->empty()){

		_mutexSensor.unlock();
		return robot::SensorData(); //! faire test unitaire pour vérifier le comportement
	}

	robot::SensorData data = _queueSensor->front();
	_queueSensor->pop();

	_mutexSensor.unlock();

	return data;
}



/**
 * @brief fonction qui ajoute une valeur dans la queue Exec
 * @param newOrder
 * @return rien
 */
void robot::Api::pushOrder(char newOrder){

	_mutexExec.lock();
	_queueExec.push(newOrder);
	_mutexExec.unlock();
}



/**
 * @brief fonction qui lis une valeur dans la queue Exec
 * @param rien
 * @return un Ordre a executer
 */
char robot::Api::fetchOrder(){

	_mutexExec.lock();

	if(_queueExec.empty()){

		_mutexExec.unlock();
		return robot::Ordres::NONE;
	}

	char ordre = _queueExec.front();
	_queueExec.pop();

	_mutexExec.unlock();

	return ordre;
}



/**
 * @brief fonction qui lis une valeur dans la queue Exec
 * @param newState le nouvel état du server
 * @return un Ordre a executer
 */
void robot::Api::setServerState(char newState){

	_serverState = newState;
}





/**
 * @brief fonction qui fait avancer le robot
 * @param rien
 * @return rien
 */
void robot::Api::goForward(){

	changerPuissanceMoteurs(100, 0, 100);	
}



/**
 * @brief fonction qui fait tourné le robot sur la gauche
 * @param rien
 * @return rien
 */
void robot::Api::turnLeft(){

	changerPuissanceMoteurs(-50, 0, 50);
    attendre(100);
    changerPuissanceMoteurs(0, 0, 0);
}



/**
 * @brief fonction qui fait tourné le robot sur la droite
 * @param rien
 * @return rien
 */
void robot::Api::turnRight(){

	changerPuissanceMoteurs(50, 0, -50);
    attendre(100);
    changerPuissanceMoteurs(0, 0, 0);
}



/**
 * @brief fonction qui fait reculer le robot
 * @param rien
 * @return rien
 */
void robot::Api::goBackward(){

	emettreSon(1000, 400, false);
    changerPuissanceMoteurs(-100, 0, -100);
}



/**
 * @brief fonction qui stope le déplacement robot
 * @param rien
 * @return rien
 */
void robot::Api::stopMoving(){

	changerPuissanceMoteurs(0, 0, 0);
}



/**
 * @brief fonction qui fait lever le bras du robot
 * @param rien
 * @return rien
 */
void robot::Api::leverBras(){

	pMoteurCentral->set_duty_cycle_sp(100);
    pMoteurCentral->run_direct();
}



/**
 * @brief fonction qui fait baisser le bras du robot
 * @param rien
 * @return rien
 */
void robot::Api::baisserBras(){

	pMoteurCentral->set_duty_cycle_sp(-100);
	pMoteurCentral->run_direct();
}



/**
 * @brief fonction qui renvoie l'etat du bouton retour
 * @param rien
 * @return true si le bouton est pressé, false si non
 */
bool robot::Api::getStateBouttonOFF(){

	return recupererEtatBoutonRetour();
}