#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h> 
// pow() function has been used, -lm option should be used for compilation


/*
structure definition for a implicant:
fields:
	literals : pointer to the character string of the alphabet {0,1,_}
	checked : it is 0 when one implicant is combined with another just upper or lower level in quine mccluskey method
'implicant' : new data type for the implicants
*/
struct node{
	char *literals;
	unsigned char checked;
};
typedef struct node implicant;

/*
this is a structure for consecutive implicants with same no  of 1's
fields:
	base : this is the start position of the block of implicants with same no of 1's
	limit : this is the size of the block of implicants with same no of 1's
implicantSegment : new data type for indexing a block of implicants
*/
struct segment{
	int base;
	int limit;
};
typedef struct segment implicantSegment;

struct term{
	int value;
	unsigned char dontCare;
};

typedef struct term minterm;

// input the boolean variables in the character array booleanVariables
void inputBooleanVariables(char *booleanVariables, int noOfVariables) {
/**
local variables:
boolVariableString:	in this char array all the boolean variables are taken as input separated by comma's
loopCounter and loopCounter2 : used as loop counter
boolVarStringLength :this should be the length of the input string including comma's and '\0'
boolVarInputStringLength : actual length of the input string
*/
	char *boolVariableString; 
	int loopCounter, loopCounter2 = 0, boolVarStringLength, boolVarInputStringLength;

	// boolVarStringLength should be two times the length of noOfVariables because extra space needed for comma's and '\0'
	boolVarStringLength = noOfVariables << 1;
	
	// allocate memory for boolVariableString of length boolVarStringLength
	boolVariableString = (char *) calloc(boolVarStringLength, sizeof(char));
	
	// input boolean variables
	printf("Enter the boolean variables as a string, separate them using ',' and press enter:\n");
	printf("There should not be any white spaces in the input, An example given below:\n");
	printf("if there are 3 variables and they are a,b and c, then input string should be like this:\n\na,b,c\nYour input:$");
	scanf("%s",boolVariableString);
	
	// calculate actual length of input string
	boolVarInputStringLength = strlen(boolVariableString);

	// boolVarInputStringLength length should be one less than boolVarStringLength excluding '\0' from boolVarInputStringLength
	if( (boolVarInputStringLength+1) != boolVarStringLength) {
		printf("you have entered invalid input, please enter exact no of variables by separating them using ','\n");
		exit(1);
	}
	
	// check if comma's have been entered alternatively in between the bool variables
	for(loopCounter = 1; loopCounter < (boolVarInputStringLength-1); loopCounter = loopCounter+2) {
		if(boolVariableString[loopCounter] != ',') {
			printf("you must separate variables using ','\n");
			exit(1);
		}
	}

	// copy only the bool variable character form string boolVariableString to booleanVariables
	for(loopCounter = 0 ; loopCounter < (noOfVariables); loopCounter++) {
		booleanVariables[loopCounter] = boolVariableString[loopCounter2];
		loopCounter2 = loopCounter2 + 2;
	}

	// free space allocated by boolVariableString, it is no longer needed
	free(boolVariableString);
}

// change a minterm value from integer to a character array of 1 and 0
void valueToLiterals(implicant *MintermImplicants, int noOfVariables, int minTermValue, int index){
	/*
	convert 'minTermValue' to a character array of 0's and 1's
	no of character will be same as no of variables
	here 'implicant' structure field 'checked' used as counter for no of 1's in that minterm
		1. shift the 'minTermValue' right by one
		2. check if shift out value is 1 or 0
		3. if that is 1 then increment counter for no of 1's in that implicant
		4. convert the shift out 0 or 1 to char and add that to minterm literal
		5. repeat (1) to (4) 'noOfVariables' times
	*/
	int loopCounter, shiftValue;
	implicant imp = MintermImplicants[index];
	for(loopCounter = noOfVariables - 1 ; loopCounter >= 0 ; loopCounter--) {
		if( (minTermValue & 1) == 1)
			MintermImplicants[index].checked++;
		(imp.literals)[loopCounter] = (minTermValue & 1) + '0';
		minTermValue >>= 1 ;
	}
}

