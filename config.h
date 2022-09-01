#pragma once

#include "types.h"
#include <cassert>
#include <functional>
#include <string>

adr_ip toIP(char* s);
adr_mac toMAC(char* s);
void createReseau(reseau* r, const char* nomFichier);
