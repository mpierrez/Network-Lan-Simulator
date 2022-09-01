#pragma once

#include <cstdint>
#include <vector>
#include <string>

using namespace std;

using equipement = uint32_t;

struct adr_ip
{
  uint8_t octets[4];
};

struct adr_mac
{
  string octets[6];

  public:
  int compareAdr_mac(adr_mac mac){
    for(int i = 0; i<6; i++)
    {
      if(this->octets[i] < mac.octets[i]) return -1; //this plus petit que mac
      else if(this->octets[i] > mac.octets[i]) return 1; //this plus grand que mac
    }
    return 0; //this et mac sont équivalent
  }
};

struct station
{
  uint32_t numero;
  adr_ip ip;
  adr_mac mac;
};

struct commutateur
{
  uint32_t numero;
  adr_mac mac;
  unsigned int nbports;
  unsigned int priorite;
  vector<adr_mac> table;
  vector<char> ports; /* 'b' : port bloque
                         'r' : port racine
                         'd' : port designe */
};

struct arete
{
    equipement e1;
    equipement e2;
    double poids;
};


struct trame
{
  vector<unsigned int> preambule{vector<unsigned int>(7,10101010)};
  unsigned int sfd = 10101011;
  adr_mac destination;
  adr_mac source;
  u_int16_t type;
  const char* data;
  string fcs[4];
};

struct bpdu
{
  equipement rootID;
  uint32_t cost = 0;
  adr_mac transID;

  public :
  int compareBpdu(bpdu x){
    //Cas 1  : this est plus petit que x
    if(this->rootID < x.rootID) return -1;
    else if(this->rootID == x.rootID && this->cost < x.cost) return -1;
    else if(this->rootID == x.rootID && this->cost == x.cost && this->transID.compareAdr_mac(x.transID) == -1) return -1;
    
    //Cas 2 : this est plus grand que x
    else if(this->rootID > x.rootID) return 1;
    else if(this->rootID == x.rootID && this->cost > x.cost) return 1;
    else if(this->rootID == x.rootID && this->cost < x.cost && this->transID.compareAdr_mac(x.transID) == 1) return 1;
    
    //Cas 3 : this et x sont équivalent
    else return 0;
  }
};