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
 */

#include <stdio.h>
#include <windows.h>
#include "lvdocommon.h"
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

typedef int bool;
#define true 1
#define false 0

#define _O_BINARY 0x8000

static int blocksize = 8;
static int quantizer = 4;
static int qmin = 1;
static int qmax = -1;
static char *framesize;
static unsigned int framewidth = 0, frameheight = 0;
static bool grayonly = FALSE;
static bool verbose = FALSE;

static struct option long_options[] =
{
		/* These options set a flag. */
		{"verbose", no_argument, &verbose, 1},
		{"gray", no_argument, &grayonly, 1},
		/* These options don’t set a flag.
       We distinguish them by their indices. */
		{"blocksize", required_argument, 0, 'b'},
		{"quantizer", required_argument, 0, 'q'},
		{"qmin", required_argument, 0, 0},
		{"qmax", required_argument, 0, 0},
		{"size", required_argument, 0, 's'},
		{0, 0, 0, 0}
};

int lvdo_dispatch(FILE *fi, FILE *fo, unsigned int blocksize, unsigned int quantizer, unsigned int qmin, unsigned int qmax, unsigned int width, unsigned int height, int grayonly, int verbose);

int main(int argc, char *argv[]) {
	extern int _setmode(int, int); //This forces Windows to use stdin and stdout
	_setmode(_fileno(stdin), _O_BINARY);
	_setmode(_fileno(stdout), _O_BINARY);

	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;
	while (1)
	{
		c = getopt_long (argc, argv, "b:q:f:?s:",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			if( strcmp( "qmin", long_options[option_index].name ) == 0 ) {
				qmin = atoi(optarg);
			} else if ( strcmp( "qmax", long_options[option_index].name ) == 0 ){
				qmax = atoi(optarg);
			}
			break;

		case 'b': blocksize = atoi(optarg);
		break;

		case 'q': quantizer = atoi(optarg);
		break;

		case 's':
			framesize = optarg;
			break;

		case '?':
			printf("blocksize -b > DCT block size [default: 8]\n"
					"quantizer > Quantizer step length, lower for more data capacity (0..7) [default: 4]\n"
					"qmin > Minimum DCT index used, lower for more data capacity "
					"(0..BLOCKSIZE*BLOCKSIZE-1) [default: 1]\n"
					"qmax > Maximum DCT index used, higher for more data capacity (1..BLOCKSIZE*BLOCKSIZE)"
					"[default: BLOCKSIZE*BLOCKSIZE/2]\n"
					"size > Frame size, must be multipliers of block size"
					"gray > Use luminance only"
					"verbose > Print debug information");
			break;

		default:
			abort ();
		}
	}

	if(blocksize <= 0) {
		g_printerr("Argument error: blocksize should be a positive integer\n");
		return 1;
	}
	if(quantizer < 0 || quantizer > 7) {
		g_printerr("Argument error: quantizer should be between 0 and 7\n");
		return 1;
	}
	if(qmax == -1)
		qmax = blocksize*blocksize/2;
	if(qmin < 0 || qmin > blocksize*blocksize) {
		g_printerr("Argument error: qmin should be between 0 and %d\n", blocksize*blocksize);
		return 1;
	}
	if(qmax < 0 || qmax > blocksize*blocksize) {
		g_printerr("Argument error: qmax should be between 0 and %d\n", blocksize*blocksize);
		return 1;
	}
	if(qmin >= qmax) {
		g_printerr("Argument error: qmax should be greater than qmin\n");
		return 1;
	}
	if(framesize && framesize[0]) {
		sscanf(framesize, "%ux%u", &framewidth, &frameheight);
	}
	if(framewidth == 0 || frameheight == 0) {
		g_printerr("Argument error: frame size should be specified with \"-s WIDTHxHEIGHT\"\n");
		return 1;
	}
	if(grayonly)
		if(framewidth % blocksize != 0 || frameheight % blocksize != 0) {
			g_printerr("Argument error: frame size can not be divided by %d\n", blocksize);
			return 1;
		} else;
	else
		if(framewidth % (blocksize*2) != 0 || frameheight % (blocksize*2) != 0) {
			g_printerr("Argument error: frame size can not be divided by %d\n", blocksize*2);
			return 1;
		}
	return lvdo_dispatch(stdin, stdout, blocksize, quantizer, qmin, qmax, framewidth, frameheight, grayonly, verbose);
}
