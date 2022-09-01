#include "reseau.h"
#include "types.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ostream>

void affiche(reseau* r)
{
  printf("Nombre d'équipements: %d\n\n", nb_equipements(r));
  printf("Nombre de stations: %d\n", nb_stations(r));
  printf("(<num de la station> : <adresse IP> | <adresse MAC>)\n\n");
  int num = 1;
  pour_chaque_station(r, [&](station s)
  {
    printf("\t➢ Station n°%d (#%d) : ", num, s.numero);
    afficher_ip(s);
    printf(" | ");
    afficher_mac(s);
    num++;
  });
  printf("\nNombre de commutateurs: %d\n", nb_commutateurs(r));
  printf("(<num du commutateur> : <adresse MAC> | <nombre de ports> | <priorité>)\n\n");
  num = 1;
  pour_chaque_commutateur(r, [&](commutateur c)
  {
    printf("\t➢ Commutateur n°%d (#%d) : ", num, c.numero);
    afficher_mac(c);
    printf(" | %d | %d\n", c.nbports, c.priorite);
    printf("\t  Etat de la table de commutation:\n");
    if(c.table.size() == 0)
    {
      printf("\t\t➢ Vide.\n\n");
    } else {
      for(int i = 0; i<c.table.size(); i++)
      {
        equipement e = *findEquipementByMAC(r, c.table[i]);
        printf("\t\t➢ Station %d : ", e-nb_commutateurs(r));
        showMAC(c.table[i]);
      }
      printf("\n");
    }
    num++;
  });
  printf("\nNombre d'arêtes: %d\n", nb_aretes(r));
  pour_chaque_arete(r, [&](arete a)
  {
    printf("\t➢ Liaison entre l'équipement %d et l'équipement %d avec un poids de %f\n", a.e1, a.e2, a.poids);
  });
  printf("==========================================\n\n");
}

void initialisation_des_ports(reseau *r){
  pour_chaque_commutateur(r, [&](commutateur c){
    c.ports = vector<char>(c.nbports, 'b');
  });
}

station* transformToStation(reseau* r, equipement e)
{
  return &r->stations[e-nb_commutateurs(r)-1];
}

commutateur* transformToCommutateur(reseau* r, equipement e)
{
  return &r->commutateurs[e-1];
}

equipement transformToEquipement(reseau* r, station s)
{
  return (equipement)s.numero;
}

equipement transformToEquipement(reseau* r, commutateur c)
{
  return (equipement)c.numero;
}

bool estUneStation(reseau* r, equipement e)
{
  bool res = false;
  pour_chaque_station(r, [&](station s)
  {
    if(e == s.numero)
    {
      res = true;
    }
  });
  return res;
}

void afficher_mac(station s)
{
  cout << s.mac.octets[0] + ":" + s.mac.octets[1] + ":" + s.mac.octets[2] + ":" + s.mac.octets[3] + ":" + s.mac.octets[4] + ":" + s.mac.octets[5] + "\n";
}

void afficher_mac(commutateur c)
{
  cout << c.mac.octets[0] + ":" + c.mac.octets[1] + ":" + c.mac.octets[2] + ":" + c.mac.octets[3] + ":" + c.mac.octets[4] + ":" + c.mac.octets[5];


}

void afficher_ip(station s)
{
  printf("%d.%d.%d.%d", s.ip.octets[0], s.ip.octets[1], s.ip.octets[2], s.ip.octets[3]);
}

void afficher_trame(trame t)
{
  int compteur = 1;
  // Préambule
  for(int i = 0; i<t.preambule.size(); i++)
  {
    string s = to_string(t.preambule[i]);
    for (char & c : s)
    {
      printf("%c", c);
      if(compteur%2 == 0)
      {
        printf(" ");
      }
      compteur++;
    }
  }

  // SFD
  string s = to_string(t.sfd);
  for (char & c : s)
  {
    printf("%c", c);
    if(compteur%2 == 0)
    {
      printf(" ");
    }
    compteur++;
  }

  // Destination
  for(int i = 0; i<sizeof t.destination.octets/sizeof t.destination.octets[0]; i++)
  {
    for (char & c : t.destination.octets[i])
    {
      printf("%c", c);
      if(compteur%2 == 0)
      {
        printf(" ");
      }
      compteur++;
    }
  }

  // Source
  for(int i = 0; i<sizeof t.source.octets/sizeof t.source.octets[0]; i++)
  {
    for (char & c : t.source.octets[i])
    {
      printf("%c", c);
      if(compteur%2 == 0)
      {
        printf(" ");
      }
      compteur++;
    }
  }

  // Type
  printf("0%x ", t.type);

  // Data
  compteur = 1;
  for(int i = 0; i<strlen(t.data); i++)
  {
    printf("%x", t.data[i]);
    if(compteur%2 == 0)
    {
      printf(" ");
    }
    compteur++;
  }

  // FCS
  compteur = 1;
  for(int i = 0; i<32; i++)
  {
    printf("1");
    if(compteur%2 == 0)
    {
      printf(" ");
    }
    compteur++;
  }
}

