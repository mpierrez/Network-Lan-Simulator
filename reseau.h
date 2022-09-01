#pragma once

#include "types.h"
#include <cassert>
#include <functional>
#include <string>

struct reseau
{
    uint32_t nb_equipements = 0;
    vector<station> stations;
    vector<commutateur> commutateurs;
    vector<arete> aretes;
    vector<double> poids;
};

uint32_t nb_equipements(reseau* r);
uint32_t nb_stations(reseau* r);
uint32_t nb_commutateurs(reseau* r);
uint32_t nb_aretes(reseau* r);

equipement ajouter_station(reseau* r, adr_ip ip, adr_mac mac);
equipement ajouter_commutateur(reseau* r, adr_mac mac, unsigned int nbports, unsigned int priorite, vector<adr_mac> table);
bool existe_arete(reseau* r, arete a);
void afficher_mac(station s);
void afficher_mac(commutateur s);
void afficher_ip(station s);
void afficher_trame(trame t);
bool ajouter_arete(reseau* r, arete a);
void affiche(reseau* r);
equipement transformToEquipement(reseau* r, station s);
equipement transformToEquipement(reseau* r, commutateur c);
station* transformToStation(reseau* r, equipement e);
commutateur* transformToCommutateur(reseau* r, equipement e);
bool estUneStation(reseau* r, equipement e);
equipement* findEquipementByMAC(reseau* r, adr_mac mac);
void showMAC(adr_mac mac);

void pour_chaque_arete(reseau* r, function<void(arete)> f);
void pour_chaque_station(reseau* r, function<void(station)> f);
void pour_chaque_commutateur(reseau* r, function<void(commutateur)> f);
trame generer_trame(reseau* r, adr_mac dest, adr_mac src, u_int16_t type, const char* data);
void emission_trame(reseau* r, trame t, equipement dest, equipement src, int num);
void pour_chaque_arete_incidente(reseau* r, station s, function<void(arete)> f);
void pour_chaque_equipement_adjacent(reseau* r, equipement e, function<void(equipement)> f);


double poids(reseau *r, arete a);
void changer_poids(reseau *r, arete a, double p);
