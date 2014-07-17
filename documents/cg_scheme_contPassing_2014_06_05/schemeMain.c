#include "scheme.h"

jmp_buf errorReturn;

OBJ SCHEME_NIL = 0;
OBJ SCHEME_TRUE = 0;
OBJ SCHEME_FALSE = 0;
OBJ SCHEME_EOF = 0;
OBJ SCHEME_VOID = 0;
OBJ SCHEME_TOPENVIRONMENT = 0;

static void
initializeGlobals() {
    SCHEME_NIL = newSchemeSingleton(TAG_NIL);
    SCHEME_TRUE = newSchemeSingleton(TAG_TRUE);
    SCHEME_FALSE = newSchemeSingleton(TAG_FALSE);
    SCHEME_EOF = newSchemeSingleton(TAG_EOF);
    SCHEME_VOID = newSchemeSingleton(TAG_VOID);
}

static void
initializeTopEnvironment() {
    OBJ env = SCHEME_TOPENVIRONMENT = newSchemeEnvironment(2);

    addBinding(env, newSchemeSymbol("a"),
		    newSchemeInteger(100));
    addBinding(env, newSchemeSymbol("b"),
		    newSchemeInteger(200));
    addBinding(env, newSchemeSymbol("c"),
		    newSchemeString("hello"));

    addBinding(env, newSchemeSymbol("#f"),
		    SCHEME_FALSE);
    addBinding(env, newSchemeSymbol("#t"),
		    SCHEME_TRUE);
    addBinding(env, newSchemeSymbol("nil"),
		    SCHEME_NIL);
    addBinding(env, newSchemeSymbol("*top*"), env);

    initializeBuiltins(env);
}

CONT repl_2();

CONT
repl(CONT cont) {
    #define input       cont->args[0]
    #define doPrompt    cont->args[1]
    OBJ o, v;
    FILE *inputFile = input->schemeFileInputStream.inputFile;

    if (feof(inputFile)) {
	C_RETURN(SCHEME_VOID);
    }

    if (doPrompt == SCHEME_TRUE) printf("scheme> ");
    o = schemeRead(input);

    C_CALL2(schemeEval, o, SCHEME_TOPENVIRONMENT, repl_2);
    // never reached

    #undef input
    #undef doPrompt
}

CONT
repl_2(CONT cont) {
    #define input       cont->args[0]
    #define doPrompt    cont->args[1]
    OBJ v = cont->retVal;
    FILE *inputFile = input->schemeFileInputStream.inputFile;

    if (v == SCHEME_EOF) {
	C_RETURN(SCHEME_VOID);
    }
    if (v != SCHEME_VOID) {
	schemePrint(v, stdout);
	printf("\n");
    }

    C_TAILCALL2(repl, input, doPrompt);
    #undef input
    #undef doPrompt
}

static
validateConfiguration() {
    if (sizeof(INT) != sizeof(char *)) {
	printf("oops");
	exit(1);
    }
//    printf("sizeof int is %d\n", (int)sizeof(int));
//    printf("sizeof INT is %d\n", (int)sizeof(INT));
//    printf("sizeof OBJ is %d\n", (int)sizeof(OBJ));
}

void
trampoline(CONT cont) {
    CONT nextCont = cont;
    CONTFUNC func;

    for (;;) {
	func = nextCont->pc;
	nextCont = (*func)(nextCont);
    }
}

CONT
finish(CONT cont) {
    printf("done:\n");
    schemePrint(cont->retVal, stdout);
    exit(0);
}

int
main(char **argv, int argc) {
    CONT cont;
    CONT callerCont;
    OBJ expr;
    OBJ input;

    validateConfiguration();
    initializeSymbolTable();
    initializeGlobals();
    initializeTopEnvironment();

    printf("Welcome to Scheme\n");

    if (setjmp(errorReturn)) {
	// hier in case of error-return
	printf("Back in REPL\n");
    }

    input = newSchemeFileInputStream();
    input->schemeFileInputStream.inputFile = stdin;

    callerCont = newSchemeContinuation();
    callerCont->pc = finish;

    cont = newSchemeContinuation();
    cont->caller = callerCont;
    cont->args[0] = input;
    cont->args[1] = SCHEME_TRUE;
    cont->pc = repl;
    trampoline(cont);

//    schemeBuiltin_load(
//                    newSchemeCons(
//                        newSchemeString("init.scm"),
//                        SCHEME_NIL));

}
