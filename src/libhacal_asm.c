#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "ctype.h"
#include "string.h"

#define IBSPACE inbuf[i] == ' ' || inbuf[i] == '\n' || inbuf[i] == 0

char** hacal_loadSet(char* filepath) {
	FILE* file = fopen(filepath,"r");
	char** set = malloc(sizeof(char*) * 15);
	for(unsigned char i = 0; i < 16; i++) {
		set[i] = malloc(sizeof(char) * 11);
		set[i][5] = 0;
	};
	char i = 0;
	unsigned char bufi;
	char get;
	loop:
		if (i == 15) goto endloop;
		get = fgetc(file);
		if (get == '\n') {
			set[i][bufi] = 0;
			bufi = 0;
			i++;
			goto loop;
		};
		if (get != EOF) {
			if (bufi < 10) {
				set[i][bufi] = tolower(get);
				bufi++;
			};
	 		goto loop;
		};
	endloop:
	return set;
}

/* Outputs the string id with the best match */
int hacallib_strfirst(char* str, unsigned char strc, char** strs) {
	int i = 0;
	int xi[strc + 1];
	for (unsigned int ii = strc; ii--; ) {
		for ( i = 0; tolower(str[i]) == strs[ii][i]; i++ );
		if ( strs[ii][i] == 0 ) i++;
		xi[ii] = i;
	};
	int best = -1;
	for ( i = strc; i--; ) if ( xi[i] > ( best > -1 ? xi[best] : -1 ) ) best = i;
	return best;
}

/* The sets should be loaded in memory as an array with pointers to arrays of chars */
char* hacal_asm(char** cset, char** vset, char* inbuf, unsigned int inbuflen) {
	unsigned char  syl       = 0;
	unsigned int   obuft     = 0;
	unsigned char* outbuf    = (char*)calloc(inbuflen, sizeof(char));
	char           csets[15];
	char           vsets[15];
	/* String comparison size; what size the string must be before it is compared to the sets. */
	if (outbuf == 0) {
		fprintf(stderr, "Failed to allocate memory for a buffer.\n");
		exit(2);
	};
	/* Calculate set entry sizes */
	for (unsigned int i = 15; i--;) {
		unsigned int ii = 0;
		while (cset[i][++ii] != 0);
		csets[i] = ii;
	};
	for (unsigned int i = 15; i--;) {
		unsigned int ii = 0;
		while (vset[i][++ii] != 0);
		vsets[i] = ii;
	};
	unsigned int i = 0;
	int s = 0;
	while (i < inbuflen) {
		while ( IBSPACE ) i++;
		s = hacallib_strfirst(inbuf + i, 15, cset);
		if ( s > -1 ) {
			i   += csets[s];
			syl |= ( s + 1 ) << 4;
		};
		s = hacallib_strfirst(inbuf + i, 15, vset);
		if ( s > -1 ) {
			i   += vsets[s];
			syl |= s + 1;
		};
		if (syl != 0) outbuf[obuft++] = syl;
		syl = 0;
		if ( IBSPACE && outbuf[obuft] != 0 ) {
			outbuf[obuft++] = 0;
			i++;
		};
	};
	for ( i = 0; outbuf[i++] != 0 || outbuf [i] != 0; );
	unsigned char* rbuf = (unsigned char*)malloc(i);
	if (rbuf == 0) {
		fprintf(stderr, "Failed to allocate memory for a buffer.\n");
		exit(2);
	};
	while ( i-- ) {
		rbuf[i] = outbuf[i];
	};
	free(outbuf);
	return rbuf;
}

/* Disassembles binary HACAL and outputs a null terminated string, also: a proper null terminated HACAL string should end in two nulls instead of one */
char* hacal_disasm(char** cset, char** vset, char* inbuf) {
	unsigned long size = 0;
	unsigned long i = 0;
	unsigned char csets[15];
	unsigned char vsets[15];
	/* Calculate set entry sizes */
	for (i = 15; i--;) {
		unsigned int ii = 0;
		while (cset[i][++ii] != 0);
		csets[i] = ii;
	};
	for (i = 15; i--;) {
		unsigned int ii = 0;
		while (vset[i][++ii] != 0);
		vsets[i] = ii;
	};
	i = 0;
	while (inbuf[i] != 0 && inbuf[i + 1] != 0 ) {
		if(inbuf[i] == 0) {
			size += 1;
		} else {
			size += csets[(inbuf[i] & 0xf0) >> 4] + vsets[inbuf[i] & 0xf];
		};
		i++;
	};
	char* outbuf = (char*)malloc(size);
	i = 0;
	while (inbuf[i] != 0 && inbuf[i + 1] != 0 ) {
		if(inbuf[i] == 0) {
			outbuf[i] = ' ';
		} else {
			for (unsigned char ii = 0; ii < csets[(inbuf[i] & 0xf0) >> 4]; ii++) {
				outbuf[i + ii] = cset[i][ii];
			};
			for (unsigned char ii = 0; ii < vsets[inbuf[i] & 0xf]; ii++) {
				outbuf[i + ii] = vset[i][ii];
			};
		};
		i++;
	};
	return outbuf;
}
