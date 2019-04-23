#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termio.h>
#include <signal.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string.h>
#include <inttypes.h>

int main(int argc, char *argv[])
{
    FILE * pFile;

    pFile = fopen(argv[1],"r");

    std::string pFile_s(argv[1]);

    fseek (pFile , 0 , SEEK_END);

    int64_t ifilesize = ftell(pFile);

    if (ifilesize == -1)
    {
        fprintf(stderr,"can't open jpg file: %s\n", argv[2]);
        perror("Failed to open jpg file");
        exit(1);
    }
    else
    {
        fprintf(stderr,"file length is %I64d bytes\n",ifilesize);
    }

    return 0;
}