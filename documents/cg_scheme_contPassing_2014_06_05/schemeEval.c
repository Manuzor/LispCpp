#include "scheme.h"

static CONT schemeEvalBuiltinFunction_2(CONT);
static CONT schemeEvalBuiltinFunction_3(CONT);

static CONT
schemeEvalBuiltinFunction(CONT cont) {
    #define expr cont->args[0]
    #define func cont->args[1]
    #define env cont->args[2]
    #define argList cont->args[3]
    #define listOfValues cont->args[4]
    #define lastValue cont->args[5]
    #define argCount cont->args[6]
    listOfValues = SCHEME_NIL;
    lastValue = SCHEME_NIL;
    argCount = makeSmallInteger(0);

    // printf("expr: "); schemePrint(expr, stdout); printf("\n");
    // printf("func: "); schemePrint(func, stdout); printf("\n");

    argList = checked_cdr(expr);
    C_JUMP(schemeEvalBuiltinFunction_2);
}

static CONT
schemeEvalBuiltinFunction_2(CONT cont) {
    OBJ retValFromBuiltinFunction;

    if (argList == SCHEME_NIL) {
	if (func->schemeBuiltinFunction.numArgs != -1) {
	    if (func->schemeBuiltinFunction.numArgs != smallIntegerValue(argCount)) {
		schemeError_o("argument count mismatch in: ", func);
	    }
	}
	C_TAILCALL1( func->schemeBuiltinFunction.code , listOfValues);
    }

    // printf("argList: "); schemePrint(argList, stdout); printf("\n");
    {
	OBJ unevaluatedArg = car(argList);
	C_CALL2(schemeEval, unevaluatedArg, env, schemeEvalBuiltinFunction_3);
    }
}

static CONT
schemeEvalBuiltinFunction_3(CONT cont) {
    OBJ evaluatedArg = cont->retVal;
    OBJ thisCons = newSchemeCons(evaluatedArg, SCHEME_NIL);

    if (listOfValues == SCHEME_NIL) {
	listOfValues = thisCons;
    } else {
	lastValue->schemeCons.cdr = thisCons;
    }
    lastValue = thisCons;
    argList = cdr(argList);
    argCount = makeSmallInteger(smallIntegerValue(argCount)+1);
    C_JUMP(schemeEvalBuiltinFunction_2);

    #undef expr
    #undef func
    #undef env
    #undef argList
    #undef listOfValues
    #undef lastValue
    #undef argCount
}

static CONT
schemeEvalBuiltinSyntax(CONT cont) {
    #define expr cont->args[0]
    #define func cont->args[1]
    #define env cont->args[2]
    OBJ argList, t;
    int argCount = 0;

    t = argList = checked_cdr(expr);
    while (t != SCHEME_NIL) {
       t = cdr(t);
       argCount++;
    }
    if (func->schemeBuiltinSyntax.numArgs != -1) {
	if (func->schemeBuiltinSyntax.numArgs != argCount) {
	    schemeError_o("argument count mismatch in: ", func);
	}
    }
    C_TAILCALL2(func->schemeBuiltinSyntax.code, argList, env);

    #undef expr
    #undef func
    #undef env
}

static CONT schemeEvalUserDefinedFunction_2();
static CONT schemeEvalUserDefinedFunction_3();
static CONT schemeEvalUserDefinedFunction_4();
static CONT schemeEvalUserDefinedFunction_5();

static CONT
schemeEvalUserDefinedFunction(CONT cont) {
    #define func cont->args[0]
    #define unevaluatedArgList cont->args[1]
    #define env cont->args[2]
    #define listOfValues cont->args[3]
    #define lastValue cont->args[4]
    #define formalArgs cont->args[5]
    #define newEnv cont->args[6]
    #define remainingUnevaluatedArgs cont->args[7]
    #define remainingBodyExprs cont->args[8]

    listOfValues = SCHEME_NIL;
    lastValue = SCHEME_NIL;
    formalArgs = func->schemeUserDefinedFunction.argList;

    newEnv = newSchemeEnvironment(10);
    newEnv->schemeEnvironment.parentEnvironment = func->schemeUserDefinedFunction.environment;
//    printf("udf: ");
//    printf("func: "); D(func); printf("\n");
//    printf("formalArgs: "); D(formalArgs); printf("\n");
//    printf("funEnv: ");
//    dumpEnvironment(func->schemeUserDefinedFunction.environment);

    remainingUnevaluatedArgs = unevaluatedArgList;
    C_JUMP(schemeEvalUserDefinedFunction_2);
}

