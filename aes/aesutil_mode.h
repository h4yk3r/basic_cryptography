#ifndef AESUTIL_H
#define AESUTIL_H
#include <iostream>
#include <vector>
#include <string>
using namespace ::std;

void initial_text(string rawtext, unsigned int* state);
void substitution(unsigned int* state);
void inv_substitution(unsigned int* state);
void get_subkey(string rawkey, unsigned int* subkey);
void subkey_xor(int round, unsigned int* text, unsigned int(&roundkey)[44]);
void row_shift(unsigned int* state);
void col_mix(unsigned int* state);
void inv_row_shift(unsigned int* state);
void inv_col_mix(unsigned int* state);

#endif
