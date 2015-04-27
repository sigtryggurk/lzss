//
//  parameters.h
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/26/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//

#ifndef Compression_parameters_h
#define Compression_parameters_h

/* encoding parameters */
#define FLAG_LEN 1     /* # bits in flag encoding */
#define CHAR_LEN 8     /* # bits in char */
#define OFFSET_LEN 16  /* # bits in offset encoding */
#define NBYTES_LEN 6   /* # bits in length encoding */

#define BYTE_LEN (FLAG_LEN + CHAR_LEN)                      /* # bits in unencoded */
#define COPY_BYTE_LEN (FLAG_LEN + OFFSET_LEN + NBYTES_LEN)  /* # bits in encoded */
#define MIN_MATCH_LEN (COPY_BYTE_LEN/BYTE_LEN + 1)          /* min bytes for encoding */
#if (OFFSET_LEN < NBYTES_LEN)
  #define MAX_MATCH_LEN ((1 << OFFSET_LEN) + MIN_MATCH_LEN - 1)   /* max bytes for encoding */
#else
  #define MAX_MATCH_LEN ((1 << NBYTES_LEN) + MIN_MATCH_LEN - 1)
#endif
#define WINDOW_SIZE (1 << OFFSET_LEN)                       /* window size for possible matches */
#define BUFFER_SIZE (((COPY_BYTE_LEN + 7) >> 3) + 1)        /* max size of compression buffer */
#define BUFFER_SIZE_BITS (BUFFER_SIZE << 3)

#define COMP_MODE 0
#define DECOMP_MODE 1

#endif
