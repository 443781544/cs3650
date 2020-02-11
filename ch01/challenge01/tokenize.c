/* This file is lecture notes from CS 3650, Spring 2020 */
/* Author: Nat Tuck */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "svec.h"

	
int
isoperator(const char* text, int ii)
{
	char op = text[ii];
	if(op == '<' || op == '>' || op == '|' || op == '&' || op == ';') {
		if(op == '&' && text[ii+1] == '&') {
			return 2;
		}	
		if(op == '|' && text[ii+1] == '|') {
			return 2;
		}	
		return 1;
	}
	return 0;

}


char*
read_string(const char* text, int ii)
{
	int nn = 0;
	while (!isspace(text[ii + nn]) && !isoperator(text, ii + nn)) {
		++nn;
	}
	char* string = malloc(nn + 1);
	memcpy(string, text + ii, nn);
	string[nn] = 0;
	return string;
}

	
	
svec*
tokenize(const char* text)
{
	svec* xs = make_svec();
	int nn = strlen(text);
	int ii = 0;
	while(ii < nn) {
		if (isspace(text[ii])) {
			++ii;
			continue;
		}

		if (isoperator(text, ii)) {
			int length = isoperator(text, ii);
			char op[4] = "";
			for(int i = 0; i < length; ++i) {
				op[i] = text[ii + i];
			}
			svec_push_back(xs, op);
			ii += length;
			continue;
		}

		char* string = read_string(text, ii);
		svec_push_back(xs, string);
		ii += strlen(string);
		free(string);

		
	}
	return xs;
}
