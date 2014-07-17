#include "scheme.h"

#define EOS     -1

static int
peekCharacter(OBJ in) {
    char ch;

    switch (tag(in)) {
	case TAG_FILEINPUTSTREAM:
	    if (in->schemeFileInputStream.lookAheadChar != NO_PEEK_CHAR) {
		return (in->schemeFileInputStream.lookAheadChar);
	    }
	    if (fread(&ch, 1, 1, in->schemeFileInputStream.inputFile) != 1) {
		return EOS;
	    }
	    in->schemeFileInputStream.lookAheadChar = ch;
	    return (int)ch;

	case TAG_STRINGINPUTSTREAM:

	default:
	    schemeError("non stream in peek");
    }
}

static int
readCharacter(OBJ in) {
    char ch;

    switch (tag(in)) {
	case TAG_FILEINPUTSTREAM:
	    if (in->schemeFileInputStream.lookAheadChar != NO_PEEK_CHAR) {
		ch = in->schemeFileInputStream.lookAheadChar;
		in->schemeFileInputStream.lookAheadChar = NO_PEEK_CHAR;
		return ch;
	    } else {
		if (fread(&ch, 1, 1, in->schemeFileInputStream.inputFile) != 1) {
		    return EOS;
		}
		return (int)ch;
	    }

	default:
	    schemeError("non stream in peek");
    }
}

static int
isSeparator(int ch) {
    return
	(ch == ' ')
	|| (ch == '\t')
	|| (ch == '\n')
	|| (ch == '\r');
}

void
skipEOLComment(OBJ in) {
    int ch;

    while (ch != '\n') {
	readCharacter(in);
	ch = peekCharacter(in);
    }
    readCharacter(in);
}

void
skipSeparators(OBJ in) {
    int ch;

    ch = peekCharacter(in);
    for (;;) {
	while (ch == ';') {
	    skipEOLComment(in);
	    ch = peekCharacter(in);
	}
	while (isSeparator(ch)) {
	    readCharacter(in);
	    ch = peekCharacter(in);
	}
	if (ch != ';') return;
    }
}

OBJ
schemeReadListRest(OBJ in) {
    int ch;
    OBJ first;
    OBJ rest;

    /* '(' already read */

    skipSeparators(in);
    ch = peekCharacter(in);
    if (ch == ')') {
	readCharacter(in);
	return SCHEME_NIL;
    }
    first = schemeRead(in);
    skipSeparators(in);
    if (peekCharacter(in) == '.') {
	readCharacter(in);
	rest = schemeRead(in);
	skipSeparators(in);
	if (peekCharacter(in) != ')') {
	    schemeError("nothing more expected after '.'");
	}
	readCharacter(in); // read the ')'
    } else {
	rest = schemeReadListRest(in);
    }
    return newSchemeCons(first, rest);
}


#define INITIAL_BUFFER_SIZE 64

OBJ
schemeReadString(OBJ in) {
    char quickBuffer[INITIAL_BUFFER_SIZE];
    char *buffer = quickBuffer;
    int currentBufferSize = INITIAL_BUFFER_SIZE;
    int numCharacters = 0;
    int ch;
    OBJ newString;

    readCharacter(in);  // skip initial "
    for (;;) {
	ch = peekCharacter(in);
	if (ch == '"') {
	    readCharacter(in);
	    buffer[numCharacters++] = '\0';
	    newString = newSchemeString(buffer);
	    if (buffer != quickBuffer) free(buffer);
	    return newString;
	}
	if (ch == '\\') {
	    readCharacter(in);
	    ch = peekCharacter(in);
	    switch (ch) {
		case 'n':
		    ch = '\n';
		    break;
		case 't':
		    ch = '\t';
		    break;
		case 'r':
		    ch = '\r';
		    break;
	    }
	}
	buffer[numCharacters++] = ch;
	if (numCharacters >= currentBufferSize) {
	    if (buffer == quickBuffer) {
		buffer = malloc(currentBufferSize*2);
		memcpy(buffer, quickBuffer, currentBufferSize);
	    } else {
		buffer = realloc(buffer, currentBufferSize*2);
	    }
	    currentBufferSize = currentBufferSize * 2;
	}
	readCharacter(in);
    }
}

static int
isValidInteger(char *cp) {
    if (*cp == '-') {
	cp++;
	if (*cp == '\0') {
	    return 0;
	}
    }
    while (*cp != '\0') {
	if (! ((*cp >= '0') && (*cp <= '9'))) {
	    return 0;
	}
	cp++;
    }
    return 1;
}

OBJ
schemeReadAtom(OBJ in) {
    char quickBuffer[INITIAL_BUFFER_SIZE];
    char *buffer = quickBuffer;
    int currentBufferSize = INITIAL_BUFFER_SIZE;
    int numCharacters = 0;
    int ch;
    OBJ newSymbol;

    for (;;) {
	ch = peekCharacter(in);
	if ((ch == '(')
	 || (ch == ')')
	 || isSeparator(ch)
	 || ch == EOS) {
	    buffer[numCharacters++] = '\0';
	    if (isValidInteger(buffer)) {
		INT iVal;

		if (sscanf(buffer, "%"_ld_, &iVal) == 1) {
		    return newSchemeInteger(iVal);
		}
	    }
	    newSymbol = newSchemeSymbol(buffer);
	    if (buffer != quickBuffer) free(buffer);
	    return newSymbol;
	}
	buffer[numCharacters++] = ch;
	if (numCharacters >= currentBufferSize) {
	    if (buffer == quickBuffer) {
		buffer = malloc(currentBufferSize*2);
		memcpy(buffer, quickBuffer, currentBufferSize);
	    } else {
		buffer = realloc(buffer, currentBufferSize*2);
	    }
	    currentBufferSize = currentBufferSize * 2;
	}
	readCharacter(in);
    }
}


OBJ
schemeRead(OBJ in) {
    int ch;

    skipSeparators(in);
    ch = peekCharacter(in);
    if (ch == EOS) {
	return SCHEME_EOF;
    }
    if (ch == '(') {
	readCharacter(in);
	return schemeReadListRest(in);
    }
    if (ch == ')') {
	schemeError("')' unexpected");
    }
    if (ch == '"') {
	return schemeReadString(in);
    }
    return schemeReadAtom(in);
}
