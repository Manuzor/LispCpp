#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "string.h"

#if defined(__x86_64__)
# define POINTER_SIZE 8
#endif
#if defined(__x86__)
# define POINTER_SIZE 4
#endif
#ifndef POINTER_SIZE
# error please check your cpu here
#endif

#define INITIAL_SYMBOL_TABLE_SIZE   2

enum schemeTagEnum {
    TAG_UNUSED = 0,
    TAG_SMALLINTEGER = 1,
    TAG_CONS,
    TAG_SYMBOL,
    TAG_STRING,
    TAG_VECTOR,
    TAG_FILEINPUTSTREAM,
    TAG_FILEOUTPUTSTREAM,
    TAG_STRINGINPUTSTREAM,
    TAG_STRINGOUTPUTSTREAM,
    TAG_INTEGER,
    TAG_DOUBLE,
    TAG_USERDEFINEDFUNCTION,
    TAG_USERDEFINEDSYNTAX,
    TAG_BUILTINFUNCTION,
    TAG_BUILTINSYNTAX,
    TAG_NIL,
    TAG_TRUE,
    TAG_FALSE,
    TAG_EOF,
    TAG_VOID,
    TAG_ENVIRONMENT,
};

typedef enum schemeTagEnum schemeTag;

typedef union schemeObject *OBJ;
typedef struct schemeContinuation *CONT;

typedef OBJ (*OBJFUNC)(OBJ);
typedef OBJ (*OBJFUNC2)(OBJ, OBJ);
typedef CONT (*CONTFUNC)();

#ifdef WIN32
typedef long long INT;
# define _ld_    "lld"
#else
typedef long INT;
# define _ld_    "ld"
#endif

#define isSmallInteger(o)       ((INT)(o) & TAG_SMALLINTEGER)
#define isPointer(o)            (!isSmallInteger(o))
#define makeSmallInteger(i)     (OBJ)(((INT)(i) << 1)|TAG_SMALLINTEGER)
#define smallIntegerValue(o)    ((INT)(o)>>1)
#if POINTER_SIZE==4
# define minSmallInteger        -0x40000000
# define maxSmallInteger        0x3FFFFFFF
#else
# define minSmallInteger        -0x4000000000000000
# define maxSmallInteger        0x3FFFFFFFFFFFFFFF
#endif

typedef void (*VOIDFUNC)();

struct schemeHeaderStrunct {
    schemeTag tag;
};
typedef struct schemeHeaderStrunct schemeHeader;

struct schemeCons {
    schemeHeader header;
    OBJ car;
    OBJ cdr;
};

// wegen alignment sizeof
#define NUM_DUMMY_ALLOC 1

struct schemeSymbol {
    schemeHeader header;
    int size;
    char characters[NUM_DUMMY_ALLOC];
};

struct schemeString {
    schemeHeader header;
    int size;
    char characters[NUM_DUMMY_ALLOC];
};

struct schemeVector {
    schemeHeader header;
    int numElements;
    OBJ elements[NUM_DUMMY_ALLOC];
};

struct schemeInteger {
    schemeHeader header;
    INT intVal;
};

struct schemeDouble {
    schemeHeader header;
    double doubleVal;
};

struct schemeFileInputStream {
    schemeHeader header;
    FILE *inputFile;
    INT  lookAheadChar;
};

struct schemeStringInputStream {
    schemeHeader header;
    OBJ  inputString;
    INT  readPosition;
};
#define NO_PEEK_CHAR    -1

struct schemeFileOutputStream {
    schemeHeader header;
    FILE *outputFile;
};

struct schemeUserDefinedFunction {
    schemeHeader header;
    OBJ argList;
    OBJ bodyList;
    OBJ environment;
};

struct schemeBuiltinFunction {
    schemeHeader header;
    char *name;
    CONTFUNC code;
    int numArgs;
};

struct schemeBuiltinSyntax {
    schemeHeader header;
    char *name;
    CONTFUNC code;
    int numArgs;
};

struct schemeEnvironment {
    schemeHeader header;
    int tally;
    OBJ keysAndValues;
    OBJ parentEnvironment;
};

struct schemeContinuation {
    schemeHeader header;
    CONT caller;
    CONTFUNC pc;
    OBJ retVal;
    OBJ args[10];
};

union schemeObject {
    schemeHeader header;
    struct schemeCons schemeCons;
    struct schemeSymbol schemeSymbol;
    struct schemeString schemeString;
    struct schemeInteger schemeInteger;
    struct schemeDouble schemeDouble;
    struct schemeFileInputStream schemeFileInputStream;
    struct schemeFileOutputStream schemeFileOutputStream;
    struct schemeUserDefinedFunction schemeUserDefinedFunction;
    struct schemeBuiltinFunction schemeBuiltinFunction;
    struct schemeBuiltinSyntax schemeBuiltinSyntax;
    struct schemeVector schemeVector;
    struct schemeEnvironment schemeEnvironment;
    CONT schemeContinuation;
};

#define tag(o)                   (isSmallInteger(o) ? TAG_SMALLINTEGER : ((o)->header.tag))

