#ifndef RSA_H_
#define RSA_H_
#include <stdint.h>

void printUsage();
int encrypt(char*,char*);
int decrypt(char*,char*);
uint32_t modularExp(uint32_t,uint32_t,uint32_t);
void printFile(char*);

#endif // RSA_H_
