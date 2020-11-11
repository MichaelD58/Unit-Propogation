#ifndef MAIN_H
#define MAIN_H

#include "Literal.h"

void initializeLiterals(Literal**, int);
void findUnitClauses(Literal**, int, int);
void unitPropogation(Literal**, int, int, int);
bool checkForContradiction(Literal**, int, int);
void orderArray(Literal**, int);
void print(Literal**, int);
int main(void);

#endif