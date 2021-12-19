
#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tokenizes the string*/
char* tokenize(char * s);

/* Get next token from the saved string*/
char* getnexttoken();

/* Converts string token to integer*/
int  getnextinttoken();

/* Converts string to integer and sets the variable*/
int setint(int *adr, char* s);

/* Groesste gemeinsame Teiler*/
int ggT(int number0, int number1);

/* Kleinstes gemeinsames vielfaches*/
int kgV(int number0, int number1);

#endif