// input the values for the implicants and store them to 'MintermImplicants' array
void inputMinterms(implicant *MintermImplicants, int noOfVariables, int noOfMinTerms, minterm *minterms) {
	int totalNoOfMinTerms, previousMintermValue,loopCounter, mintermValue ;
	char enter;
	totalNoOfMinTerms = 1 << noOfVariables;
	previousMintermValue = -1 ;
	printf("Enter all the minterms including don't care ones in strictly increasing order:\n");
	for(loopCounter = 0 ; loopCounter < noOfMinTerms ; loopCounter++) {
		printf("Enter the %d-th minterm:", loopCounter +1 );
		if( (scanf("%d%c", &mintermValue, &enter)) != 2 || enter != '\n' || mintermValue <= previousMintermValue ||mintermValue >=totalNoOfMinTerms ) {
			printf("You have entered invalid input for minterm !\n");
			exit(1);
		}
		valueToLiterals(MintermImplicants, noOfVariables, mintermValue, loopCounter);
		minterms[loopCounter].value = mintermValue ;
		minterms[loopCounter].dontCare = 0;
		previousMintermValue = mintermValue ;
	}
}

void inputDontCare(int noOfDontCare, minterm *minterms, int noOfVariables, int noOfMinTerms) {
	int loopCounter, dontCareTerm, previousMintermValue, totalNoOfMinTerms, loopCounter2;
	char enter;
	totalNoOfMinTerms = 1 << noOfVariables;
	printf("Enter the dont care minterms in strictly increasing order:\n");
	previousMintermValue = -1 ;
	for(loopCounter = 0 ; loopCounter < noOfDontCare ; loopCounter++) {
		printf("Enter the %d-th dont-care:", loopCounter +1 );
		if( (scanf("%d%c", &dontCareTerm, &enter)) != 2 || enter != '\n' || dontCareTerm <= previousMintermValue ||dontCareTerm >=totalNoOfMinTerms ) {
			printf("You have entered invalid input for don't care minterm !\n");
			exit(1);
		}
		for(loopCounter2 = 0 ; loopCounter2 < noOfMinTerms ; loopCounter2++) {
			if(minterms[loopCounter2].value == dontCareTerm)
				break;	
		}
		if(loopCounter2 == noOfMinTerms) {
			printf("Minterm dont found\n");
			exit(1);
		}
		minterms[loopCounter2].dontCare = 1;
		previousMintermValue = dontCareTerm ;
	}
}

// allocate memory for a list of implicants
implicant* allocateImplicants(int noOfImplicants, int noOfVariables) {
	implicant *implicantArray ;
	int  loopCounter;

	implicantArray = (implicant*) calloc(noOfImplicants, sizeof(implicant) );
	for(loopCounter = 0; loopCounter < noOfImplicants ; loopCounter++) {
		implicantArray[loopCounter].checked = 0;
		implicantArray[loopCounter].literals = (char *) calloc(noOfVariables, sizeof(char) );
	}
	return implicantArray;
}


// output leftside mathematical form of the function
void outputFunctionName(char *booleanVariables, int noOfVariables) {
	int loopCounter;
	printf("f(%c", booleanVariables[0]);
	for(loopCounter = 1; loopCounter < noOfVariables; loopCounter++)
		printf(",%c", booleanVariables[loopCounter]);
	printf(") = ");	
}


// show list of implicants
void showImplicants(implicant *mintermImplicants, int noOfImplicants, int noOfVariables) {
	int loopCounter, innerLoopCounter;
	for(loopCounter = 0 ; loopCounter < noOfImplicants ; loopCounter++) {
		printf("%d-th implicant:\n\t", loopCounter);
		for(innerLoopCounter = 0 ; innerLoopCounter < noOfVariables ; innerLoopCounter++)
			printf("%c", (mintermImplicants[loopCounter]).literals[innerLoopCounter]);
		printf("\n\tno of '1's:%d\n",(mintermImplicants[loopCounter]).checked);
	}
}

