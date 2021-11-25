/**
 * @file robot.cpp
 * @brief Implémentation de la classe Robot
 * @author David SALLE
 * @date 25/02/2010
 * @version 0.2
 *
 * Cette classe Robot formera une sur-couche à l'ensemble des classes ev3dev
 * et facilitera ainsi le développement des programmes pour les "challenges"
 */

// Librairies utilisées
#include "robot.hpp"


//==============================================================================
// Méthodes constructeur et desctructeur
//==============================================================================
Robot::Robot()
{
	
	// Instanciations dynamiques des objets "moteurs" selon leurs ports de sortie respectifs
	pMoteurGauche = new ev3dev::large_motor("ev3-ports:outA");			// "outA"
	pMoteurCentral = new ev3dev::medium_motor("ev3-ports:outB");	// "outB"
	pMoteurDroit = new ev3dev::large_motor("ev3-ports:outC");			// "outC"

	// Vérifications de la présence des moteurs
	if (pMoteurGauche->connected() == false)
	{
		std::cout << "ERREUR => Le moteur gauche ne semble pas présent (port A)" << std::endl;
	}
	if (pMoteurCentral->connected() == false)
	{
		std::cout << "ERREUR => Le moteur central ne semble pas présent (port B)" << std::endl;
	}
	if (pMoteurDroit->connected() == false)
	{
		std::cout << "ERREUR => Le moteur droit ne semble pas présent (port C)" << std::endl;
	}

	// Initialisations des moteurs
	initialiserMoteurs();
	
	// Vérification de la présence des capteurs
	if (capteurAngle.connected() == false)
	{
		std::cout << "ERREUR => Le capteur d'angle ne semble pas présent" << std::endl;
	}
	else
	{
		initialiserGyroscope();
	}

	if (capteurContact.connected() == false)
	{
		std::cout << "ERREUR => Le capteur de contact ne semble pas présent" << std::endl;
	}
	if (capteurCouleur.connected() == false)
	{
		std::cout << "ERREUR => Le capteur de couleur ne semble pas présent" << std::endl;
	}
	if (capteurDistance.connected() == false)
	{
		std::cout << "ERREUR => Le capteur de distance ne semble pas présent" << std::endl;
	}
}


Robot::~Robot()
{
	// Destruction des objets "moteurs"
	delete pMoteurGauche;
	delete pMoteurCentral;
	delete pMoteurDroit;
}


//==============================================================================
// Méthodes utilitaires
//==============================================================================
void Robot::attendre(int delai_en_millisecondes)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(delai_en_millisecondes));
}


//==============================================================================
// Méthodes pour gérer le gyroscope
//==============================================================================
inline int Robot::recupererGyroscopeAngle()
{
	return capteurAngle.angle();
}


inline int Robot::recupererGyroscopeVitesse()
{
	return capteurAngle.rate();
}


void Robot::initialiserGyroscope()
{
	capteurAngle.set_mode(capteurAngle.mode_gyro_cal);
	capteurAngle.set_mode(capteurAngle.mode_gyro_ang);
}


//==============================================================================
// Méthodes pour gérer le capteur de contact
//==============================================================================
inline bool Robot::recupererEtatCapteurContact()
{
	return capteurContact.is_pressed();
}


//==============================================================================
// Méthodes pour gérer le capteur de couleur
//==============================================================================
inline int Robot::recupererLumiereReflechie()
{
	return capteurCouleur.reflected_light_intensity();
}

inline int Robot::recupererLumiereAmbiante()
{
	return capteurCouleur.ambient_light_intensity();
}

inline int Robot::recupererCouleurCode()
{
	return capteurCouleur.color();
}

inline int Robot::recupererCouleurRouge()
{
	return capteurCouleur.red();
}

inline int Robot::recupererCouleurVerte()
{
	return capteurCouleur.green();
}

inline int Robot::recupererCouleurBleue()
{
	return capteurCouleur.blue();
}


//==============================================================================
// Méthodes pour gérer le capteur de distance (ultrason)
//==============================================================================
inline float Robot::recupererDistance()
{
	return capteurDistance.distance_centimeters();
}


//==============================================================================
// Méthodes pour gérer la batterie
//==============================================================================
inline float Robot::recupererBatterieTension()
{
	return ev3dev::power_supply::battery.measured_volts();
}

inline float Robot::recupererBatterieIntensite()
{
	return ev3dev::power_supply::battery.measured_amps();
}


//==============================================================================
// Méthodes pour gérer l'écran LCD
//==============================================================================
void Robot::allumerPixel(int x, int y)
{
	if ( (x >= 0) && (x < 178) && (y >= 0) && (y < 128) )
	{
		unsigned char *pFB = ecran.frame_buffer();
		pFB[(x / 8) + (y * 24)] = 1 << (x % 8);
	}
}

