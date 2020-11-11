#include "Main.h"
#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Method called which fills the literalArray with the litterals inside of the std input
void initializeLiterals(Literal **literalArray, int literalCounter){

    char line[50]; //One line of the std input
    int clauseCounter = 1;//Counter used to count the number of clauses read in
    char* token; //Used to store a literal upon the use of strtok
    char* ch; //Used to store a duplicate of the token. required to stop overrides
    
    //Loops for as long as lines containing a clause are being read in
    while (fgets(line, sizeof(line), stdin) != NULL){

        size_t length = strlen(line);//Gets the length of the line for the removal of \n
        if (line[0] == '\n'){//Checks to see if the first character in the line is a \n a.k.a an empty clause
            printf("-"); //Contradiction is shown
            exit(EXIT_SUCCESS);//Execution is stopped
        }
        if (line[length - 1] == '\n'){ //Checks to see if the last character in the line is a \n
            line[length - 1] = '\0'; //Replaces the /n with an /0
        }
        const char s[2] = " ";  //Constant where each clause is split into individual literals 
        token = strtok(line, s); // Get the first token 
        bool sign;//Used to track the sign of the literal
        //Loops for as long as there are literals
        while( token != NULL ) {
            size_t tokenLength = strlen(token);//Gets the length of a literal
            if (token[0] == '-'){ //Checks to see if a literal is negative and assigns the boolean as such
                sign = true;
                for(int i = 0; i < tokenLength; i++){
                    token[i] = token[i+1];//Gets the literal name without the '-'
                }
            }else{
                sign = false;
            }
            for(int i = 0; i < tokenLength; i++){
                if(token[i] == '-'){//Checks to see if there's a '-' anywhere else other than the start of the literal
                    printf("Invalid input. '-' present.");//If so then this is an invalid literal name. An error is output
                    exit(EXIT_SUCCESS);//Execution is stopped
                }
            }
            ch = strdup(token);//Assigns the token to a seperate variable to stop overrides
            literalArray[literalCounter] = new_Literal(ch, sign, clauseCounter);//Adds the literal and its data to the array
            literalCounter++;//New literal
            token = strtok(NULL, s);//Gets new token/literal
        }
        clauseCounter++;//New clause
    }
    findUnitClauses(literalArray, literalCounter, clauseCounter);//Runs the method that finds any unit clauses
    print(literalArray, literalCounter);//Runs the method that sends the output to std out
    free(token);//Frees memory for the token variable
    free(ch);//Frees memory for the ch variable
}

//Method that finds any unit clauses by seeing if the literals beside the potential unit clause are from different clauses
void findUnitClauses(Literal **literalArray, int literalCounter, int clauseCounter){

    int unitClause;//Used to track the literal that is a unit clause

    //For each literal in the array
    for(int i = 0; i < literalCounter; i++){
        bool unitClauseFound = true;
        if ((literalArray[i]->getClause(literalArray[i])) < 0){//Checks to see if the literal being analysed has already been dealt with
            unitClauseFound = false; //Not a new unit clause
        }else{
            //If the literal hasn't been analysed then it is compared with every other literal
            for(int j = 0; j < literalCounter; j++){
                //If the literal is on the same clause as another literal of a different type then its not a unit clause
                if ((literalArray[i]->getClause(literalArray[i])) == (literalArray[j]->getClause(literalArray[j])) && (literalArray[i]->getLiteral(literalArray[i])) != (literalArray[j]->getLiteral(literalArray[j]))){
                    unitClauseFound = false;
                }
            }
        }

        //If a unit clause has been found then the tracker int is assigned a value and the method that performs unit propogation is called
        if (unitClauseFound){
            unitClause = i;
            unitPropogation(literalArray, literalCounter, unitClause, clauseCounter);
        }
        
    }
}

//Method that performs unit propogation by finding literals of the same type but of different signs and removing them to a negative clause number, the unit clause itself
//is then removed to a different negative clause
void unitPropogation(Literal **literalArray, int literalCounter, int unitClause, int clauseCounter){
    
    int clause;//Int used to track the clause number of a clause undergoing unit propogation to see if an empty clause is produced
    
    //For every literal in the array
    for(int i = 0; i < literalCounter; i++){
        int comparison = strcmp(literalArray[i]->getLiteral(literalArray[i]), literalArray[unitClause]->getLiteral(literalArray[unitClause]));//Int used to compare literal and unit clause
        if ((comparison == 0) && (literalArray[i]->getSign(literalArray[i])) != (literalArray[unitClause]->getSign(literalArray[unitClause]))){//If literals are the same and they have a different sign
            //The literal's sign is flipped
            if((literalArray[unitClause]->getSign(literalArray[unitClause])) ==  0 ){
                literalArray[i]->setSign(literalArray[i], 0);
            }else{
                literalArray[i]->setSign(literalArray[i], 1);
            }
            clause = literalArray[i]->getClause(literalArray[i]);//Clause variable is assigned the clause value of the literal being removed
            literalArray[i]->setClause(literalArray[i], -2);//Literal is removed from clause and given a negative clause value - analysis finished 
            
            //If a contradiction occurs (checked by running the contradiction checker that returns a boolean) then the contradiction output is displayed and execution is stopped
            if(checkForContradiction(literalArray, literalCounter, clause)){
                printf("-");
                exit(EXIT_SUCCESS);
            } 
        }
    }

    bool duplicate = false;//Boolean used to track that a unit clause of the same type hasn't already been logged as a unit clause to stop the same unit clause being printed multiple times.
    //For every literal
    for(int i = 0; i < literalCounter; i++){
        int comparison = strcmp(literalArray[i]->getLiteral(literalArray[i]), literalArray[unitClause]->getLiteral(literalArray[unitClause]));//Int used to compare literal and unit clause
        //If the literal and unit clause are the same and the literal is already assigned as a unit clause then a duplicate has occured
        if((comparison == 0) && (literalArray[i]->getClause(literalArray[i]) == -1)){
            duplicate = true;
        }
    }

    //If its a duplicate the unit clause is assigned the discard clause, else it is assigned the clause used to identify all unit clauses
    if(duplicate){
        literalArray[unitClause]->setClause(literalArray[unitClause], -2);
    }else{
        literalArray[unitClause]->setClause(literalArray[unitClause], -1);
    }
     
    //Runs the method to find any unit clauses again
    findUnitClauses(literalArray, literalCounter, clauseCounter);

}

