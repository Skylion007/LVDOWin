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
#include <malloc.h> //For memory handling
#include <stdlib.h>

#ifndef _LVDOCOMMON_H
#define _LVDOCOMMON_H

//The following two declarations were borrowed from GLib
#define unlikely(x)     __builtin_expect((x),0) //For compiler optimizations
#define SIZE_OVERFLOWS(a,b) (unlikely ((b) > 0 && (a) > 0xFFFFFFFFUL / (b)))

static __inline void* malloc_n (unsigned int n_blocks, unsigned int n_block_bytes)
{
  if (SIZE_OVERFLOWS (n_blocks, n_block_bytes))
    {
      fprintf(stderr, "Warning: An overflow has occured");
    }
  return malloc (n_blocks * n_block_bytes);
}

//end of GLIB code

static __inline unsigned char prevent_byte_overflow(int x) {
    if(x > 255) {
        printf("lvdo: [warning] clipping %d to 255\n", x);
        return 255;
    }
    if(x < 0) {
        printf("lvdo: [warning] clipping %d to 0\n", x);
        return 0;
    }
    return x;
}

static __inline unsigned char prevent_yuv_overflow(int x) {
    if(x > 235) {
        printf("lvdo: [warning] clipping %d to 235\n", x);
        return 235;
    }
    if(x < 16) {
        printf("lvdo: [warning] clipping %d to 16\n", x);
        return 16;
    }
    return x;
}

/* Adapted from http://rosettacode.org/wiki/Zig-zag_matrix#C
   Licensed under GFDL 1.2 */
static __inline unsigned int *new_zigzag_index(unsigned int size) {
    unsigned int *zigzag_index = malloc_n(size*size, sizeof (unsigned int));
    unsigned int i, j, idx;
    if(size == 0)
        return zigzag_index;
    for (i = idx = 0; i < size * 2; i++)
        for (j = (i < size) ? 0 : i-size+1; j <= i && j < size; j++)
            zigzag_index[(i&1) ? j*(size-1)+i : (i-j)*size+j ] = idx++;
    return zigzag_index;
}

static __inline unsigned int *new_zigzag_reverse(unsigned int size) {
    unsigned int *zigzag_index = new_zigzag_index(size);
    unsigned int *zigzag_reverse = malloc_n(size*size, sizeof (unsigned int));
    unsigned int i;
    if(size == 0)
        return zigzag_index;
    for(i = 0; i < size*size; i++)
        zigzag_reverse[zigzag_index[i]] = i;
    free(zigzag_index);
    return zigzag_reverse;
}

/*
static unsigned int floor_sqrt(unsigned int x) __attribute__((__const__));
static unsigned int floor_sqrt(unsigned int x) {
    unsigned int sq = 0;
    unsigned int k = 0;
    while(sq < x)
        sq += ++k<<1 | 1;
    return k;
}
*/

static unsigned int ceil_sqrt(unsigned int x) {
    unsigned int sq = 0;
    unsigned int k = 0;
    while(sq < x)
        sq += k++<<1 | 1;
    return k;
}

static __inline void print_block(const unsigned int *block, unsigned int size) {
    unsigned int i, j;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++)
            printf("%d ", block[i*size+j]);
        printf(";\n");
    }
    printf("\n");
}

static __inline void print_block_double(const double *block, unsigned int size) {
    unsigned int i, j;
    for(i = 0; i < size; i++) {
        for(j = 0; j < size; j++)
            printf("%lf ", block[i*size+j]);
        printf(";\n");
    }
    printf("\n");
}

#endif
