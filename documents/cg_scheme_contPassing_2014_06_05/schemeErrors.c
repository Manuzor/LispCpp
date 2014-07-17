#include "scheme.h"

void
schemeErrorWithFileAndLineno(char *msg, char *sourceFile, int lineNr) {
    fprintf(stderr, "error reported by %s, %d: %s\n", sourceFile, lineNr, msg);
    fprintf(stderr, "going back to REPL...\n");
    longjmp(errorReturn, 1);
}

void
schemeErrorWithFileAndLineno_o(char *msg, OBJ o, char *sourceFile, int lineNr) {
    fprintf(stderr, "error reported by %s, %d: %s ", sourceFile, lineNr, msg);
    schemePrint(o, stderr);
    fprintf(stderr, "\n");
    fprintf(stderr, "going back to REPL...\n");
    longjmp(errorReturn, 1);
}