#define isSchemeCons(o)          (tag(o) == TAG_CONS)
#define isSchemeNil(o)           ((o) == SCHEME_NIL)
#define isSchemeTrue(o)          ((o) == SCHEME_TRUE)
#define isSchemeFalse(o)         ((o) == SCHEME_FALSE)
#define isSchemeVoid(o)          ((o) == SCHEME_VOID)
#define isSchemeSymbol(o)        (tag(o) == TAG_SYMBOL)
#define isSchemeString(o)        (tag(o) == TAG_STRING)
#define isSchemeInteger(o)       (tag(o) == TAG_INTEGER)
#define isSchemeBuiltinSyntax(o) (tag(o) == TAG_BUILTINSYNTAX)
#define isSchemeInputStream(o)   (tag(o) == TAG_INPUTSTREAM)
#define isSchemeOutputStream(o)  (tag(o) == TAG_OUTPUTSTREAM)

#define INLINE static __inline__

#include "schemeFunctions.h"

#define schemeError(msg)        schemeErrorWithFileAndLineno(msg, __FILE__, __LINE__)
#define schemeError_o(msg,o)    schemeErrorWithFileAndLineno_o(msg, o, __FILE__, __LINE__)

extern OBJ SCHEME_NIL;
extern OBJ SCHEME_TRUE;
extern OBJ SCHEME_FALSE;
extern OBJ SCHEME_EOF;
extern OBJ SCHEME_VOID;
extern jmp_buf errorReturn;

INLINE OBJ
checked_car(OBJ o) {
    return o->schemeCons.car;
}

INLINE OBJ
checked_cdr(OBJ o) {
    return o->schemeCons.cdr;
}

INLINE OBJ
car(OBJ o) {
    if (!isSchemeCons(o)) schemeError_o("non-cons given to car: ", o);
    return checked_car(o);
}

INLINE OBJ
cdr(OBJ o) {
    if (!isSchemeCons(o)) schemeError_o("non-cons given to cdr: ", o);
    return checked_cdr(o);
}

//#define DEBUG_RECURSION

#ifdef DEBUG_RECURSION
# define TRAMP(cont) return trampoline(cont)
#else
# define TRAMP(cont) return cont
#endif

#ifdef USE_MACROS

#define C_RETURN(returnValue) { \
    cont->caller->retVal = returnValue; \
    return cont->caller; \
}

#define C_TAILCALL1(func, arg) { \
    cont->args[0] = arg1; \
    cont->pc = func; \
    TRAMP (cont); \
}

#define C_TAILCALL2(func, arg1, arg2) { \
    cont->args[0] = arg1; \
    cont->args[1] = arg2; \
    cont->pc = func; \
    TRAMP (cont); \
}

#define C_TAILCALL3(func, arg1, arg2, arg3) { \
    cont->args[0] = arg1; \
    cont->args[1] = arg2; \
    cont->args[2] = arg3; \
    cont->pc = func; \
    TRAMP (cont); \
}

#define C_JUMP(func) { \
    cont->pc = func; \
    return cont; \
}

#define C_CALL2(func, arg1, arg2, contAddress) { \
    CONT newCont = newSchemeContinuation(); \
    cont->pc = contAddress; \
    newCont->caller = cont; \
    newCont->args[0] = arg1; \
    newCont->args[1] = arg2; \
    newCont->pc = func; \
    TRAMP (newCont); \
}

#else

#define C_RETURN(returnValue) \
    return _C_RETURN(cont, returnValue);

INLINE CONT _C_RETURN(CONT cont, OBJ returnValue) {
    cont->caller->retVal = returnValue;
    return cont->caller;
}

#define C_JUMP(func) \
    return _C_JUMP(cont, func);

INLINE CONT _C_JUMP(CONT cont, CONTFUNC func) {
    cont->pc = func;
    return cont;
}

#define C_TAILCALL1(func, arg1) \
    return _C_TAILCALL1(cont, func, arg1);

INLINE CONT _C_TAILCALL1(CONT cont, CONTFUNC func, OBJ arg1) {
    cont->args[0] = arg1;
    cont->pc = func;
    TRAMP (cont);
}

#define C_TAILCALL2(func, arg1, arg2) \
    return _C_TAILCALL2(cont, func, arg1, arg2);

INLINE CONT _C_TAILCALL2(CONT cont, CONTFUNC func, OBJ arg1, OBJ arg2) {
    cont->args[0] = arg1;
    cont->args[1] = arg2;
    cont->pc = func;
    TRAMP (cont);
}

#define C_TAILCALL3(func, arg1, arg2, arg3) \
    return _C_TAILCALL3(cont, func, arg1, arg2, arg3);

INLINE CONT _C_TAILCALL3(CONT cont, CONTFUNC func, OBJ arg1, OBJ arg2, OBJ arg3) {
    cont->args[0] = arg1;
    cont->args[1] = arg2;
    cont->args[2] = arg3;
    cont->pc = func;
    TRAMP(cont);
}

#define C_CALL2(func, arg1, arg2, contAddress) \
    return _C_CALL2(cont, func, arg1, arg2, contAddress);

INLINE CONT _C_CALL2(CONT cont, CONTFUNC func, OBJ arg1, OBJ arg2, CONTFUNC contAddress) {
    CONT newCont = newSchemeContinuation();
    cont->pc = contAddress;
    newCont->caller = cont;
    newCont->args[0] = arg1;
    newCont->args[1] = arg2;
    newCont->pc = func;
    TRAMP (newCont);
}

#endif