// allocate memory for 'index' according to no of variables
implicantSegment* allocateSegment(int noVariables) {
	implicantSegment *index;
	int loopCounter;
	index = (implicantSegment *) calloc( (noVariables+1), sizeof(implicantSegment) );
	for(loopCounter = 0; loopCounter <= noVariables; loopCounter++) {
		index[loopCounter].base = 0;
		index[loopCounter].limit = 0;
	}
	return index;
}

// sort the minterm implicants according to no  of 1's and copy them to 'allImplicants'
void sortImplicants(implicant *allImplicants, implicant *mintermImplicants, int noOfMinTerms, implicantSegment *index, int noOfVariables,minterm *minterms, minterm *sortedMinterm) {
	int outerloopCounter, innerloopCounter, implicantCounter = 0;
	for(outerloopCounter = 0; outerloopCounter <= noOfVariables ; outerloopCounter++) {
		for(innerloopCounter = 0; innerloopCounter < noOfMinTerms; innerloopCounter++) {
			if( (mintermImplicants[innerloopCounter].checked) == outerloopCounter ) {
				allImplicants[implicantCounter] = mintermImplicants[innerloopCounter] ;
				sortedMinterm[implicantCounter] = minterms[innerloopCounter];
				index[outerloopCounter].limit = 1;
				index[outerloopCounter].base = implicantCounter;
				implicantCounter++;
				innerloopCounter++;
				break;
			}
		}
		for(; innerloopCounter < noOfMinTerms; innerloopCounter++) {
			if( (mintermImplicants[innerloopCounter].checked) == outerloopCounter ) {
				allImplicants[implicantCounter] = mintermImplicants[innerloopCounter] ;
				sortedMinterm[implicantCounter] = minterms[innerloopCounter];
				index[outerloopCounter].limit++;
				implicantCounter++;
			}
		}
	}
}

// unchedk a list of implicants
void uncheckAllImplicants(implicant *allImplicants, int noOfMinTerms) {
	int loopCounter;
	for(loopCounter = 0; loopCounter <= noOfMinTerms; loopCounter++)
		allImplicants[loopCounter].checked = 0;
}


//combine two implicant and return the combined implicant
// if combine is not successfull then the return implicant checked value is 2
implicant combineImplicant(implicant *allImplicants, int index1, int index2, int noOfVariables) {
	int loopCounter, bitMisMatched = 0;
	char *literal1, *literal2;
	implicant implicant1,implicant2,newimplicant;

	implicant1 = allImplicants[index1];
	implicant2 = allImplicants[index2];

	literal1 = implicant1.literals;
	literal2 = implicant2.literals;

	newimplicant.literals = (char *) calloc(noOfVariables, sizeof(char) );

	newimplicant.checked = 0;
	for(loopCounter = 0; loopCounter < noOfVariables; loopCounter++) {
		if( (literal1[loopCounter] == '_' ) ^ (literal2[loopCounter] == '_' ) ) {
			newimplicant.checked = 2;
			return newimplicant;
		}
		if(bitMisMatched == 1 && literal1[loopCounter] != literal2[loopCounter]) {
			newimplicant.checked = 2;
			return newimplicant;
		}
		if(literal1[loopCounter] != literal2[loopCounter] && bitMisMatched == 0) {
			bitMisMatched = 1;
			newimplicant.literals[loopCounter] = '_';
		}
		else {
			newimplicant.literals[loopCounter] = literal1[loopCounter];
		}
	}

	allImplicants[index1].checked = 1;
	allImplicants[index2].checked = 1;

	return 	newimplicant;
}


