#include "scheme.h"

enum howToPrint {
    PRINT, DISPLAY, WRITE
};

#define PRINTF(a)   fprintf a; fflush(stdout);

// forward decl
static void __schemePrint(OBJ, enum howToPrint, FILE *);

static void
__printListRest(OBJ rest, enum howToPrint how, FILE *out) {
    for (;;) {
	__schemePrint(rest->schemeCons.car, how, out);
	if (isSchemeNil(rest->schemeCons.cdr)) {
	    break;
	}
	if (! isSchemeCons(rest->schemeCons.cdr)) {
	    PRINTF((out, " . "));
	    __schemePrint(rest->schemeCons.cdr, how, out);
	    break;
	}
	PRINTF((out, " "));
	rest = rest->schemeCons.cdr;
    }
}

static void
__schemePrint(OBJ o, enum howToPrint how, FILE *out) {
    switch (tag(o)) {
	case TAG_NIL:
	    PRINTF((out, "()"));
	    break;

	case TAG_TRUE:
	    PRINTF((out, "#t"));
	    break;

	case TAG_FALSE:
	    PRINTF((out, "#f"));
	    break;

	case TAG_EOF:
	    PRINTF((out, "#eof"));
	    break;

	case TAG_SYMBOL:
	    PRINTF((out, "%s", o->schemeSymbol.characters));
	    break;

	case TAG_STRING:
	    if (how == DISPLAY) {
		PRINTF((out, "%s", o->schemeString.characters));
	    } else {
		PRINTF((out, "\"%s\"", o->schemeString.characters));
	    }
	    break;

	case TAG_SMALLINTEGER:
	    PRINTF((out, "%"_ld_, smallIntegerValue(o)));
	    break;

	case TAG_INTEGER:
	    PRINTF((out, "%"_ld_, o->schemeInteger.intVal));
	    break;

	case TAG_CONS:
	    PRINTF((out, "("));
	    __printListRest(o, how, out);
	    PRINTF((out, ")"));
	    break;

	case TAG_BUILTINFUNCTION:
	    PRINTF((out, "<procedure:%s>", o->schemeBuiltinFunction.name));
	    break;

	case TAG_BUILTINSYNTAX:
	    PRINTF((out, "<syntax:%s>", o->schemeBuiltinFunction.name));
	    break;

	case TAG_USERDEFINEDFUNCTION:
	    PRINTF((out, "(lambda "));
	    __schemePrint(o->schemeUserDefinedFunction.argList, WRITE, out);
	    PRINTF((out, " "));
	    __printListRest(o->schemeUserDefinedFunction.bodyList, WRITE, out);
	    PRINTF((out, ")"));
	    break;

	case TAG_ENVIRONMENT:
	    PRINTF((out, "<environment:...>"));
	    dumpEnvironment(o);
	    break;

	case TAG_VOID:
	    PRINTF((out, "#void"));
	    break;


	default:
	    PRINTF((out, "oops - cannot print (%d)\n", tag(o)));
    }
}

void
D(OBJ o) {
    __schemePrint(o, PRINT, stdout);
}

void
schemePrint(OBJ o, FILE *out) {
    __schemePrint(o, PRINT, out);
}

void
schemeWrite(OBJ o, FILE *out) {
    __schemePrint(o, WRITE, out);
}

void
schemeDisplay(OBJ o, FILE *out) {
    __schemePrint(o, DISPLAY, out);
}
