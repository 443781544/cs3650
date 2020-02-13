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
        while(fgets(cmd, 256, stdin) != NULL) {
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
		svec* tokens;
		int append = 0;

		while(fgets(cmd, 256, pFile) != NULL) {

			fflush(stdout);

			svec* tokens = tokenize(cmd);

			while(strcmp(svec_get(tokens, tokens->size-1), "\\") == 0) {
				tokens->size -= 1;
				char tmp[256];
				fgets(tmp, 256, pFile);
				svec* nextLine = tokenize(tmp);
				for(int i = 0; i < nextLine->size; ++i) {
					svec_push_back(tokens, svec_get(nextLine, i));
				}

				free_svec(nextLine);
			}			
			execute(tokens);
			free_svec(tokens);
		
		}
	}
	fclose(pFile);
    }


    return 0;
}


/*

*/

