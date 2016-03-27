.PHONY: clean asn1

all: rsa keygen pubkey_decode pubkey_encode

rsa: rsa.c rsa.h
	gcc rsa.h rsa.c -o out/rsa -lm

keygen: keygen.h keygen.c
	gcc keygen.h keygen.c -o out/keygen -lm

asn1: asn1/rsa.asn1
	cd asn1 && asn1c rsa.asn1 && rm -f converter-sample.c

pubkey_decode: asn1
	cd asn1 && gcc -I. -o ../out/pubkey_decode *.c ../pubkey_decode.c

pubkey_encode: asn1
	cd asn1 && gcc -I. -o ../out/pubkey_encode *.c ../pubkey_encode.c

clean:
	rm -f out/* && cd asn1 && rm -f PrKey.* PubKey.* der_* ber_* xer_* per_* constr* asn_* OCTET_* Native* INTEGER.* converter* BIT_STRING.* Makefile.am.sample