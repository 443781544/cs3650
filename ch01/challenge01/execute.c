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
    	executePipe(tokens, contains(tokens, ops[1]));
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
    	executeRedirectOut(tokens, contains(tokens, ops[6]));
    
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
executePipe(svec* tokens, int index) {


	int rv, cpid1;

	int pipe_fds[2];
	rv = pipe(pipe_fds);
	check_rv(rv);

	int p_read = pipe_fds[0];
	int p_write = pipe_fds[1];

	int stdin_dup = dup(0);
	int stdout_dup = dup(1);


	if ((cpid1 = fork())) {
		// parent
		close(p_write);	
		close(0);
		dup(p_read);
		int status;
		waitpid(cpid1, &status, 0);
		
		svec* cmd = make_svec();
		
		for(int i = index + 1; i < tokens->size; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		dup2(stdout_dup, 1);
		execute(cmd);
		free_svec(cmd);
		dup2(stdin_dup, 0);

	}
	else {
		close(p_read);
		close(1);
		dup(p_write);
		svec* cmd = make_svec();
		
		for(int i = 0; i < index; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		execute(cmd);
		free_svec(cmd);
		exit(0);
	}
	
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

int executeRedirectOut(svec* tokens, int index){
 	int cpid;
	if ((cpid = fork())) {
		waitpid(cpid, 0, 0);
	}
	else {
		int stdoutCopy = dup(1);
		svec* cmd = make_svec();
	
		for(int i = 0; i < index; ++i) {
			svec_push_back(cmd, svec_get(tokens, i));
		}
		char* file = svec_get(tokens, index + 1);

		close(1);
		open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);

		execute(cmd);
		free_svec(cmd);
		dup2(stdoutCopy, 1); 
	}
	return 0;

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