// check if the newly generated combined implicant is already present in the list of combined implicant
int findInImplicant(implicant *allImplicants ,  implicant newimplicant, int base, int limit, int noOfVariables) {
	int loopCounter, innerloopCounter;
	char  *tempimplicantLiteral,*newimplicantLiteral;
	newimplicantLiteral = newimplicant.literals;
	for(loopCounter = base ; loopCounter < limit ; loopCounter++) {
		tempimplicantLiteral = allImplicants[loopCounter].literals;
			for(innerloopCounter = 0; innerloopCounter < noOfVariables; innerloopCounter++)
				if(newimplicantLiteral[innerloopCounter] != tempimplicantLiteral[innerloopCounter])
					break;
		if(innerloopCounter == noOfVariables)
			return 1;
	}
	return 0;
}

// this is the method where all things happenig
// combine two block of implicant with no of 1's difference is one
void reduceImplicants(implicant *allImplicants, int noOfVariables, int *noOfImplicants, implicantSegment *index) {
	int loopCounter1, loopCounter2, loopCounter3, loopCounter4;
	int limit1, limit2, limit3, limit4, found;
	int tempBase,tempLimit;
	implicant newimplicant;

	for(loopCounter1 = 1; loopCounter1 < noOfVariables; loopCounter1++) {
		limit2 = noOfVariables-loopCounter1+1 ;
		for(loopCounter2 = 0; loopCounter2 < limit2 ; loopCounter2++) {
			loopCounter3 = index[loopCounter2].base ;
			limit3 = loopCounter3 + index[loopCounter2].limit ;
			index[loopCounter2].base = (*noOfImplicants);
			index[loopCounter2].limit = 0;
			
			for(; loopCounter3 < limit3; loopCounter3++){
				loopCounter4 = index[loopCounter2+1].base;
				limit4 = loopCounter4 + index[loopCounter2+1].limit ;
				for(; loopCounter4 < limit4; loopCounter4++) {
					newimplicant = combineImplicant(allImplicants, loopCounter3, loopCounter4, noOfVariables);
					if( newimplicant.checked == 0) {
						tempBase = index[loopCounter2].base;
						tempLimit = tempBase + index[loopCounter2].limit;
						found = findInImplicant(allImplicants , newimplicant,tempBase, tempLimit, noOfVariables);
						if(found == 0) {
							allImplicants[*noOfImplicants] = newimplicant;
							(*noOfImplicants)++;
							index[loopCounter2].limit++;
						}
					}
				}
			}
		}
	}
}

// list all the prime implicants , actually they are unchecked
void getPrimeImplicants(implicant *allImplicants, int noOfImplicants, implicant *primeImplicants, int *noOfPrimeImplicants) {
	int loopCounter;
	for(loopCounter = 0; loopCounter < noOfImplicants; loopCounter++) { 
		if(allImplicants[loopCounter].checked == 0) {
			primeImplicants[*noOfPrimeImplicants] = allImplicants[loopCounter];
			(*noOfPrimeImplicants)++;
		}
	}
	
}

// check if implicant 'literal1 ' covers minterm 'literal2' or not
int covers(char *literal1, char *literal2, int noOfVariables) {
	int loopCounter;
	for(loopCounter = 0; loopCounter < noOfVariables; loopCounter++)
		if(literal1[loopCounter] != '_' && literal1[loopCounter] != literal2[loopCounter] ) 
			return 0;
	return 1;
}

// print a implicant represented by bool variables
void printImplicant(char *literal, int noOfVariables, char *booleanVariables) {
	int loopCounter;
	for(loopCounter = 0; loopCounter < noOfVariables; loopCounter++) {
		if(literal[loopCounter] == '0')
			printf("%c\'", booleanVariables[loopCounter]);
		else if(literal[loopCounter] == '1')
			printf("%c", booleanVariables[loopCounter]);
	}
	printf("\t");
}

