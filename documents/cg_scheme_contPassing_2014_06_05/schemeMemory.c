#include "scheme.h"

OBJ
schemeAlloc(unsigned sizeInBytes) {
    return (OBJ) malloc(sizeInBytes);
}

OBJ
newSchemeFileInputStream() {
    OBJ newStream;

    // printf("sizeof header = %d\n", sizeof(schemeHeader));
    // printf("sizeof struct schemeFileInputStream = %d\n", sizeof(struct schemeFileInputStream));
    // printf("offset of inputfile in schemeFileInputStream = %d\n",
    //             &( ((OBJ)0)->schemeFileInputStream.inputFile));

    newStream = schemeAlloc( sizeof(struct schemeFileInputStream) );
    newStream->header.tag = TAG_FILEINPUTSTREAM;
    newStream->schemeFileInputStream.lookAheadChar = NO_PEEK_CHAR;
    return newStream;
}

OBJ
newSchemeInteger(INT iVal) {
    OBJ newInteger;

    if ((iVal >= minSmallInteger)
     && (iVal <= maxSmallInteger)) {
	return makeSmallInteger(iVal);
    }

    newInteger = schemeAlloc( sizeof(struct schemeInteger));
    newInteger->schemeInteger.intVal = iVal;
    newInteger->header.tag = TAG_INTEGER;
    return newInteger;
}

OBJ
newSchemeCons(OBJ car, OBJ cdr) {
    OBJ newCons;

    newCons = schemeAlloc( sizeof(struct schemeCons));
    newCons->schemeCons.car = car;
    newCons->schemeCons.cdr = cdr;
    newCons->header.tag = TAG_CONS;
    return newCons;
}

OBJ
newSchemeStringL(char *theChars, int numChars) {
    OBJ newString;

    newString = schemeAlloc( sizeof(struct schemeString)
			     + numChars+1 - NUM_DUMMY_ALLOC );
    memcpy(newString->schemeString.characters, theChars, numChars+1);
    newString->schemeString.size = numChars;
    newString->header.tag = TAG_STRING;
    return newString;
}

OBJ
newSchemeString(char *theChars) {
    newSchemeStringL(theChars, strlen(theChars));
}

/* static */ OBJ *knownSymbols = 0;
/* static */ int numKnownSymbols = 0;
/* static */ int sizeOfKnownSymbols = 0;

void
initializeSymbolTable() {
    knownSymbols = (OBJ *)malloc(sizeof(OBJ) * INITIAL_SYMBOL_TABLE_SIZE);
    numKnownSymbols = 0;
    sizeOfKnownSymbols = INITIAL_SYMBOL_TABLE_SIZE;
}

OBJ static
searchSymbol(char *charsToSearch)
{
    int i;

    for (i=0; i<numKnownSymbols; i++) {
	OBJ oldSym = knownSymbols[i];

	if (strcmp(oldSym->schemeSymbol.characters, charsToSearch) == 0) {
	    return oldSym;
	}
    }
    return (OBJ)0;
}

void static
rememberSymbol(OBJ theSymbol) {
    if (numKnownSymbols >= sizeOfKnownSymbols) {
	int newSize = sizeOfKnownSymbols * 2;

	knownSymbols = (OBJ *)realloc(knownSymbols, sizeof(OBJ) * newSize);
	sizeOfKnownSymbols = newSize;
    }
    knownSymbols[numKnownSymbols++] = theSymbol;
}

OBJ
newSchemeSymbol(char *theChars) {
    OBJ newSymbol, oldSymbol;
    int numChars = strlen(theChars);

    // if such a symbol already exists...
    if ((oldSymbol = searchSymbol(theChars)) != 0) {
	return oldSymbol;
    }

    newSymbol = schemeAlloc( sizeof(struct schemeSymbol)
			     + numChars+1 - NUM_DUMMY_ALLOC );
    memcpy(newSymbol->schemeSymbol.characters, theChars, numChars+1);
    newSymbol->schemeSymbol.size = numChars;
    newSymbol->header.tag = TAG_SYMBOL;
    rememberSymbol(newSymbol);
    return newSymbol;
}

OBJ
newSchemeVector(int numElements) {
    OBJ newVector;
    int i;

    newVector = schemeAlloc( sizeof(struct schemeVector)
			     + (sizeof(OBJ) * (numElements - NUM_DUMMY_ALLOC))
			   );
    newVector->header.tag = TAG_VECTOR;
    newVector->schemeVector.numElements = numElements;
    for (i=0; i<numElements; i++) {
	newVector->schemeVector.elements[i] = SCHEME_NIL;
    }
    return newVector;
}

OBJ
newSchemeSingleton(schemeTag tag) {
    OBJ newAny;

    newAny = schemeAlloc( sizeof(schemeHeader));
    newAny->header.tag = tag;
    return newAny;
}

OBJ
newSchemeEnvironment(int numElements) {
    OBJ newEnvironment;
    OBJ keysAndValues;

    keysAndValues = newSchemeVector(numElements);

    newEnvironment = schemeAlloc( sizeof(struct schemeEnvironment));
    newEnvironment->header.tag = TAG_ENVIRONMENT;
    newEnvironment->schemeEnvironment.tally = 0;
    newEnvironment->schemeEnvironment.keysAndValues = keysAndValues;
    newEnvironment->schemeEnvironment.parentEnvironment = SCHEME_NIL;
    return newEnvironment;
}

OBJ
newSchemeBuiltinFunction(CONTFUNC func, char *name, int nArgs) {
    OBJ newFunc;

    newFunc = schemeAlloc( sizeof(struct schemeBuiltinFunction));
    newFunc->header.tag = TAG_BUILTINFUNCTION;
    newFunc->schemeBuiltinFunction.code = func;
    newFunc->schemeBuiltinFunction.name = name;
    newFunc->schemeBuiltinFunction.numArgs = nArgs;
    return newFunc;
}

OBJ
newSchemeBuiltinSyntax(CONTFUNC func, char *name, int nArgs) {
    OBJ newFunc;

    newFunc = schemeAlloc( sizeof(struct schemeBuiltinFunction));
    newFunc->header.tag = TAG_BUILTINSYNTAX;
    newFunc->schemeBuiltinSyntax.code = func;
    newFunc->schemeBuiltinSyntax.name = name;
    newFunc->schemeBuiltinSyntax.numArgs = nArgs;
    return newFunc;
}

OBJ
newSchemeUserDefinedFunction(OBJ lambdaArgList, OBJ bodyList, OBJ env) {
    OBJ newFunc;

    newFunc = schemeAlloc( sizeof(struct schemeUserDefinedFunction));
    newFunc->header.tag = TAG_USERDEFINEDFUNCTION;
    newFunc->schemeUserDefinedFunction.argList = lambdaArgList;
    newFunc->schemeUserDefinedFunction.bodyList = bodyList;
    newFunc->schemeUserDefinedFunction.environment = env;
    return newFunc;
}

CONT
newSchemeContinuation() {
    CONT newCont;

    newCont = (CONT)schemeAlloc( sizeof(struct schemeContinuation));
    return newCont;
}
