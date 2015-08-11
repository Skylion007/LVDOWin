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

typedef int bool;
#define true 1
#define false 0


static int blocksize = 8;
static int quantizer = 4;
static int qmin = 1;
static int qmax = -1;
static char *framesize;
static unsigned int framewidth = 0, frameheight = 0;
static bool grayonly = FALSE;
static bool verbose = FALSE;


int lvdo_dispatch(FILE *fi, FILE *fo, unsigned int blocksize, unsigned int quantizer, unsigned int qmin, unsigned int qmax, unsigned int width, unsigned int height, int grayonly, int verbose);

int main(int argc, char *argv[]) {
	extern int _setmode(int, int);
	_setmode(0, 0x8000);
	_setmode(1, 0x8000);


	quantizer = 6;
	qmin = 1;
	qmax = 4;
	framesize = "640x480";

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
            g_printerr("Argument error: frame size can not be divided by %d", blocksize);
            return 1;
        } else;
    else
        if(framewidth % (blocksize*2) != 0 || frameheight % (blocksize*2) != 0) {
            g_printerr("Argument error: frame size can not be divided by %d", blocksize*2);
            return 1;
        }
    return lvdo_dispatch(stdin, stdout, blocksize, quantizer, qmin, qmax, framewidth, frameheight, grayonly, verbose);
}
