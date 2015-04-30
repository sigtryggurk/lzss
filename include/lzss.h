//
//  compression.h
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#ifndef __Compression__compression__
#define __Compression__compression__

#include <iostream>

/**
 *  Compresses input file according to the following sliding window dictionary encoding:
 *
 *  unencoded "byte": A 0 bit followed by an 8-bit char
 *  encoded "copy byte": A 1 bit, followed by OFFSET_LEN bits that tells us where to start
 *  copying from, followed NBYTES_LEN bits that tell us how many bytes to copy
 *
 *  Compressed output is bit-packed for storage efficiency.
 *
 *  @param in input file to compress
 *  @param out compressed output file
 */
void compress(FILE *in, FILE *out);

/**
 *  Decompresses input file according to the aforementioned sliding window dictionary encoding (see compress):
 *
 *  @param in input file to decompress
 *  @param out decompressed output file
 */
void decompress(FILE *in, FILE *out);

#endif /* defined(__Compression__compression__) */
