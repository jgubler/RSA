#ifndef ASN1_LIB_H_
#define ASN1_LIB_H_
#include <sys/types.h>
#include <PubKey.h>

int pubkey_encode(char*, PubKey_t*);
PubKey_t* pubkey_decode(char*);
int write_out(const void*, size_t, void*);

#endif