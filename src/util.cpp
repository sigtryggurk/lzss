//
//  util.cpp
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#include "util.h"
/**
 * Constructor for BitWriter.
 *
 * @param file output file to be written
 */
BitWriter::BitWriter(FILE* file)
{
    this->file = file;
    this->offset = 0;
}

/**
 * Write bits to BitWriter (<=32 bits)
 *
 * @param val value to be written
 * @param size number of bits to be written
 */
void BitWriter::write (int val, int size) {
    if (this->offset + size >= BUFFER_SIZE_BITS)
    {
      int rembits = BUFFER_SIZE_BITS - this->offset;
      PUT_BITS_VALUE(this->writeBuffer, this->offset, val >> (size-rembits), rembits);
      fwrite(this->writeBuffer , sizeof(char), BUFFER_SIZE, this->file);
      PUT_BITS_VALUE(this->writeBuffer, 0, val, size - rembits);
    }
    else
      PUT_BITS_VALUE(this->writeBuffer, this->offset, val, size);
    
    this->offset = (this->offset + size) % BUFFER_SIZE_BITS;
}

/**
 * Flush any remaining bytes to the file
 */
void BitWriter::flush()
{
    if (this->offset !=0)
    {
      int rembits = 8 - (this->offset % 8);
      PUT_BITS_VALUE(this->writeBuffer, this->offset, 0, rembits); /* mask out end of incomplete byte */
      fwrite(this->writeBuffer , sizeof(char), (this->offset +7) >> 3, this->file);
    }
}


/**
 * Constructor for BitReader.
 *
 * @param file input file to be read
 */
BitReader::BitReader(FILE* file)
{
    this->file = file;
    this->offset = 0;
    this->curByte = getc(file);
}

/**
 * Read bits from BitReader (<=32 bits)
 *
 * @param size number of bits to read
 * @return read bits
 */
int BitReader::read (int size) {
    
    if (this->curByte == EOF)
        return EOF;
    
    int result;
    
    /* can return bits from current byte */
    if (size < (8 - this->offset))
    {
        result = GET_BITS_VALUE(&this->curByte, this->offset, size);
        this->offset = (this->offset + size) % 8;
    }
    /* need to read in more bytes from file */
    else
    {
        int nbytes = (size + this->offset) >> 3; /* 1 extra byte if all is consumed */
        this->readBuffer[0] = this->curByte;
        size_t count = fread(&this->readBuffer[1], sizeof(char), nbytes, this->file);
        if (count == nbytes)
        {
          result = GET_BITS_VALUE(this->readBuffer, this->offset, size);
          this->curByte = readBuffer[nbytes];
        }
        /* handle case where exactly the rest of input is consumed */ 
        else if (((count >> 3) + (8 - this->offset)) == size)
        {
          result = GET_BITS_VALUE(this->readBuffer, this->offset, size);
          this->curByte = EOF;
        }
        else
          result = this->curByte = EOF;
        
        this->offset = (this->offset + size) % 8;
    }
    return result;
}