void Robot::eteindrePixel(int x, int y)
{
	if ( (x >= 0) && (x < 178) && (y >= 0) && (y < 128) )
	{
		unsigned char *pFB = ecran.frame_buffer();
		pFB[(x / 8) + (y * 24)] = 0 << (x % 8);
	}
}


//==============================================================================
// Méthodes pour gérer les boutons
//==============================================================================
inline bool Robot::recupererEtatBoutonHaut()
{
	return ev3dev::button::up.pressed();
}

inline bool Robot::recupererEtatBoutonBas()
{
	return ev3dev::button::down.pressed();
}

inline bool Robot::recupererEtatBoutonDroite()
{
	return ev3dev::button::right.pressed();
}

inline bool Robot::recupererEtatBoutonGauche()
{
	return ev3dev::button::left.pressed();
}

inline bool Robot::recupererEtatBoutonCentral()
{
	return ev3dev::button::enter.pressed();
}

inline bool Robot::recupererEtatBoutonRetour()
{
	return ev3dev::button::back.pressed();
}


//==============================================================================
// Méthodes pour gérer les leds
//==============================================================================
void Robot::allumerLed(Emplacement e, Couleur c)
{
	if ( (e == GAUCHE) && (c == ROUGE) )
	{
		ev3dev::led::red_left.on();
	}
	if ( (e == GAUCHE) && (c == VERTE) )
	{
		ev3dev::led::green_left.on();
	}
	if ( (e == DROITE) && (c == ROUGE) )
	{
		ev3dev::led::red_right.on();
	}
	if ( (e == DROITE) && (c == VERTE) )
	{
		ev3dev::led::green_right.on();
	}
}

void Robot::eteindreLed(Emplacement e, Couleur c)
{
	if ( (e == GAUCHE) && (c == ROUGE) )
	{
		ev3dev::led::red_left.off();
	}
	if ( (e == GAUCHE) && (c == VERTE) )
	{
		ev3dev::led::green_left.off();
	}
	if ( (e == DROITE) && (c == ROUGE) )
	{
		ev3dev::led::red_right.off();
	}
	if ( (e == DROITE) && (c == VERTE) )
	{
		ev3dev::led::green_right.off();
	}
}


//==============================================================================
// Méthodes pour gérer le son
//==============================================================================
void Robot::emettreSon(float frequence, float duree, bool synchrone)
{
	ev3dev::sound::tone(frequence, duree, synchrone);
}

void Robot::parler(std::string phrase, bool synchrone)
{
	ev3dev::sound::speak(phrase, synchrone);
}

void Robot::lireFichierSon(std::string nomFichier, bool synchrone)
{
	ev3dev::sound::play(nomFichier, synchrone);
}


//==============================================================================
// Méthodes pour gérer les moteurs
//==============================================================================
void Robot::initialiserMoteurs()
{
	// Ré-initialisation des 3 moteurs
	pMoteurGauche->reset();
	pMoteurCentral->reset();
	pMoteurDroit->reset();
}

void Robot::changerPuissanceMoteurs(int puissanceGauche, int puissanceCentrale, int puissanceDroite)
{
	// Seuillage des puissances (au cas où)
	if (puissanceGauche < -100)
	{
		puissanceGauche = -100;
	}
	if (puissanceCentrale < -100)
	{
		puissanceCentrale = -100;
	}
	if (puissanceDroite < -100)
	{
		puissanceDroite = -100;
	}
	if (puissanceGauche > 100)
	{
		puissanceGauche = 100;
	}
	if (puissanceCentrale > 100)
	{
		puissanceCentrale = 100;
	}
	if (puissanceDroite > 100)
	{
		puissanceDroite = 100;
	}

	// Modification de la puissance des moteurs...
	pMoteurGauche->set_duty_cycle_sp(puissanceGauche);
	pMoteurCentral->set_duty_cycle_sp(puissanceCentrale);
	pMoteurDroit->set_duty_cycle_sp(puissanceDroite);

	// ...et application
	pMoteurGauche->run_direct();
	pMoteurDroit->run_direct();
	pMoteurCentral->run_direct();
}

int Robot::recupererPositionDuMoteur(Emplacement e)
{
	if (e == GAUCHE)
	{
		return pMoteurGauche->position();
	}
	else if (e == CENTRE)
	{
		return pMoteurCentral->position();
	}
	else if (e == DROITE)
	{
		return pMoteurDroit->position();
	}
}

void Robot::recupererPositionsDesMoteurs(int &positionGauche, int &positionCentre, int &positionDroite)
{
	positionGauche = pMoteurGauche->position();
	positionCentre = pMoteurCentral->position();
	positionDroite = pMoteurDroit->position();
}