// show the table of the prime implicants
void showImplicantTable(char **implicantTable, int noOfPrimeImplicants, int noOfMinTerms,minterm *sortedMinterm, implicant *primeImplicants,int noOfVariables, char *booleanVariables) {
	int loopCounter, innerLoopCounter;
	printf("Minterms->\t");
	for(loopCounter = 0; loopCounter < noOfMinTerms; loopCounter++)
		printf("%d\t", sortedMinterm[loopCounter].value);
	printf("\n\n");
	for(loopCounter = 0; loopCounter < noOfPrimeImplicants; loopCounter++) {
		printImplicant(primeImplicants[loopCounter].literals, noOfVariables, booleanVariables);
		printf("\t");
		for(innerLoopCounter = 0; innerLoopCounter < noOfMinTerms; innerLoopCounter++)
			printf("%d\t", implicantTable[loopCounter][innerLoopCounter]);
		printf("\n\n");
	}
	printf("\n\n");
}

// make the implicant table
void makeImplicantTable(char **implicantTable, implicant *MintermImplicants, implicant *primeImplicants, int noOfMinTerms, int noOfPrimeImplicants, int noOfVariables) {
	int loopCounter, innerLoopCounter;
	for(loopCounter = 0; loopCounter < noOfPrimeImplicants; loopCounter++) {
		for(innerLoopCounter = 0; innerLoopCounter < noOfMinTerms; innerLoopCounter++) {
		if( covers(primeImplicants[loopCounter].literals, MintermImplicants[innerLoopCounter].literals, noOfVariables) == 1 ) 
				implicantTable[loopCounter][innerLoopCounter] = 1;	
			else
				implicantTable[loopCounter][innerLoopCounter] = 0;
		}
	}
}

// allocate memory for implicant table
char **allocateTable(int noOfPrimeImplicants, int noOfMinTerms) {
	char **implicantTable;
	int loopCounter ;
	implicantTable = (char **) calloc(noOfPrimeImplicants, sizeof(char*));
	for(loopCounter = 0; loopCounter < noOfPrimeImplicants; loopCounter++)
		implicantTable[loopCounter] = (char *) calloc(noOfMinTerms, sizeof(char));
	return implicantTable;
}

// find all the essential and non-essential prime impliants
void findEssentialImplicants(implicant *primeImplicants, implicant *MintermImplicants, char **implicantTable, int noOfPrimeImplicants, int noOfMinTerms, int noOfVariables, char *booleanVariables, minterm *sortedMinterms) {

	int loopCounter, innerLoopCounter, coverCounter, primeImplicantNo, nonessentialCounter;
	char *essentialMarker;
	essentialMarker = (char*) calloc(noOfPrimeImplicants, sizeof(char));
	for(loopCounter = 0; loopCounter < noOfMinTerms; loopCounter++)
		essentialMarker[loopCounter] = 0;
	printf("List of essential prime implicants:\n");
	for(loopCounter = 0; loopCounter < noOfMinTerms; loopCounter++) {
		coverCounter = 0 ;
		for(innerLoopCounter = 0 ; innerLoopCounter < noOfPrimeImplicants ; innerLoopCounter ++) {
			if(implicantTable[innerLoopCounter][loopCounter] == 1) {
				coverCounter++;
				primeImplicantNo = innerLoopCounter;
			}
		}
		
		if(coverCounter == 1 && sortedMinterms[loopCounter].dontCare == 0) {
			essentialMarker[primeImplicantNo] = 1;
			printImplicant(primeImplicants[primeImplicantNo].literals, noOfVariables, booleanVariables);
			for(innerLoopCounter = 0 ; innerLoopCounter < noOfMinTerms ; innerLoopCounter ++) {
				if(implicantTable[primeImplicantNo][innerLoopCounter] == 1)
					for(coverCounter = 0;coverCounter < noOfPrimeImplicants; coverCounter++)
						implicantTable[coverCounter][innerLoopCounter] = 0;
			}
		}
	}
	
	for(loopCounter = 0 ; loopCounter < noOfMinTerms ; loopCounter ++) { 
		if(sortedMinterms[loopCounter].dontCare == 1 )
			for(innerLoopCounter = 0 ; innerLoopCounter < noOfPrimeImplicants ; innerLoopCounter ++)
				implicantTable[innerLoopCounter][loopCounter] = 0;
	}
	
	nonessentialCounter = 0;
	printf("\n\nList of Non-essential prime implicants:\n");
	for(loopCounter = 0; loopCounter < noOfPrimeImplicants; loopCounter++) {
		if( essentialMarker[loopCounter] == 0 ) {
			nonessentialCounter++;
			printImplicant(primeImplicants[loopCounter].literals, noOfVariables, booleanVariables);
			printf("covers minterms :\n\t");
			for(innerLoopCounter = 0 ; innerLoopCounter < noOfMinTerms ; innerLoopCounter ++) {
				if(implicantTable[loopCounter][innerLoopCounter] == 1)
					printImplicant(MintermImplicants[innerLoopCounter].literals, noOfVariables, booleanVariables);
			}
		}
		printf("\n");
	}
	if(nonessentialCounter == 0)
			printf("\tNo non-essential prime implicants needed\n");
	printf("\n\n");
}


