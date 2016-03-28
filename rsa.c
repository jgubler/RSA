#include "rsa.h"
#include <asn1_lib.h>
#include <PubKey.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

int encrypt(char* message, char* key) {
	printf("Can't encrypt yet, but here's the decoded public key:\n");
	pubkey_decode(key);
	printf("\nAnd here's the message: \n");
	printFile(message);

	printf("Trying to encrypt here, experimental\n");
	printf("Message is encrypted in 24 Bit (3 Byte) blocks for now (:D)\n");

	FILE *clearText, *cipherText;
	clearText = fopen(message, "rb");
	cipherText = fopen("zekuremessage","wb");
	
	PubKey_t* pubKey = pubkey_decode(key);
	uint32_t pubExp, mod;
    long temp = 0;
    asn_INTEGER2long(&(pubKey->publicExponent), &temp);
    pubExp = (uint32_t) temp;
    asn_INTEGER2long(&(pubKey->modulus), &temp);
    mod = (uint32_t) temp;

	if(clearText && cipherText) {
		int x=1, i=0, eof=0;
		uint8_t bytes[3];
		uint32_t clear=0, cipher=0;

		while(!feof(clearText)) {
			size_t availableBytes = fread((void*) bytes, 1, 3, clearText);
			if(availableBytes == 1) {
				clear += bytes[0];
			} else if(availableBytes == 2) {
				clear += (bytes[0] << 8) + bytes[1];
			} else if(availableBytes ==3) {
				clear += (bytes[0] << 16) + (bytes[1] << 8) + bytes[2];
			}
			uint32_t cipher = modularExp(clear, pubExp, mod);
			fwrite(&cipher, 4, 1, cipherText);
		}
		if(clearText) {
			fclose(clearText);
		}
		if(cipherText) {
			fclose(cipherText);
		} 
	} else {
		fprintf(stderr, "Error opening files\n");
	}
	

	return 0;
}

int decrypt(char* message, char* key) {
	printf("Can't decrypt yet, but here's the decoded private key:\n");
	prkey_decode(key);
	printf("\nAnd here's the message: \n");
	printFile(message);
	return 0;
}

// Implements a^b mod n, with fast exponentiation through square&multiply
uint32_t modularExp (uint32_t a, uint32_t b, uint32_t n) {
    // log2(b) = ln(b)/ln(2)
    int explog = log(b)/log(2);
    uint64_t res = a;
    int currentBit = 0;
    // traverse the bits of b, square for every bit and if it is 1, multiply as well
    for(int i=explog-1; i>=0; i--) {
        res = (res * res) % n;
        // to get the bit for the current SAM cycle, we need to right shift by i and extract the last bit
        currentBit = (b>>i) % 2;
        if(currentBit==1) {
            res = (res * a) % n;
        }
    }
    return res;
}

void printFile(char* filename) {
	int c;
	FILE *file;
	file = fopen(filename, "r");
	if(file) {
		while((c = getc(file)) != EOF) {
			putchar(c);
		}
		fclose(file);
		printf("\n");
	} else {
		fprintf(stderr, "Error reading file\n");
	}
}