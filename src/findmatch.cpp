//
//  findmatch.cpp
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#include "findmatch.h"
#include "parameters.h"

MatchData findLargestMatch_brute(unsigned char  *window, const int curWindowSlot,
                                 unsigned char *unCompressedData, const int curUncomp)
{
    int curLen = 0;
    int curStart = curWindowSlot;
    MatchData match;
    match.length = 0;
    
    do
    {
        curLen = 0;
        while((window[(curStart + curLen) % WINDOW_SIZE] ==
              unCompressedData[(curUncomp + curLen) % MAX_MATCH_LEN])
              && (curLen < MAX_MATCH_LEN))
                curLen++;
        
        if (curLen > match.length)
        {
            match.length = curLen;
            match.offset = curStart;
        }
        
        if (curLen >= MAX_MATCH_LEN)
        {
            match.length = MAX_MATCH_LEN;
            break;
        }
        
        curStart = (curStart + 1) % WINDOW_SIZE;

    }
    while (curStart != curWindowSlot);
    
    return match;
}

/**
 * Helper function to populate partial match table for KMP algorithm
 *
 * @param unCompressedData uncompressed data to populate kmpTable
 * @param kmpTable partial match table to be populated
 */
static void populateTable(unsigned char *unCompressedData, int* kmpTable)
{
    int curPos;   /* current pos in the kmpTable */
    int nextPos;  /* next pos for the current substring in unCompressedData */
    
    kmpTable[0] = -1;
    kmpTable[1] = 0;
    curPos = 2;
    nextPos = 0;
    
    while (curPos < MAX_MATCH_LEN)
    {
        if (unCompressedData[curPos - 1] == unCompressedData[nextPos])
        {
            /* continue matching */
            nextPos++;
            kmpTable[curPos] = nextPos;
            curPos++;
        }
        else if (nextPos > 0)
        {
            /* no more matches, but at least one for fallback */
            nextPos = kmpTable[nextPos];
        }
        else
        {
            /* no possible substring */
            kmpTable[curPos] = 0;
            curPos++;
        }
    }
}

MatchData findLargestMatch_KMP(unsigned char  *window, const int curWindowSlot,
                               unsigned char *unCompressedData, const int curUncomp)
{
    MatchData match;
    unsigned int startpos;                                   /* startpos of string being searched */
    unsigned int curMatchLen;
    int i;
    int kmpTable[MAX_MATCH_LEN];                             /* partial match table */
    unsigned char linearUncompressedData[MAX_MATCH_LEN];     /* non-circular copy of unCompressedData */
    
    /* build non-circular copy of unCompressedData */
    for (i = 0; i < MAX_MATCH_LEN; i++)
    {
        linearUncompressedData[i] = unCompressedData[(curUncomp + i) % MAX_MATCH_LEN];
    }
    
    /* populate partial match table */
    populateTable(linearUncompressedData, kmpTable);
    
    match.length = 0;
    match.offset = 0;
    startpos = 0;
    curMatchLen = 0;
    
    while (startpos < WINDOW_SIZE)
    {
        if (linearUncompressedData[curMatchLen] ==
            window[(startpos + curMatchLen + curWindowSlot) % WINDOW_SIZE])
        {
            curMatchLen++;
        
            if (MAX_MATCH_LEN == curMatchLen)
            {
                /* don't need to check any further */
                match.length = MAX_MATCH_LEN;
                match.offset = (startpos + curWindowSlot) % WINDOW_SIZE;
                break;
            }
        }
        else
        {
            /* partial match is longest so far */
            if (curMatchLen > match.length)
            {
                match.length = curMatchLen;
                match.offset = (startpos + curWindowSlot) % WINDOW_SIZE;
            }
            
            startpos = startpos + curMatchLen - kmpTable[curMatchLen];
            if (kmpTable[curMatchLen] > 0)
                curMatchLen = kmpTable[curMatchLen];
            else
                curMatchLen = 0;
        }
    }

    return match;
}
