//
//  util.h
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#ifndef Compression_util_h
#define Compression_util_h

#include <iostream>
#include <sys/param.h>
#include "parameters.h"


/* swap between BE and LE */
#define bswap(val)\
((((unsigned int)(((unsigned int)(val) >> 24) & 0xFF)) << 0)  | \
 (((unsigned int)(((unsigned int)(val) >> 16) & 0xFF)) << 8)  | \
 (((unsigned int)(((unsigned int)(val) >> 8) & 0xFF)) << 16)  | \
 (((unsigned int)(((unsigned int)(val) >> 0) & 0xFF)) << 24))

#ifdef __LITTLE_ENDIAN__
  #define bswapLE(val) bswap(val)
#else
  #define bswapLE(val) (val)
#endif

/* put size<=32 bits into bvec starting at pos */
#define PUT_BITS_VALUE(bvec, pos, val, size)\
*((unsigned int *)((unsigned char *)bvec + ((pos) >> 3))) = bswapLE(       \
bswapLE(*((unsigned int *)((unsigned char *)bvec + ((pos) >> 3)))) &       \
(~((unsigned int) 0xffffffff << (32 - (size)) >> ((pos) % 8))) |           \
(((unsigned int)val) << (32 - (size) - (pos) % 8)))

/* get size<=32 bits from bvec starting at pos */
#define GET_BITS_VALUE(bvec, pos, size)\
((((unsigned int)bswapLE(*(unsigned int*)(((unsigned char *)(bvec)) + ((pos) >> 3))))\
<< ((pos) % 8)) >> (32 - (size)))

/**
 * BitWriter
 * A file writer the writes at the bit level
 *
 * @field file output file
 * @field offset current offset in the writeBuffer
 * @field writeBuffer buffers output bit vals
 * @method flush write entire buffer to file
 * @method write write (<=32) bits to file
 */
class BitWriter
{
private:
    FILE *file;
    int offset;
    unsigned char writeBuffer[BUFFER_SIZE];
public:
    BitWriter(FILE* file);
    void flush();
    void write(int val, int size);
};

/**
 * A reader that reads a FILE at the bit level
 *
 * Initialize with input FILE
 *
 * @field curByte current byte from which we are reading
 * @field file input file
 * @field offset bit offset in curByte
 * @field readBuffer buffer for reads
 * @method get_bits read bits (<=BUFFER_SIZE)
 * @method close closes file
 */

class BitReader
{
private:
    int curByte;
    FILE *file;
    int offset;
    unsigned char readBuffer[BUFFER_SIZE];
public:
    BitReader(FILE* file);
    int read(int size);
};


#endif
