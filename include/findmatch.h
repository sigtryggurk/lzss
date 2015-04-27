//
//  findmatch.h
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#ifndef __Compression__findmatch__
#define __Compression__findmatch__

#include <stdio.h>
#include "util.h"

/**
 * Structure to store the match data given found by findLargestMatch_*
 *
 */
typedef struct MatchData
{
    unsigned int offset;
    unsigned int length;
} MatchData;

/**
 *  Finds the largest matching string between window and unCompressedData (<=MAX_MATCH_LEN)
 *
 *  Brute force approach: Find largest match for every possible startpos after curWindowSlot and 
 *  return the largest
 *
 *  @param window sliding window that stores the MAX_MATCH_LEN most recent encountered chars
 *  @param curWindowSlot current start position in window
 *  @param uncompressedData array of uncompressed chars we are trying to match
 *  @param curUncomp current start position in unCompressedData
 *  @return
 */
MatchData findLargestMatch_brute(unsigned char  *window, int curWindowSlot, unsigned char *unCompressedData, int curUncomp);

/**
 *  Finds the largest matching string between window and unCompressedData (<=MAX_MATCH_LEN)
 *
 *  Uses Knuth-Morris-Pratt Linear Search Optimization (http://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm)
 *
 *  @param window sliding window that stores the MAX_MATCH_LEN most recent encountered chars
 *  @param curWindowSlot current start position in window
 *  @param uncompressedData array of uncompressed chars we are trying to match
 *  @param curUncomp current start position in unCompressedData
 *  @return
 */
MatchData findLargestMatch_KMP(unsigned char  *window, int curWindowSlot, unsigned char *unCompressedData, int curUncomp);

#endif /* defined(__Compression__findmatch__) */