uint32_t nb_equipements(reseau* r)
{
    return r->nb_equipements;
}

uint32_t nb_stations(reseau* r)
{
  return r->stations.size();
}

uint32_t nb_commutateurs(reseau* r)
{
  return r->commutateurs.size();
}

uint32_t nb_aretes(reseau* r)
{
    return r->aretes.size();
}

equipement ajouter_station(reseau* r, adr_ip ip, adr_mac mac)
{
  struct station s;
  r->nb_equipements++;
  s.numero = nb_equipements(r);
  s.ip = ip;
  s.mac = mac;
  r->stations.push_back(s);
  equipement e(nb_equipements(r));
  return e;
}

equipement ajouter_commutateur(reseau* r, adr_mac mac, unsigned int nbports, unsigned int priorite, vector<adr_mac> table)
{
  struct commutateur c;
  r->nb_equipements++;
  c.numero = nb_equipements(r);
  c.mac = mac;
  c.nbports = nbports;
  c.priorite = priorite;
  c.table = table;
  r->commutateurs.push_back(c);
  equipement e(nb_equipements(r));
  return e;
}

bool existe_arete(reseau* r, arete a)
{
    if (a.e1 > a.e2)
        swap(a.e1, a.e2);
    for (uint32_t i = 0; i < r->aretes.size(); i++)
    {
        arete aa = r->aretes[i];
        if (a.e1 == aa.e1 && a.e2 == aa.e2)
            return true;
    }
    return false;
}

bool ajouter_arete(reseau* r, arete a)
{
    if (a.e1 > nb_equipements(r) || a.e2 > nb_equipements(r) || a.e1 == a.e2)
    {
        return false;
      }
    if (a.e1 > a.e2)
        swap(a.e1, a.e2);
    if (!existe_arete(r, a))
    {
        r->aretes.push_back(a);
        return true;
    }
    else
      printf("refus\n");
        return false;
}

void pour_chaque_arete(reseau* r, function<void(arete)> f)
{
    for (uint32_t i = 0; i < r->aretes.size(); i++)
        f(r->aretes[i]);
}

void pour_chaque_station(reseau* r, function<void(station)> f)
{
  for(int i = 0; i<r->stations.size(); i++)
    f(r->stations[i]);
}

void pour_chaque_commutateur(reseau* r, function<void(commutateur)> f)
{
  for(int i = 0; i<r->commutateurs.size(); i++)
    f(r->commutateurs[i]);
}

trame generer_trame(reseau* r, adr_mac dest, adr_mac src, u_int16_t type, const char* data)
{
  trame t;
  t.destination = dest;
  t.source = src;
  t.type = type;
  t.data = data;
  return t;
}

equipement* findEquipementByMAC(reseau* r, adr_mac mac)
{
  for(int i = 0; i<r->stations.size(); i++)
  {
    if(r->stations[i].mac.compareAdr_mac(mac) == 0)
    {
      return &r->stations[i].numero;
    }
  }

  for(int i = 0; i<r->commutateurs.size(); i++)
  {
    if(r->commutateurs[i].mac.compareAdr_mac(mac) == 0)
    {
      return &r->commutateurs[i].numero;
    }
  }
  return NULL;
}

void showMAC(adr_mac mac)
{
  cout << mac.octets[0] + ":" + mac.octets[1] + ":" + mac.octets[2] + ":" + mac.octets[3] + ":" + mac.octets[4] + ":" + mac.octets[5] + "\n";
}

bool estDansLaTable(vector<adr_mac> table, adr_mac mac)
{
  for(int i = 0; i<table.size(); i++)
  {
    if(table[i].compareAdr_mac(mac) == 0)
    {
      return true;
    }
  }
  return false;
}

