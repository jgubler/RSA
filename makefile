.PHONY: clean asn1

all: rsa asn1 keygen

rsa: rsa.c rsa.h
	gcc rsa.h rsa.c -o out/rsa -lm

keygen: keygen.h keygen.c asn1
	cd out && gcc ../keygen.h ../keygen.c ../asn1_lib.c ../asn1/*.c -o keygen -lm -I.. -I../asn1

asn1: asn1/rsa.asn1
	cd asn1 && asn1c rsa.asn1 && rm -f converter-sample.c

clean:
	rm -f out/*
	cd asn1 && rm -f PrKey.* PubKey.* der_* ber_* xer_* per_* constr* asn_* OCTET_*
	cd asn1 && rm -f Native* INTEGER.* converter* BIT_STRING.* Makefile.am.sample