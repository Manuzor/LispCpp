#include "scheme.h"

CONT
schemeBuiltin_plus(CONT cont) {
    #define argList cont->args[0]
    INT sum = 0;

    while (argList != SCHEME_NIL) {
	OBJ a = car(argList);

	switch (tag(a)) {
	    case TAG_SMALLINTEGER:
		sum += smallIntegerValue(a);
		break;

	    case TAG_INTEGER:
		sum += a->schemeInteger.intVal;
		break;

	    default:
		schemeError("(+): argument is not an integer");

	}
	argList = cdr(argList);
    }
    C_RETURN ( newSchemeInteger(sum) );
}

CONT
schemeBuiltin_times(CONT cont) {
    #define argList cont->args[0]
    INT product = 1;

    while (argList != SCHEME_NIL) {
	OBJ a = car(argList);

	switch (tag(a)) {
	    case TAG_SMALLINTEGER:
		product *= smallIntegerValue(a);
		break;

	    case TAG_INTEGER:
		product *= a->schemeInteger.intVal;
		break;

	    default:
		schemeError("(*): argument is not an integer");

	}
	argList = cdr(argList);
    }
    C_RETURN ( newSchemeInteger(product) );
}

CONT
schemeBuiltin_minus(CONT cont) {
    #define argList cont->args[0]
    INT diff = 0;
    OBJ arg1;

    if (argList == SCHEME_NIL) {
	schemeError("(-): at least 1 argument expected");
    }
    arg1 = car(argList);
    switch (tag(arg1)) {
	case TAG_SMALLINTEGER:
	    diff = smallIntegerValue(arg1);
	    break;
	case TAG_INTEGER:
	    diff = arg1->schemeInteger.intVal;
	    break;
	default:
	    schemeError("(-): argument is not an integer");
    }
    argList = cdr(argList);
    if (argList == SCHEME_NIL) {
	C_RETURN (newSchemeInteger(-diff));
    }

    while (argList != SCHEME_NIL) {
	OBJ argI = car(argList);

	switch (tag(argI)) {
	    case TAG_SMALLINTEGER:
		diff -= smallIntegerValue(argI);
		break;
	    case TAG_INTEGER:
		diff -= argI->schemeInteger.intVal;
		break;
	    default:
		schemeError("(+): argument is not an integer");
	}
	argList = cdr(argList);
    }
    C_RETURN ( newSchemeInteger(diff));
}

CONT
schemeBuiltin_cons(CONT cont) {
    #define argList cont->args[0]
    OBJ arg1 = car(argList);
    OBJ arg2 = car(cdr(argList));

    C_RETURN ( newSchemeCons(arg1, arg2));
}

CONT
schemeBuiltin_car(CONT cont) {
    #define argList cont->args[0]
    OBJ arg1 = car(argList);

    C_RETURN (car(arg1));
}

CONT
schemeBuiltin_cdr(CONT cont) {
    #define argList cont->args[0]
    OBJ arg1 = car(argList);

    C_RETURN (cdr(arg1));
}

CONT
schemeBuiltin_print(CONT cont) {
    #define argList cont->args[0]
    OBJ arg1 = car(argList);

    schemePrint(arg1, stdout);
    C_RETURN ( SCHEME_VOID );
}

CONT
schemeBuiltin_eqP(CONT cont) {
    #define argList cont->args[0]
    OBJ arg1 = car(argList);
    OBJ arg2 = car(cdr(argList));

    if (arg1==arg2) {
	C_RETURN ( SCHEME_TRUE );
    }
    C_RETURN ( SCHEME_FALSE );
}

CONT
schemeBuiltin_display(CONT cont) {
    #define argList cont->args[0]
    OBJ arg1 = car(argList);

    schemeDisplay(arg1, stdout);
    C_RETURN ( SCHEME_VOID );
}

CONT schemeBuiltin_load2(CONT);
CONT repl(CONT);

CONT
schemeBuiltin_load(CONT cont) {
    #define argList cont->args[0]
    #define input cont->args[1]
    OBJ fileName = car(argList);
    FILE *theFile;

    if (!isSchemeString(fileName)) {
	schemeError("(load): string argument expected");
    }
    theFile = fopen(fileName->schemeString.characters, "r");

    input = newSchemeFileInputStream();
    input->schemeFileInputStream.inputFile = theFile;

    C_CALL2(repl, input, SCHEME_FALSE, schemeBuiltin_load2);
}

CONT
schemeBuiltin_load2(CONT cont) {
    FILE *theFile = input->schemeFileInputStream.inputFile;

    fclose(theFile);
    C_RETURN (SCHEME_VOID);

    #undef argList
    #undef input
}

CONT schemeBuiltin_if2(CONT);

CONT
schemeBuiltin_if(CONT cont) {
    #define argList cont->args[0]
    #define env     cont->args[2]

    #define unevaluatedCond     car(argList)
    #define unevaluatedIfPart   car(cdr(argList))
    #define unevaluatedElsePart car(cdr(cdr(argList)))

    C_CALL2(schemeEval, unevaluatedCond, env, schemeBuiltin_if2);
}

