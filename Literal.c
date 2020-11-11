#include "Literal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//Getter for the character
char* getLiteral(Literal *this) {
	return this->ch;
}

//Setter for the character
void setLiteral(Literal *this, char* ch) {
	this->ch = ch;
}

//Getter for the sign
bool getSign(Literal *this) {
	return this->sign;
}

//Setter for the sign
void setSign(Literal *this, bool sign) {
	this->sign = sign;
}

//Getter for the clause number
int getClause(Literal *this) {
	return this->clause;
}

//Setter for the clause number
void setClause(Literal *this, int clause) {
	this->clause = clause;
}

//Prints out the literal
void printLiteral(Literal *this) {
	printf("%s, %d, %d\n", this->ch, this->sign, this->clause);
}

//Structure for literal
Literal *new_Literal(char* ch, bool sign, int clause) {
	Literal *this = malloc(sizeof(Literal));
	this->ch = ch;
    this->sign = sign;
	this->clause = clause;
	this->getLiteral = getLiteral;
	this->setLiteral = setLiteral;
    this->getSign = getSign;
	this->setSign = setSign;
	this->getClause = getClause;
	this->setClause = setClause;
	this->printLiteral = printLiteral;
	return this;
}