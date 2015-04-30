//
//  compression.cpp
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#include "lzss.h"
#include <stdlib.h>
#include "parameters.h"
#include "util.h"
#include "findmatch.h"

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
void compress(FILE *in, FILE *out)
{
    BitWriter writer(out);
    unsigned char uncompressedData[MAX_MATCH_LEN];
    unsigned char window[WINDOW_SIZE];
    
    int curUncomp = 0;
    int curWindowSlot = 0;
    
    MatchData match;
    bool flag = 0;
    int c, i, uncompressedDataLen;
    
    /* make sure window is init as all null */
    for (i = 0; i < WINDOW_SIZE; i++)
    {
        window[i] = '\0';
    }
    
    /* populate uncompressed data */
    for (uncompressedDataLen = 0; (uncompressedDataLen < MAX_MATCH_LEN) && ((c = getc(in)) != EOF); uncompressedDataLen++)
    {
        uncompressedData[uncompressedDataLen] = c;
    }
    
    if (uncompressedDataLen == 0)
    {
        return;  /* in is empty */
    }
    
    /* first byte is never a match */
    match.length = 1;
    match.offset = 0;
    
    while (uncompressedDataLen > 0)
    {
        /* matched with garbage bytes */
        if (match.length > uncompressedDataLen)
        {
            match.length = uncompressedDataLen;
        }
        
        /* match is not long enough - stream byte */
        if (match.length < MIN_MATCH_LEN)
        {
            match.length = 1;
            flag = false;
            writer.write(flag, FLAG_LEN);
            writer.write(uncompressedData[curUncomp], CHAR_LEN);
        }
        /* match is long enough - stream copy byte */
        else
        {
            flag = true;
            writer.write(flag, FLAG_LEN);
            writer.write(match.offset, OFFSET_LEN);
            writer.write(match.length - MIN_MATCH_LEN, NBYTES_LEN);
        }
        
        /* slide window by match.length */
        i = 0;
        while ((i < match.length) && ((c = getc(in)) != EOF))
        {
            /* add old byte into sliding window and new into lookahead */
            window[curWindowSlot] = uncompressedData[curUncomp];
            uncompressedData[curUncomp] = c;
            curWindowSlot = (curWindowSlot + 1) % WINDOW_SIZE;
            curUncomp = (curUncomp + 1) % MAX_MATCH_LEN;
            i++;
        }
        
        /* handle when we get EOF */
        while (i < match.length)
        {
            window[curWindowSlot] = uncompressedData[curUncomp];
            curWindowSlot = (curWindowSlot + 1) % WINDOW_SIZE;
            curUncomp = (curUncomp + 1) % MAX_MATCH_LEN;
            uncompressedDataLen--;
            i++;
        }
        
        /* find match for the remaining characters */
        match = findLargestMatch_KMP(window, curWindowSlot, uncompressedData, curUncomp);
        
    }
    
    /* flush out any unwritten bytes */
    writer.flush();
}

/**
 *  Decompresses input file according to the aforementioned sliding window dictionary encoding (see compress):
 *
 *  @param in input file to decompress
 *  @param out decompressed output file
 */
void decompress(FILE *in, FILE *out)
{
    BitReader reader(in);
    unsigned char window[WINDOW_SIZE];
    
    MatchData encoding;
    bool flag;
    int  c, i;
    int next = 0;  /* next char in window */
    
    
    /* make sure window is init as all null */
    for (i = 0; i < WINDOW_SIZE; i++)
    {
        window[i] = '\0';
    }
    
    c = reader.read(FLAG_LEN);
    while (c != EOF)
    {
        flag = c & 0x01;
        if (flag)
        {
            if ((encoding.offset = reader.read(OFFSET_LEN)) == EOF)
                break;
           
            if ((encoding.length = reader.read(NBYTES_LEN)) == EOF)
                break;
            
            encoding.length += MIN_MATCH_LEN;
            
            /* write out bytes and put them in window TODO: use fwrite+memcpy*/
            for (i = 0; i < encoding.length; i++)
            {
                c = window[(encoding.offset + i) % WINDOW_SIZE];
                putc(c, out);
                window[(next + i) % WINDOW_SIZE] = c;
            }
            
            next = (next + encoding.length) % WINDOW_SIZE;
        }
        else
        {
            /* uncompressed char */
            if ((c = reader.read(CHAR_LEN)) == EOF)
                break;
            
            /* write out byte and put in window */
            putc(c, out);
            window[next] = c;
            next = (next + 1) % WINDOW_SIZE;
            
        }
        
        /* get next flag */
        c = reader.read(FLAG_LEN);
    }
}
