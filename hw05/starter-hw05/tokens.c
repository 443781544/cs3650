#include <stdio.h>
#include <stdlib.h>

#include "svec.h"
#include "tokenize.h"

int
main(int argc, char* argv[]) {
   char line[100];

   while (1) {
     printf("tokens$ ");
     fflush(stdout);

     char* rv = fgets(line, 96, stdin);
     if (!rv) {
        exit(0);
     }
     svec* tokens = tokenize(line);
     
     svec* rev = reverse_svec(tokens);
     print_svec(rev);
     free_svec(tokens);
     free_svec(rev);
   }

}




