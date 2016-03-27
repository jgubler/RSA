#include <stdio.h>
#include <sys/types.h>
#include <PubKey.h>
#include <INTEGER.h>

/* Write the encoded output into some FILE stream */
static int write_out(const void *buffer, size_t size, void *app_key) {
	FILE *out_fp = app_key;
	size_t wrote = fwrite(buffer, 1, size, out_fp);
	return (wrote == size) ? 0 : -1;
}

int main(int argc, char **argv) {
	PubKey_t *pubKey;
	asn_enc_rval_t ec;

	/* Allocate the PubKey_t */
	pubKey = calloc(1, sizeof(PubKey_t));
	if(!pubKey) {
		perror("calloc() failed");
		exit(1);
	}

	/* Initialize the PubKey members */
	INTEGER_t *mod = calloc(1, sizeof(INTEGER_t));
	INTEGER_t *pubExp = calloc(1, sizeof(INTEGER_t));
	asn_long2INTEGER(mod, 77l);
	asn_long2INTEGER(pubExp, 13l);
	pubKey->modulus = *mod;
	pubKey->publicExponent = *pubExp;

	/* BER encode the data if filename is given */
	if(argc < 2) {
		fprintf(stderr, "Specify filename for BER output\n");
	} else { 
		const char *filename = argv[1];
		FILE *fp = fopen(filename, "wb"); /* for BER output */

		if(!fp) {
			perror(filename);
			exit(1);
		}

		/* Encode the PubKey type as BER */
		ec = der_encode(&asn_DEF_PubKey, pubKey, write_out, fp);
		fclose(fp);
		if(ec.encoded == -1) {
			fprintf(stderr, "Could not encode PubKey (at %s)\n", 
				ec.failed_type ? ec.failed_type->name : "unknown");
			exit(1);
		} else {
			fprintf(stderr, "Created %s with BER encoded PubKey\n", filename);
		}
	}

	/* Also print the constructed PubKey XER encoded (XML) */
	xer_fprint(stdout, &asn_DEF_PubKey, pubKey);

	return 0; /* Encoding finished successfully */
}