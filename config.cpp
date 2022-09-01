#include "reseau.h"
#include "config.h"
#include "types.h"

#include <cstdio>
#include <cstring>
#include <iostream>

adr_ip toIP(char* s)
{
  char* c = strtok(s,".");
  adr_ip ip;
  for(int i = 0; i<4;i++)
  {
    ip.octets[i] = atoi(c);
    c = strtok (NULL, ".");
  }
  return ip;
}

adr_mac toMAC(char* s)
{
  char* c = strtok(s,":");
  adr_mac mac;
  for(int i = 0; i<6;i++)
  {
    mac.octets[i] = c;
    c = strtok (NULL, ":");
  }
  return mac;
}

void createReseau(reseau* r, const char* nomFichier)
{
  printf("============================================================\n");
  printf("Informations sur le réseau contenu dans le fichier %s:\n", nomFichier);
  printf("============================================================\n");
  FILE* fichier = fopen(nomFichier, "r");
  char chaine[1000] = "";

  unsigned int ligne = 1;
  unsigned int col = 0;
  unsigned int nbequipements;
  unsigned int nbliaisons;
  unsigned int numero = 0;
  while (fgets(chaine, 1000, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
  {
    char * pch;
    if(ligne == 1)
    {
      pch = strtok (chaine," ");
      while (pch != NULL)
      {
        switch (col) {
          case 0: nbequipements = atoi(pch); break;
          case 1: nbliaisons = atoi(pch); break;
        }
        pch = strtok (NULL, " ");
        col++;
      }
    } else {
      unsigned int type;
      char* station[2];
      char* commutateur[3];
      char* arete[3];
      pch = strtok (chaine,";");
      if(ligne<=nbequipements+1){
      // On lit une seule ligne à la fois
        while (pch != NULL)
        {
          if(col == 0)
          {
            type = atoi(pch);
          } else {
            switch (type) {
              case 1:
                switch (col) {
                  case 1: station[0] = pch; break;
                  case 2: station[1] = pch; ajouter_station(r, toIP(station[1]), toMAC(station[0])); break;
                }
                break;
              case 2:
                switch (col) {
                  case 1: commutateur[0] = pch; break;
                  case 2: commutateur[1] = pch; break;
                  case 3: commutateur[2] = pch; vector<adr_mac> table; ajouter_commutateur(r, toMAC(commutateur[0]), atoi(commutateur[1]), atoi(commutateur[2]), table); break;
                }
                break;
              }
            }
            pch = strtok (NULL, ";");
            col++;
          }
        } else {
          // On lit les liaisons
          while (pch != NULL)
          {
            switch (col) {
              case 0: arete[0] = pch; break;
              case 1: arete[1] = pch; break;
              case 2: arete[2] = pch; ajouter_arete(r, {(unsigned int)atoi(arete[0])+1,(unsigned int)atoi(arete[1])+1, atof(arete[2])} ); break;
            }
            pch = strtok (NULL, ";");
            col++;
          }
        }
        numero++;
      }
      col = 0;
      ligne++;
    }
    fclose(fichier);
}
