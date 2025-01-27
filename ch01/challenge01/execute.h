#ifndef EXECUTE_H
#define EXECUTE_H

#include "svec.h"

int execute(svec* tokens);
int executeSemicolon(svec* tokens, int index);
int executePipe(svec* tokens, int index);
int executeBackground(svec* tokens, int index);
int executeAnd(svec* tokens, int index);
int executeOr(svec* tokens, int index);
int executeRedirectIn(svec* tokens, int index);
int executeRedirectOut(svec* tokens, int index);
int executecmd(svec* tokens);
int executePar(svec* tokens, int index1, int index2);

#endif
