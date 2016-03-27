#include <stdio.h>
#include <sys/types.h>
#include <PubKey.h>

int main(int argc, char **argv) {
	char buf[1024];			/* Temporary buffer */
	asn_dec_rval_t rval;	/* Decoder return value */
	PubKey_t *pubKey = 0; 	/* Type to decode */
	FILE *fp;				/* Input file handler */
	size_t size;			/* Number of bytes read */
	char *filename;			/* Input file name */

	/* Require a single filename argument */
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <file.ber>\n", argv[0]);
		exit(1);
	} else {
		filename = argv[1];
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

	return 0; /* Decoding finished successfully */
}