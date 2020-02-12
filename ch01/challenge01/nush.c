#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "svec.h"
#include "tokenize.h"
#include "execute.h"


int
main(int argc, char* argv[])
{
    char cmd[256];


    if (argc == 1) {
	printf("nush$ ");
        while(fgets(cmd, 256, stdin) != 0) {
		fflush(stdout);

		svec* tokens = tokenize(cmd);
    		execute(tokens);
    		free_svec(tokens);
		printf("nush$ ");

	}
    }
    else {
        FILE* pFile;
	pFile = fopen(argv[1], "r");
	if (pFile != NULL) {
		while(fgets(cmd, 256, pFile) != 0) {
			fflush(stdout);
			svec* tokens = tokenize(cmd);
			execute(tokens);
			free_svec(tokens);
		}
	}
	fclose(pFile);
    }


    return 0;
}

