#include "reseau.h"
#include "config.h"

#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

int main()
{
    printf("BIENVENUE A LA SIMULATION RESEAU DE LA SAE\n");

    // Création du réseau à partir du fichier "mylan.lan"
    reseau r;
    createReseau(&r, "mylan.lan");

    // Affichage du réseau
    affiche(&r);

    // Création du réseau à partir du fichier "mylan_no_cycle.lan"
    readConfigFile(&r, "mylan_no_cycle.lan");

    // Paramétrage des adresses mac
    adr_mac mac_st1 = {"54","d6","a6","82","c5","01"};
    adr_mac mac_st8 = {"54","d6","a6","82","c5","08"};

    // Génération de la trame entre st1 et st8
    trame t = generer_trame(&r, mac_st8, mac_st1, 0x800, "Bonjour, c'est un joli test !");

    // Emission de la trame depuis la station 1
    emission_trame(&r, t, *findEquipementByMAC(&r, mac_st1), *findEquipementByMAC(&r, mac_st1),1);
    affiche(&r);


    // Génération de la trame entre st8 et st1
    t = generer_trame(&r, mac_st1, mac_st8, 0x800, "Re-bonjour, voici le second test !");

    // Emission de la trame depuis la station 8
    emission_trame(&r, t, *findEquipementByMAC(&r, mac_st8), *findEquipementByMAC(&r, mac_st8),1);

    affiche(&r);
    // Fin du programme
    printf("\n\nSortie du programme! Merci de votre visite\n");
    return 0;
}
