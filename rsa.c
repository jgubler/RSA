#include "rsa.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <asn1_lib.h>

int main(int argc, char* argv[]) {
	if(argc != 4) {
		printUsage();
		return 1;
	}
	int res;
	// For now, only works with key files named key.pub/key.pr, see usage
	if(strncmp(argv[1], "-e", 2) == 0 && strncmp(argv[2], "key.pub", 7) == 0) {
		res = encrypt(argv[3], argv[2]);
	} else if (strncmp(argv[2], "-e", 2) == 0 && strncmp(argv[3], "key.pub", 7) == 0) {
		res = encrypt(argv[1], argv[3]);
	} else if (strncmp(argv[1], "-d", 2) == 0 && strncmp(argv[2], "key.pr", 6) == 0) {
		res = decrypt(argv[3], argv[2]);
	} else if (strncmp(argv[2], "-d", 2) && strncmp(argv[3], "key.pr", 6) == 0) {
		res = decrypt(argv[1], argv[3]);
	} else {
		printUsage();
		return 1;
	}
	if(res != 0) {
		fprintf(stderr, "Error Reading Files");
	}
	return res;
}

void printUsage() {
	printf("\n\nPlease use one of the following usages: \n\n");
	printf("\trsa <message file> -e key.pub\n");
	printf("\trsa -e key.pub <message file>\n");
	printf("\trsa <message file> -d key.pr\n");	
	printf("\trsa -d key.pr <message file>\n\n\n");
}

int encrypt() {
	printf("Can't encrypt yet, but here's the decoded public key:\n");
	pubkey_decode("key.pub");
	return 0;
}

int decrypt() {
	printf("Can't decrypt yet, but here's the decoded private key:\n");
	prkey_decode("key.pr");
	return 0;
}