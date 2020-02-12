#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#include "svec.h"
#include "execute.h"

void
check_rv(int rv)
{
	if (rv == -1) {
		perror("fail");
		exit(1);
	}
}


//return the index of the string in the sv if sv contains the string, 0 otherwise.
int contains(svec* sv, const char* string) {
    for(int i = 0; i < sv->size; i++) {
	if(strcmp(svec_get(sv, i), string) == 0) {
	    return i;
	}
    }
    return 0;

}

int
execute(svec* tokens)
{

    const char* ops[] = {";", "|", "&", "||", "&&", "<", ">"};
    
    if(contains(tokens, ops[0])) {
    	executeSemicolon(tokens, contains(tokens, ops[0]));
    
    }
    else if(contains(tokens, ops[1])) {
    	executePipe(tokens);
    }
    else if(contains(tokens, ops[2])) {
    	executeBackground(tokens, contains(tokens, ops[2]));
    
    }
    else if(contains(tokens, ops[3])) {
    	executeOr(tokens, contains(tokens, ops[3]));
    
    }
    else if(contains(tokens, ops[4])) {
    	executeAnd(tokens, contains(tokens, ops[4]));
    
    }
    else if(contains(tokens, ops[5])) {
    	executeRedirectIn(tokens, contains(tokens, ops[5]));
    
    }
    else if(contains(tokens, ops[6])) {
    	executeRedirectOut(tokens);
    
    }
    else {
    	if(tokens->size == 0) {
       		return 0;
        }
        	
        if(strcmp(svec_get(tokens, 0), "cd") == 0) {
        	if(tokens->size == 1) {
			chdir(getenv("HOME"));
        	}
        	else if(tokens->size > 2) {
        		printf("nush: cd: too many arguments\n");
        	}
        	else {
			char* dir = svec_get(tokens, 1);
			if(chdir(dir) == -1) {
				printf("nush: cd: %s: No such file or directory\n", dir);
			}
        	}
        }
        else if(strcmp(svec_get(tokens, 0), "exit") == 0) {
        	exit(0);
        }
        else {
    		executecmd(tokens);
    	}
    }

}

int
executeSemicolon(svec* tokens, int index) {
	svec* left = make_svec();
	svec* right = make_svec();
	
	for(int i = 0; i < index; ++i) {
		svec_push_back(left, svec_get(tokens, i));
	}
	for(int i = index + 1; i < tokens->size; ++i) {
		svec_push_back(right, svec_get(tokens, i));
	}
	execute(left);
	execute(right);
	
	free_svec(left);
	free_svec(right);
}

int
executePipe(svec* tokens) {
	
}

int executeBackground(svec* tokens, int index){
	int cpid;
	if ((cpid = fork())) {
		svec* cmd = make_svec();
		
		for(int i = index + 1; i < tokens->size; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		execute(cmd);
		free_svec(cmd);

	} else {

		svec* cmd = make_svec();
		
		for(int i = 0; i < index; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		execute(cmd);
		free_svec(cmd);
	}
}

int executeOr(svec* tokens, int index){
	svec* cmd = make_svec();

	for(int i = 0; i < index; ++i) {
		svec_push_back(cmd, svec_get(tokens, i));
	}
	int status = execute(cmd);
	free_svec(cmd);

	if (status != 0) {
		svec* cmd = make_svec();
	
		for(int i = index + 1; i < tokens->size; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		execute(cmd);
		free_svec(cmd);
		
	}

}

int executeAnd(svec* tokens, int index){
	svec* cmd = make_svec();

	for(int i = 0; i < index; ++i) {
		svec_push_back(cmd, svec_get(tokens, i));
	}
	int status = execute(cmd);
	free_svec(cmd);

	if (status == 0) {
		svec* cmd = make_svec();
	
		for(int i = index + 1; i < tokens->size; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		execute(cmd);
		free_svec(cmd);
		
	}
}

int executeRedirectIn(svec* tokens, int index){

	int cpid;
	if ((cpid = fork())) {
		waitpid(cpid, 0, 0);
	}
	else {
		int stdinCopy = dup(0);
		svec* cmd = make_svec();
	
		for(int i = 0; i < index; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		char* file = svec_get(tokens, index + 1);

		close(0);
		open(file, O_RDONLY);

		execute(cmd);
		free_svec(cmd);
		dup2(stdinCopy, 0); 

	}


}

int executeRedirectOut(svec* tokens){
/*
 	int cpid;
	if ((cpid = fork())) {
		waitpid(cpid, 0, 0);
		printf("%d: Child done.\n", getpid());
	}
	else {
		int fd = open("foo.txt", O_CREAT | O_APPEND | O_WRONLY, 0644);
		close(1);
		dup(fd);
		close(fd);

		execlp("echo", "echo", "exec'd", "echo", NULL);

	}
	return 0;
*/
}

int
executecmd(svec* tokens) 
{
	int cpid;
	if ((cpid = fork())) {
	
		int status;
		waitpid(cpid, &status, 0);
		return status;
		
	} else {
		char* args[tokens->size + 1];
		for (int ii = 0; ii < tokens->size; ++ii) {
			args[ii] = svec_get(tokens, ii);
		}
		args[tokens->size] = 0;

		execvp(svec_get(tokens, 0), args);
	}

}