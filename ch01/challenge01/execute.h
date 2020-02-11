#ifndef EXECUTE_H
#define EXECUTE_H

#include "svec.h"

void execute(svec* tokens);
void executeSemicolon(svec* tokens, int index);
void executePipe(svec* tokens);
void executeBackground(svec* tokens);
void executeAnd(svec* tokens, int index);
void executeOr(svec* tokens, int index);
void executeRedirectOut(svec* tokens);
void executeRedirectIn(svec* tokens);
void executecmd(svec* tokens);

#endif
