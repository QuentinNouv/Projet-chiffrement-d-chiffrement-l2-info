#ifndef cipher
#define cipher
typedef unsigned char byte;
void xorcipher(int lenkey, byte *key, int lentar,byte *tar);
void xorcipher2(char* file_i, int* len, char* file_o, int lenkey, const byte *key);
#endif