// main function
int main() {
	/*
	List of variables in main() method :
	loopCounter : used as a counter vairiable
	noOfVariables : the number of boolean variables for the boolean function
	totalNoOfMinTerms : keeps maximum no of minterms possible for the specified no of variables
	noOfMinTerms : no of minterms contained in the boolean function
	noOfImplicants : keeps no of all implicants generated in the quine mccluskey method
	noOfPrimeImplicants : no of all prime implicants generated in the quine mccluskey method
	enter : just a character for varifying input
	booleanVariables : this is a character array that contains all boolean variables
	implicantTable : a 2D character array that contains the implicant table
	MintermImplicants : array of all the minterm implicants taken as minterm value from the user
	allImplicants : an array of all types of implicants generated through quine mccluskey method
	primeImplicants : an array of all prime implicants generated through quine mccluskey method
	index : an array of base and limit values of the implicants in an iteration partitioned according to no of 1's
	*/
	int loopCounter;
	int noOfVariables, totalNoOfMinTerms, noOfMinTerms, noOfImplicants, noOfPrimeImplicants, noOfDontCare ;
	char enter, *booleanVariables, **implicantTable;
	implicant *mintermImplicants, *allImplicants, *primeImplicants;
	implicantSegment *index;
	minterm *minterms, *sortedMinterm;
	
	// input no of boolean variables
	printf("Enter the total number of boolean variables for the boolean function and press enter key :$");
	if( (scanf("%d%c", &noOfVariables, &enter)) != 2 || enter != '\n' || noOfVariables <= 0 ) {
			printf("\nYou have entered invalid input for no of variables, enter a positive integer value !\n");
			return;
	}
	
	//allocate mrmory for character array of bool variables
	booleanVariables = (char *) calloc( noOfVariables, sizeof(char) ); 
	
	// input all the bool variables in the char array booleanVariables through this function call
	inputBooleanVariables(booleanVariables, noOfVariables);
	
	// max possible minterms 2^n, where n is the no of variable
	totalNoOfMinTerms = (1 << noOfVariables) ;


	// input no of minterms including the dont care ones
	printf("How many minterms are there in your boolean function including the don't care ones ?(press enter key after the value):$");
	if( (scanf("%d%c", &noOfMinTerms, &enter)) != 2 || enter != '\n' || noOfMinTerms < 0 ||noOfMinTerms > totalNoOfMinTerms) {
			printf("You have entered invalid input for no of minterms, enter a non-negative integer value not greater than maximum minterms.\n");
			return;
	}
	
	// if no minterm in the function then the function is 0
	if(noOfMinTerms == 0) {
		printf("\nYour boolean function always false !\n");
		outputFunctionName(booleanVariables, noOfVariables);
		printf("0\n");
		return;
	}

	// if all the minterms are there  in the function, the function is 1
	if(noOfMinTerms == totalNoOfMinTerms) {
		printf("\nYour boolean function always true !\n");
		outputFunctionName(booleanVariables, noOfVariables);
		printf("1\n");
		return;
	}
	
	// at first total no of implicants is same as no of input minters
	noOfImplicants = noOfMinTerms ;
	
	// allocate memory for the minterm implicants according to no of variables
	mintermImplicants = allocateImplicants(noOfMinTerms, noOfVariables);
	
	
	
	printf("How many don't care minterms are there in your boolean function?(press enter key after the value):$");
	if( (scanf("%d%c", &noOfDontCare, &enter)) != 2 || enter != '\n' || noOfDontCare < 0 ||noOfDontCare > noOfMinTerms) {
			printf("You have entered invalid input for no of dont cares, enter a non-negative integer value less than no of minterms !\n");
			return;
	}
	
	minterms = (minterm*) calloc(noOfMinTerms, sizeof(minterm));
	sortedMinterm = (minterm*) calloc(noOfMinTerms, sizeof(minterm));
	
	// input all the minterms and store them in mintermImplicants array
	inputMinterms(mintermImplicants, noOfVariables, noOfMinTerms, minterms);
	
	if (noOfDontCare > 0)
		inputDontCare(noOfDontCare, minterms, noOfVariables, noOfMinTerms);
	
	// allocate memory for the array 'index ' according to no of variables
	index = allocateSegment(noOfVariables);

	//showImplicants(mintermImplicants,noOfMinTerms, noOfVariables);

	/* 
	allocate memory for the array 'allImplicants', no of max elements in the
	array 3^n because an implicant is a string of the alphabet {0,1,_}
	*/
	allImplicants = allocateImplicants( ((int) pow(3,noOfVariables)), noOfVariables );

	// sort the implicants of 'mintermImplicants' according to there no of 1's an copy them to 'allImplicants'
	sortImplicants(allImplicants,mintermImplicants, noOfMinTerms, index, noOfVariables,minterms, sortedMinterm);
	
	// uncheck all the implicants before starting iteration of quine mccluskey 
	uncheckAllImplicants(allImplicants, noOfMinTerms);
	
	// combine implicants of adjacent block, where diff of no of 1's in this two block is one until no more reduction 
	reduceImplicants(allImplicants, noOfVariables, &noOfImplicants, index);

	// use mintermImplicants array as the array for prime implicants(no more memory allocation needed)
	primeImplicants = mintermImplicants;
	
	// set no of prime implicants to zero
	noOfPrimeImplicants = 0;
	
	// copy all the prime implicants from array 'allImplicants' to array 'primeImplicants'
	getPrimeImplicants(allImplicants, noOfImplicants, primeImplicants, &noOfPrimeImplicants);
	
	// the implicant arise first in 'allImplicants' array is actually minterm implicants
	mintermImplicants = allImplicants ;
	
	// allocate memory for prime implicants table
	implicantTable = allocateTable(noOfPrimeImplicants, noOfMinTerms);

	// make the prime implicants table
	makeImplicantTable(implicantTable, mintermImplicants, primeImplicants, noOfMinTerms, noOfPrimeImplicants, noOfVariables);
	
	// show the prime implicants table
	printf("\nshowing the primary implicant table:\n");
	showImplicantTable(implicantTable, noOfPrimeImplicants, noOfMinTerms, sortedMinterm, primeImplicants, noOfVariables, booleanVariables); 

	// find the essential prime implicant from implicant table
	findEssentialImplicants(primeImplicants, mintermImplicants, implicantTable, noOfPrimeImplicants, noOfMinTerms,noOfVariables, booleanVariables, sortedMinterm);
	
	// show the modified implicants table for non-essential prime implicants
	printf("modified implicants table for non-essential prime implicants:\n");
	showImplicantTable(implicantTable, noOfPrimeImplicants, noOfMinTerms, sortedMinterm,primeImplicants, noOfVariables, booleanVariables); 
	
	printf("\n\n");

	free(booleanVariables) ;
	free(mintermImplicants) ;
	free(primeImplicants);
	free(index);
	free(implicantTable);
	free(minterms);
	free(sortedMinterm);
	return 0;
}
