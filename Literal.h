#ifndef LITERAL_H
#define LITERAL_H 
#include <stdbool.h>


typedef struct Literal Literal;

typedef char* (*Literal_getLiteral_t)(Literal *);
typedef void (*Literal_setLiteral_t)(Literal *, char*);
typedef bool (*Literal_getSign_t)(Literal *);
typedef void (*Literal_setSign_t)(Literal *, bool);
typedef int (*Literal_getClause_t)(Literal *);
typedef void (*Literal_setClause_t)(Literal *, int);
typedef void (*Literal_printLiteral_t)(Literal *);


typedef struct Literal{
	char* ch;
    bool sign;
	int clause;
	Literal_getLiteral_t getLiteral;
	Literal_setLiteral_t setLiteral;
    Literal_getSign_t getSign;
	Literal_setSign_t setSign;
	Literal_getClause_t getClause;
	Literal_setClause_t setClause;
	Literal_printLiteral_t printLiteral;
};

Literal* new_Literal(char*, bool, int);

#endif