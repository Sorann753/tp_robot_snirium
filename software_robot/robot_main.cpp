/**
 * @file robot_software.cpp
 * @brief le programme principale du robot
 * @author arthus DORIATH
 * @date 23/11/2021
 * @version 0.3
 */

#include "robot_main.hpp"

int main(int argc, char** argv) {

    const int PORT = (argc > 1) ? atoi(argv[1]) : 50000;

    // Instanciation de l'objet robot
    Robot monRobot;

    // Création de la socket serveur
    int sd_serveur = socket(AF_INET, SOCK_STREAM, 0);

    // Configuration de la socket, notamment le port d'écoute
    struct sockaddr_in cfg_serveur;
    cfg_serveur.sin_family = AF_INET;
    cfg_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
    cfg_serveur.sin_port = htons(PORT);

    // Attachement de la socket au port défini
    bind(sd_serveur, (struct sockaddr *)&cfg_serveur, sizeof(cfg_serveur));

    // Création une file d'attente de connexion
    listen(sd_serveur, 5);

    while(true) {
        // Dès qu’un nouveau client se connecte à notre serveur,
        // une nouvelle socket est créée pour gérer le client
        int sd_client = accept(sd_serveur, NULL, NULL);

        // Réception de la requête du client
        char buffer[1024];
        memset(buffer, 0x00, 1024);
        int nbOctets = recv(sd_client, buffer, sizeof(buffer), 0);

        std::string reponse(buffer);
        std::cout << reponse << std::endl;

        // Envoi de la réponse au client
        std::string requete = "Hello world!";
        send(sd_client, requete.c_str(), requete.size(), 0);

        // Fermeture de la socket client
        close(sd_client);
        //port par défaut

        monRobot.parler(argv[1], true);

        // Initialisation des moteurs et des tachymètres à 0
        monRobot.initialiserMoteurs();

        for(int i = 0; i < 3; i++){

            // Le robot tourne sur lui-même pendant 2s puis s'arrête
            // monRobot.changerPuissanceMoteurs(100, 0, -100);

            // Le robot émet un son de fréquence 440Hz pendant 1000ms
            monRobot.emettreSon(1000, 1000, true);

            // monRobot.attendre(1000);
            // monRobot.changerPuissanceMoteurs(0, 0, 0);
        }
    }

    // Fermeture de la socket serveur
    close(sd_serveur);

    // Fin du programme
    return 0;
}