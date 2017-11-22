#ifndef valid
#define valid
#include "cipher.h"
#include <stdbool.h>
bool IskeyValid(byte* key, int lenkey);
byte** buildkey(int lenkey, int lentar, byte *tar, int* nb);
void libDoublePointeur(byte** pointeur, int lenPointeur);
bool ValidKeyChar(byte key);
#endif