void emission_trame(reseau* r, trame t, equipement src, equipement pred, int num){
  printf("-------------------------\nTentative d'émission de la trame n°%d...\n", num);
  if(estUneStation(r,src))
  {
    station* s = transformToStation(r,src);
    printf("\tSOURCE (station %d): ", s->numero-nb_commutateurs(r));
    showMAC(s->mac);
    printf("\tDESTINATION (station %d): ", *findEquipementByMAC(r, t.destination)-nb_commutateurs(r));
    showMAC(t.destination);
    if(s->mac.compareAdr_mac(t.destination) == 0)
    {
      // On a trouvé la destination
      printf("\nDestination trouvée!\n\nContenu de la trame: ");
      afficher_trame(t);
      printf("\n");
    } else {
      pour_chaque_equipement_adjacent(r,src,[&](equipement e)
      {
        if(e!=pred)
        {
          int n = num+1;
          if(estUneStation(r,e))
          {
            printf("\tEmission:\n\t\t➢ Equipement voisin détecté : émission de la trame depuis la station %d à la station %d\n\n", s->numero-nb_commutateurs(r), e-nb_commutateurs(r));
          } else {
            printf("\tEmission:\n\t\t➢ Equipement voisin détecté : émission de la trame depuis la station %d au commutateur %d\n\n", s->numero-nb_commutateurs(r), e);
          }
          emission_trame(r,t,e,src,n);
        }
      });
    }
  } else {
    commutateur* c = transformToCommutateur(r,src);
    printf("\tSOURCE (commutateur %d): ", src);
    showMAC(c->mac);
    printf("\tDESTINATION (station %d): ", *findEquipementByMAC(r, t.destination)-nb_commutateurs(r));
    showMAC(t.destination);
    if(estUneStation(r,pred))
    {
      station* spred = transformToStation(r, pred);
      if(!estDansLaTable(c->table, spred->mac))
      {
        printf("\tAdressage:\n\t\t➢ Ajout de l'adresse MAC de la station %d à la table de commutation du switch %d : ", spred->numero-nb_commutateurs(r), src);
        afficher_mac(*spred);
        c->table.push_back(spred->mac);
        printf("\t\t➢ Port sélectionné: %d\n", c->table.size()-1);
      }
    } else {
      if(!estDansLaTable(c->table, t.source))
      {
        printf("\tAdressage:\n\t\t➢ Ajout de l'adresse source de la TRAME à la table de commutation du switch %d : ", src);
        showMAC(t.source);
        c->table.push_back(t.source);
        printf("\t\t➢ Port sélectionné: %d\n", c->table.size()-1);
      }
    }
    if(estDansLaTable(c->table, t.destination))
    {
      // On a trouvé la destination donc on emet la trame vers cette station
      int n = num+1;
      printf("\tEmission:\n\t\t➢ Destination trouvée dans la table de commutation : émission de la trame depuis le commutateur %d à la station %d\n\n", c->numero, *findEquipementByMAC(r, t.destination)-nb_commutateurs(r));
      emission_trame(r, t, *findEquipementByMAC(r, t.destination), src, n);
    } else {
      // Si pas trouvé, alors on fait un broadcast
      pour_chaque_equipement_adjacent(r,src,[&](equipement e)
      {
        if(e != pred)
        {
          if(estUneStation(r,e))
          {
            printf("\tEmission:\n\t\t➢ Equipement voisin détecté : émission de la trame depuis le commutateur %d à la station %d\n\n", src, e-nb_commutateurs(r));
          } else {
            printf("\tEmission:\n\t\t➢ Equipement voisin détecté : émission de la trame depuis le commutateur %d au commutateur %d\n\n", src, e);
          }
          int n = num+1;
          emission_trame(r, t, e, src, n);
        }
      });
    }
  }
  num++;
}




void pour_chaque_arete_incidente(reseau* r, station s, function<void(arete)> f)
{
    if (s.numero >= nb_equipements(r))
        return;
    for (uint32_t i = 0; i < r->aretes.size(); i++)
    {
        arete a = r->aretes[i];
        if (a.e1 == s.numero || a.e2 == s.numero)
            f(a);
    }
}

void pour_chaque_equipement_adjacent(reseau* r, equipement e, function<void(equipement)> f)
{
    for (uint32_t i = 0; i < r->aretes.size(); i++)
    {
        arete a = r->aretes[i];
        if (a.e1 == e)
            f(a.e2);
        else if (a.e2 == e)
            f(a.e1);
    }
}

double poids(reseau * r, arete a)
{
  if(existe_arete(r,a))
  {
    if(a.e1 > a.e2)
        swap(a.e1, a.e2);
    vector<arete> v = r->aretes;
    return a.poids;
  }
  return 0.0;
}

void changer_poids(reseau* r, arete a, double p)
{
  if(existe_arete(r,a))
  {
    if(a.e1 > a.e2)
        swap(a.e1, a.e2);
    vector<arete> v = r->aretes;
    for(int i = 0; i<v.size(); i++)
    {
      if(v[i].e1 == a.e1 && v[i].e2 == a.e2)
      {
        a.poids = p;
      }
    }
  }
}
