#include "helpers.hpp"

/* Tokenizes the string*/
char* tokenize(char * s){
   return strtok(s, " ");
}

/* Get next token from the saved string*/
char* getnexttoken(){
    return strtok(NULL, " ");
}

/* Converts string token to integer*/
int  getnextinttoken(){
    return atoi(strtok(NULL, " "));
}

/* Converts string to integer and sets the variable*/
int setint(int *adr, char* s){
    int status = 1;
    if(s!=NULL){
        *adr = atoi(s);
        status = 0;
    }
    return status;
}

/* Groesste gemeinsame Teiler*/
int ggT(int number0, int number1){
  return number1==0?number0:ggT(number1, number0%number1);
}

/* Kleinstes gemeinsames vielfaches*/
int kgV(int number0, int number1){
    return (number0*number1)/ggT(number0, number1);
}


