#include <asn1_lib.h>
#include <stdio.h>
#include <INTEGER.h>

/* Write the encoded output into some FILE stream */
int write_out(const void *buffer, size_t size, void *app_key) {
	FILE *out_fp = app_key;
	size_t wrote = fwrite(buffer, 1, size, out_fp);
	return (wrote == size) ? 0 : -1;
}

int pubkey_encode(char* filename, PubKey_t *pubKey) {
	asn_enc_rval_t ec;

	/* BER encode the data if filename is given */
	if(!filename) {
		fprintf(stderr, "Specify filename for BER output\n");
	} else { 
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

PubKey_t* pubkey_decode(char* filename) {
	char buf[1024];			/* Temporary buffer */
	asn_dec_rval_t rval;	/* Decoder return value */
	PubKey_t *pubKey = 0; 	/* Type to decode */
	FILE *fp;				/* Input file handler */
	size_t size;			/* Number of bytes read */

	/* Require a single filename argument */
	if(!filename) {
		fprintf(stderr, "Specify filename for BER input\n");
		exit(1);
	}

	/* Open input file as read-only binary */
	fp = fopen(filename, "rb");
	if(!fp) {
		perror(filename);
		exit(1);
	}

	/* Read up to the buffer size */
	size = fread(buf, 1, sizeof(buf), fp);
	fclose(fp);
	if(!size) {
		fprintf(stderr, "%s: Empty or broken\n", filename);
		exit(1);
	}

	/* Decode the input buffer as PubKey type */
	rval = ber_decode(0, &asn_DEF_PubKey, (void **) &pubKey, buf, size);
	if(rval.code != RC_OK) {
		fprintf(stderr, "%s: Broken PubKey encoding at byte %ld\n", filename, 
			(long)rval.consumed);
		exit(1);
	}

	/* Print the decoded PubKey type as XML */
	xer_fprint(stdout, &asn_DEF_PubKey, pubKey);

	return pubKey; /* Decoding finished successfully */
}

int prkey_encode(char* filename, PrKey_t *prKey) {
	asn_enc_rval_t ec;

	/* BER encode the data if filename is given */
	if(!filename) {
		fprintf(stderr, "Specify filename for BER output\n");
	} else { 
		FILE *fp = fopen(filename, "wb"); /* for BER output */

		if(!fp) {
			perror(filename);
			exit(1);
		}

		/* Encode the PrKey type as BER */
		ec = der_encode(&asn_DEF_PrKey, prKey, write_out, fp);
		fclose(fp);
		if(ec.encoded == -1) {
			fprintf(stderr, "Could not encode PrKey (at %s)\n", 
				ec.failed_type ? ec.failed_type->name : "unknown");
			exit(1);
		} else {
			fprintf(stderr, "Created %s with BER encoded PrKey\n", filename);
		}
	}

	/* Also print the constructed PrKey XER encoded (XML) */
	xer_fprint(stdout, &asn_DEF_PrKey, prKey);

	return 0; /* Encoding finished successfully */
}

PrKey_t* prkey_decode(char* filename) {
	char buf[1024];			/* Temporary buffer */
	asn_dec_rval_t rval;	/* Decoder return value */
	PrKey_t *prKey = 0; 	/* Type to decode */
	FILE *fp;				/* Input file handler */
	size_t size;			/* Number of bytes read */

	/* Require a single filename argument */
	if(!filename) {
		fprintf(stderr, "Specify filename for BER input\n");
		exit(1);
	}

	/* Open input file as read-only binary */
	fp = fopen(filename, "rb");
	if(!fp) {
		perror(filename);
		exit(1);
	}

	/* Read up to the buffer size */
	size = fread(buf, 1, sizeof(buf), fp);
	fclose(fp);
	if(!size) {
		fprintf(stderr, "%s: Empty or broken\n", filename);
		exit(1);
	}

	/* Decode the input buffer as PrKey type */
	rval = ber_decode(0, &asn_DEF_PrKey, (void **) &prKey, buf, size);
	if(rval.code != RC_OK) {
		fprintf(stderr, "%s: Broken PrKey encoding at byte %ld\n", filename, 
			(long)rval.consumed);
		exit(1);
	}

	/* Print the decoded PrKey type as XML */
	xer_fprint(stdout, &asn_DEF_PrKey, prKey);

	return prKey; /* Decoding finished successfully */
}