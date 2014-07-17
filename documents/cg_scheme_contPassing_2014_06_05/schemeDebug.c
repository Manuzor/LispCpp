#include "scheme.h"

void
dumpObject(OBJ o) {
    switch (tag(o)) {
	case TAG_NIL:
	    printf("nil\n");
	    break;

	case TAG_TRUE:
	    printf("true\n");
	    break;

	case TAG_FALSE:
	    printf("false\n");
	    break;

	case TAG_SYMBOL:
	    printf("symbol: %s\n", o->schemeSymbol.characters);
	    break;

	case TAG_STRING:
	    printf("string: %s\n", o->schemeString.characters);
	    break;

	default:
	    printf("oops\n");
    }
}