//Method used to check if there's a contradiction by seeing if, before the unit clause is assigned to its negative clause, it is the only unit clause left
bool checkForContradiction(Literal **literalArray, int literalCounter, int clause){

    bool contradiction = true;
    //For every literal
    for(int i = 0 ; i < literalCounter ; i++){
        //Checks to see if another literal is on the same clause as the removed literal meaning the clause isn't empty
        if(literalArray[i]->getClause(literalArray[i]) == clause){
            contradiction = false;
        }
    }
    return contradiction;//Returns the result of the contradiction boolean
}

//Method that order the literals into lexicographical order by use of a bubble sort
void orderArray(Literal **literalArray, int literalCounter){

    for (int i = 0; i < literalCounter; i++){//Counter tracks the literal that will be moved
        for (int j = i+1; j < literalCounter; j++){//Counter tracks the literal's neighbour that will swap with it
            int comparison = strcmp(literalArray[i]->getLiteral(literalArray[i]), literalArray[j]->getLiteral(literalArray[j]));//Int used to compare neighbours
            if(comparison > 0){//Checks to see if literal comes after its neighbour alphabetically
            //Temp variables
            char* tempLiteral = literalArray[i]->getLiteral(literalArray[i]);
            bool tempSign = literalArray[i]->getSign(literalArray[i]);
            int tempClause = literalArray[i]->getClause(literalArray[i]);
            //Swapping data around
            literalArray[i]->setLiteral(literalArray[i], literalArray[j]->getLiteral(literalArray[j]));
            literalArray[i]->setSign(literalArray[i], literalArray[j]->getSign(literalArray[j]));
            literalArray[i]->setClause(literalArray[i], literalArray[j]->getClause(literalArray[j]));
            literalArray[j]->setLiteral(literalArray[j], tempLiteral);
            literalArray[j]->setSign(literalArray[j], tempSign);
            literalArray[j]->setClause(literalArray[j], tempClause);
       }
     }
   }
}

void print(Literal **literalArray, int literalCounter){
    
    orderArray(literalArray, literalCounter);//Runs the method that puts the literals into lexicographical order

    int numberOfClauses = 0;//Used to track the number of unit clauses
    int numberOfClausesPrinted = 0;//Used to track the number of unit clauses that have been printed so that only the right amount are printed

    //For every literal, it checks if it is a unit clause, in which case the unit clause counter is incremented.
    for(int i = 0; i <literalCounter; i++){
        if((literalArray[i]->getClause(literalArray[i])) == -1){
            numberOfClauses++;
        }
    }

    //For every literal in the array
    for(int i = 0; i <literalCounter; i++){
        if((literalArray[i]->getClause(literalArray[i])) == -1){//Checks to see if the literal is a unit clause
            if(numberOfClausesPrinted < numberOfClauses-1){//Checks to see that there are still more unit clauses left to be printed
                if((literalArray[i]->getSign(literalArray[i])) ==  0 ){//Checks to see what the sign of the literal is and prints
                    printf("%s ", literalArray[i]->getLiteral(literalArray[i]));
                    numberOfClausesPrinted++;
                }else{
                    printf("-%s ", literalArray[i]->getLiteral(literalArray[i]));
                    numberOfClausesPrinted++;
                }
            }else{//If this is the last unit clause to be printed then it is printed with no space beside it
                if((literalArray[i]->getSign(literalArray[i])) ==  0 ){
                    printf("%s", literalArray[i]->getLiteral(literalArray[i]));
                    numberOfClausesPrinted++;
                }else{
                    printf("-%s", literalArray[i]->getLiteral(literalArray[i]));
                    numberOfClausesPrinted++;
                }
            }
        }
    }
}

int main(void) {

    Literal **literalArray  = malloc(100 * sizeof(Literal));//Array of type Literal which stored every literal encountered in the std input
    int literalCounter = 0;//Counter used to track the number of literals in the LiteralArray

    initializeLiterals(literalArray, literalCounter);//Runs the method that fills the array of literals
    free(literalArray);//Frees the memory that is used to store the array of literals
    
    return 0;
}