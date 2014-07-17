OBJ schemeRead(OBJ);
void schemePrint(OBJ, FILE *);

OBJ newSchemeFileInputStream();
OBJ newSchemeVector(int);
OBJ newSchemeString(char *);
OBJ newSchemeSymbol(char *);
OBJ newSchemeInteger(INT);
OBJ newSchemeCons(OBJ, OBJ);
OBJ newSchemeSingleton(schemeTag);
OBJ newSchemeEnvironment(int);
OBJ newSchemeBuiltinFunction(CONTFUNC, char *, int);
OBJ newSchemeBuiltinSyntax(CONTFUNC, char *, int);
OBJ newSchemeUserDefinedFunction(OBJ, OBJ, OBJ);
CONT newSchemeContinuation();

void D(OBJ);
void dumpEnvironment(OBJ);

void addBinding(OBJ, OBJ, OBJ);
OBJ getBinding(OBJ, OBJ);
void setBinding(OBJ, OBJ, OBJ);

CONT schemeEval(CONT);

void schemeErrorWithFileAndLineno(char *, char *, int);
void schemeErrorWithFileAndLineno_o(char *, OBJ, char *, int);
