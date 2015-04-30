//
//  command.cpp
//  Compression
//
//  Created by Sigtryggur Kjartansson on 4/27/15.
//  Copyright (c) 2015 Sigtryggur Kjartansson. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "parameters.h"
#include "lzss.h"

using namespace std;

int main(int argc, const char * argv[])
{
    if (argc != 4)
    {
        cout << "Invalid number of arguments" << endl;
        return EXIT_FAILURE;
    }

    FILE *in = fopen(argv[1], "rb");
    FILE *out = fopen(argv[2], "wb");
    
    char * pEnd;
    long int mode;
    mode = strtol (argv[3],&pEnd,10);
    
    if (mode == COMP_MODE)
    {
       compress(in, out);
    }
    else if (mode == DECOMP_MODE)
    {
       decompress(in, out);
    }
    else
    {
        cout << "Invalid compression mode" << endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}
