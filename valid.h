#ifndef valid
#define valid
#include "types.h"
#include <stdbool.h>
bool IskeyValid(byte* key, int lenkey);
byte** buildkey(int lenkey, byte* tar,int* nb);
void libDoublePointeur(byte** pointeur, int lenPointeur);
void C1(int lenkey, int lentar, byte* tar);
bool ValidKeyChar(byte key);
#endif
