/*
    Copyright (C) 2014  StarBrilliant <m13253@hotmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    This program has been heavily modified by Skylion (Skylion.aaron@gmail.com)
    to work with getOpt on Windows.
 */
#define _CRT_SECURE_NO_DEPRECATE


# define TCHAR		char
# define _T(x)		x
# define _tprintf	printf
# define _tmain		main
#include <stdio.h>
#include <locale.h>
#include "wingetopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // [LONG|INT][MIN|MAX]
#include <errno.h>  // errno
#include <ctype.h>
typedef int bool;
#define true 1
#define false 0

// S2I prefix so as not to conflict with OVERFLOW and UNDEFLOW of math.
typedef enum { S2ISUCCESS, S2IOVERFLOW, S2IUNDERFLOW, S2IINCONVERTIBLE } STR2INT_ERROR;

/**
 * Convert string s to int i (output in i), supposing it is base the base.
 *
 * Max base = 36
 *
 * Return `STR2INT_ERROR` accordingly.
 *
 * Preceding whitespace is ignored, tailing whitespace will lead to an error.
 *
 * Test input output pairs:
 *
 *  "10"      10
 *  "-10"    -10
 *  "10."     S2IINCONVERTIBLE
 *  "10.0"    S2IINCONVERTIBLE
 *  "10.0e10" S2IINCONVERTIBLE
 *  "a10"     S2IINCONVERTIBLE
 *  "10a"     S2IINCONVERTIBLE
 *  " 10"     10
 *  "10 "     S2IINCONVERTIBLE
 *  "1000000000000"  S2IOVERFLOW (10^12 > int max)
 *  "-1000000000000" S2IINCONVERTIBLE (-10^12 < int min)
 *
 */
STR2INT_ERROR str2int(int *i, char *s, int base) {
	char *end;
	long  l;
	errno = 0;
	l = strtol(s, &end, base);

	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
		return S2IOVERFLOW;
	}
	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
		return S2IUNDERFLOW;
	}
	if (*s == '\0' || *end != '\0') {
		return S2IINCONVERTIBLE;
	}
	*i = l;
	return S2ISUCCESS;
}


/* Like str2int, but also print error to stderr. */
STR2INT_ERROR str2int_stderr(int *i, char *s, int base) {
	STR2INT_ERROR out;

	out = str2int (i, s, base);
	if(out == S2IINCONVERTIBLE){
		fprintf(stderr,"\"%s\" is not strtol int \n", s);
	} else if(out == S2IOVERFLOW){
		fprintf(stderr,"\"%s\" is too large for an int. Max value: %d\n", s, INT_MAX);
	} else if(out == S2IUNDERFLOW){
		fprintf(stderr,"\"%s\" is too small for an int. Min value: %d\n", s, INT_MIN);
	}
	return out;
}

int lvdo_dispatch(FILE *fi, FILE *fo, unsigned int blocksize, unsigned int quantizer, unsigned int qmin, unsigned int qmax, unsigned int width, unsigned int height, int grayonly, int verbose);

//TODO Add file handling
//TODO Fix parsing to accept nice long arguments.
int main( int argc, const char* argv[] )
{
	int blocksize = 8;
	int quantizer = 4;
	static int qmin = 1;
	static int qmax = -1;
	static char *framesize;
	static unsigned int framewidth = 0, frameheight = 0;
	static bool grayonly = true;
	static bool verbose = false;
	opterr = 0;

	int arg;

	while ((arg = getopt(argc, argv, "b:q:f:")) != -1)
		switch (arg)
		{
		case 'b':
			str2int_stderr(&blocksize, optarg, 10);
		break;
		case 'q':
			str2int_stderr(&quantizer, optarg, 10);
			break;
		case 'f':
			framesize = optarg;
			break;
		case '?':
			if (optopt == 'b' || optopt == 'q' || optopt == 'f')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,
						"Unknown option character `\\x%x'.\n",
						optopt);
			return 1;
		default:
			abort ();
		}

	if(blocksize <= 0) {
		printf("Argument error: blocksize should be a positive integer\n");
		return 1;
	}
	if(quantizer < 0 || quantizer > 7) {
		printf("Argument error: quantizer should be between 0 and 7\n");
		return 1;
	}
	if(qmax == -1)
		qmax = blocksize*blocksize/2;
	if(qmin < 0 || qmin > blocksize*blocksize) {
		printf("Argument error: qmin should be between 0 and %d\n", blocksize*blocksize);
		return 1;
	}
	if(qmax < 0 || qmax > blocksize*blocksize) {
		printf("Argument error: qmax should be between 0 and %d\n", blocksize*blocksize);
		return 1;
	}
	if(qmin >= qmax) {
		printf("Argument error: qmax should be greater than qmin\n");
		return 1;
	}
	if(framesize && framesize[0]) {
		sscanf(framesize, "%ux%u", &framewidth, &frameheight);
	}
	if(framewidth == 0 || frameheight == 0) {
		printf("Argument error: frame size should be specified with \"-s WIDTHxHEIGHT\"\n");
		return 1;
	}
	if(grayonly)
		if(framewidth % blocksize != 0 || frameheight % blocksize != 0) {
			printf("Argument error: frame size can not be divided by %d", blocksize);
			return 1;
		} else;
	else
		if(framewidth % (blocksize*2) != 0 || frameheight % (blocksize*2) != 0) {
			printf("Argument error: frame size can not be divided by %d", blocksize*2);
			return 1;
		}
	return lvdo_dispatch(stdin, stdout, blocksize, quantizer, qmin, qmax, framewidth, frameheight, grayonly, verbose);
}