static CONT
schemeEvalUserDefinedFunction_2(CONT cont) {
    OBJ thisBodyExpr;

    if (remainingUnevaluatedArgs != SCHEME_NIL) {
	OBJ unevaluatedArg;

	unevaluatedArg = car(remainingUnevaluatedArgs);
	//        printf("uarg: "); D(unevaluatedArg); printf("\n");
	C_CALL2(schemeEval, unevaluatedArg, env, schemeEvalUserDefinedFunction_3);
	// not reached
    }
    //    dumpEnvironment(newEnv);

    remainingBodyExprs = func->schemeUserDefinedFunction.bodyList;
    if (remainingBodyExprs == SCHEME_NIL) {
	schemeError("empty body in function");
    }
    thisBodyExpr = car(remainingBodyExprs);
    if (cdr(remainingBodyExprs) == SCHEME_NIL) {
	C_TAILCALL2(schemeEval, thisBodyExpr, newEnv);
    } else {
	C_CALL2(schemeEval, thisBodyExpr, newEnv, schemeEvalUserDefinedFunction_5);
    }
}


static CONT
schemeEvalUserDefinedFunction_3(CONT cont) {
    OBJ thisFormalArg;
    OBJ evaluatedArg = cont->retVal;

    // evaluatedArg = schemeEval(unevaluatedArg, env);
    // printf("arg: "); D(evaluatedArg); printf("\n");

    thisFormalArg = car(formalArgs);
    addBinding(newEnv, thisFormalArg, evaluatedArg);
    formalArgs = cdr(formalArgs);
    remainingUnevaluatedArgs = cdr(remainingUnevaluatedArgs);
    C_JUMP(schemeEvalUserDefinedFunction_2);
}

static CONT
schemeEvalUserDefinedFunction_5(CONT cont) {
    lastValue = cont->retVal;
    remainingBodyExprs = cdr(remainingBodyExprs);
    if (remainingBodyExprs != SCHEME_NIL) {
	OBJ thisBodyExpr;

	thisBodyExpr = car(remainingBodyExprs);
	if (cdr(remainingBodyExprs) == SCHEME_NIL) {
	    // this is the very last body expr!
	    C_TAILCALL2(schemeEval, thisBodyExpr, newEnv);
	}
	C_CALL2(schemeEval, thisBodyExpr, newEnv, schemeEvalUserDefinedFunction_5);
    }
    C_RETURN( lastValue );
}

#undef func
#undef unevaluatedArgList
#undef env
#undef listOfValues
#undef lastValue
#undef formalArgs
#undef newEnv
#undef remainingUnevaluatedArgs
#undef remainingBodyExprs

static CONT
schemeEvalUserDefinedSyntax(CONT cont) {
    #define expr cont->args[0]
    #define env cont->args[1]

    schemeError("unimplemented user syntax eval");

    #undef expr
    #undef env
}

static CONT schemeEvalList_2(CONT);

static CONT
schemeEvalList(CONT cont) {
#define expr cont->args[0]
#define env cont->args[1]
    OBJ funcOrSyntaxUnEvaluated = checked_car(expr);

    //    funcOrSyntax = schemeEval(funcOrSyntaxUnEvaluated, env);
    C_CALL2(schemeEval, funcOrSyntaxUnEvaluated, env, schemeEvalList_2);
}

static CONT
schemeEvalList_2(CONT cont) {
    OBJ funcOrSyntax = cont->retVal;
//    funcOrSyntax = schemeEval(funcOrSyntaxUnEvaluated, env);

    switch (tag(funcOrSyntax)) {
	case TAG_BUILTINFUNCTION:
	    C_TAILCALL3(schemeEvalBuiltinFunction, expr, funcOrSyntax, env);

	case TAG_BUILTINSYNTAX:
	    C_TAILCALL3(schemeEvalBuiltinSyntax, expr, funcOrSyntax, env);

	case TAG_USERDEFINEDFUNCTION:
	    C_TAILCALL3(schemeEvalUserDefinedFunction, funcOrSyntax, cdr(expr), env);

	case TAG_USERDEFINEDSYNTAX:
	    C_TAILCALL2(schemeEvalUserDefinedSyntax, expr, env);

	default:
	    schemeError_o("non function in function slot: ", funcOrSyntax);
    }
#undef expr
#undef env
}

CONT
schemeEval(CONT cont) {
#define expr cont->args[0]
#define env cont->args[1]
    OBJ v;

    switch (tag(expr)) {
	case TAG_SYMBOL:
	    v = getBinding(env, expr);
	    C_RETURN( v );

	case TAG_CONS:
	    C_TAILCALL2(schemeEvalList, expr, env);

	default:
	    C_RETURN (expr);
    }
#undef expr
#undef env
}