CONT
schemeBuiltin_if2(CONT cont) {
    OBJ evaluatedCond = cont->retVal;
    OBJ exprToEvaluate;

    if (evaluatedCond == SCHEME_TRUE) {
	exprToEvaluate = unevaluatedIfPart;
    } else {
	exprToEvaluate = unevaluatedElsePart;
    }
    C_TAILCALL2(schemeEval, exprToEvaluate, env);

    #undef argList
    #undef env

    #undef unevaluatedCond
    #undef unevaluatedIfPart
    #undef unevaluatedElsePart
}

CONT schemeBuiltin_define2(CONT);

CONT
schemeBuiltin_define(CONT cont) {
    #define argList cont->args[0]
    #define env     cont->args[2]

    #define arg1    car(argList)
    #define rest    cdr(argList)

    if (argList == SCHEME_NIL) {
	schemeError("(define): missing argument");
    }

    if (isSchemeSymbol(arg1)) {
	// (define var expr)
	OBJ unevaluatedExpression;

	if (rest == SCHEME_NIL) {
	    schemeError("(define): missing expression argument");
	}
	C_CALL2(schemeEval, car(rest), env, schemeBuiltin_define2);
	// not reached
    }

    if (!isSchemeCons(arg1)) {
	schemeError("(define): invalid argument");
    }

    // (define (f argList) bodyList)
    {
	OBJ functionNamePlusLambdaArgList = arg1;
	OBJ bodyList = rest;
	OBJ lambdaArgList;
	OBJ func;
	OBJ funcName;

	funcName = car(functionNamePlusLambdaArgList);
	lambdaArgList = cdr(functionNamePlusLambdaArgList);
	func = newSchemeUserDefinedFunction(lambdaArgList, bodyList, env);
	addBinding(env, funcName, func);
	// dumpEnvironment(env);
	C_RETURN( SCHEME_VOID );
    }
}

CONT
schemeBuiltin_define2(CONT cont) {
    OBJ evaluatedExpr = cont->retVal;

    addBinding(env, arg1, evaluatedExpr);
    C_RETURN( SCHEME_VOID );

    #undef argList
    #undef env

    #undef arg1
    #undef rest
}

CONT
schemeBuiltin_lambda(CONT cont) {
    #define argList cont->args[0]
    #define env     cont->args[2]

    #define arg1     car(argList)
    #define rest     cdr(argList)

    if (argList == SCHEME_NIL) {
	schemeError("(lambda): missing argument");
    }
    C_RETURN ( newSchemeUserDefinedFunction(arg1, rest, env) );

    #undef argList
    #undef env

    #undef arg1
    #undef rest
}

CONT schemeBuiltin_set2(CONT);

CONT
schemeBuiltin_set(CONT cont) {
    #define argList cont->args[0]
    #define env     cont->args[2]

    #define arg1                car(argList)
    #define unevaluatedArg2     car(cdr(argList))

    if (!isSchemeSymbol(arg1)) {
	schemeError("(set!): not a symbol key");
    }
    C_CALL2(schemeEval, unevaluatedArg2, env, schemeBuiltin_set2);
}

CONT
schemeBuiltin_set2(CONT cont) {
    OBJ evaluatedArg2 = cont->retVal;

    setBinding(env, arg1, evaluatedArg2);
    C_RETURN (SCHEME_VOID);

    #undef argList
    #undef env

    #undef arg1
    #undef unevaluatedArg2
}

static
defineFunction(OBJ env, CONTFUNC code, char *name, int nArgs) {
    OBJ f;

    f = newSchemeBuiltinFunction( code, name, nArgs );
    addBinding(env, newSchemeSymbol(name), f);
}

static
defineSyntax(OBJ env, CONTFUNC code, char *name, int nArgs) {
    OBJ f;

    f = newSchemeBuiltinSyntax( code, name, nArgs );
    addBinding(env, newSchemeSymbol(name), f);
}

void
initializeBuiltins(OBJ env) {
    OBJ f;

    // syntax
    defineSyntax(env, schemeBuiltin_if, "if" , 3);
    defineSyntax(env, schemeBuiltin_define, "define" , -1);
    defineSyntax(env, schemeBuiltin_lambda, "lambda" , -1);
    defineSyntax(env, schemeBuiltin_set, "set!" , 2);

    // functions
    defineFunction(env, schemeBuiltin_plus, "+" , -1);
    defineFunction(env, schemeBuiltin_times, "*" , -1);
    defineFunction(env, schemeBuiltin_minus, "-" , -1);

    defineFunction(env, schemeBuiltin_cons, "cons" , 2);
    defineFunction(env, schemeBuiltin_car, "car" , 1);
    defineFunction(env, schemeBuiltin_cdr, "cdr" , 1);

    defineFunction(env, schemeBuiltin_eqP, "eq?" , 2);
    defineFunction(env, schemeBuiltin_display, "display" , 1);

    defineFunction(env, schemeBuiltin_load, "load" , 1);
    defineFunction(env, schemeBuiltin_print, "print" , 1);
}
