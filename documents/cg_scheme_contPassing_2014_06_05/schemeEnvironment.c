#include "scheme.h"

void
addBinding(OBJ env, OBJ keyToAdd, OBJ value) {
    int tally = env->schemeEnvironment.tally;
    OBJ keysAndValues = env->schemeEnvironment.keysAndValues;
    OBJ newKeyValuePair;
    int i;

    for (i=0; i<tally; i++) {
	OBJ keyAndValuePair = keysAndValues->schemeVector.elements[i];
	OBJ key = keyAndValuePair->schemeCons.car;

	if (key == keyToAdd) {
	    // keyToAdd already in environment
	    keyAndValuePair->schemeCons.cdr = value;
	    return;
	}
    }
    // keyToAdd not in env
    if (tally >= keysAndValues->schemeVector.numElements) {
	// resize / grow
	OBJ newKeysAndValues = newSchemeVector(tally * 2);

	memcpy(newKeysAndValues->schemeVector.elements,
	       keysAndValues->schemeVector.elements,
	       sizeof(OBJ) * tally);
	env->schemeEnvironment.keysAndValues = newKeysAndValues;
	keysAndValues = newKeysAndValues;
    }
    newKeyValuePair = newSchemeCons(keyToAdd, value);
    keysAndValues->schemeVector.elements[i] = newKeyValuePair;
    env->schemeEnvironment.tally++;
}

OBJ
getBinding(OBJ env, OBJ keyToSearch) {
    OBJ theEnv = env;

    while (theEnv != SCHEME_NIL) {
	int tally = theEnv->schemeEnvironment.tally;
	OBJ keysAndValues = theEnv->schemeEnvironment.keysAndValues;
	int i;

	for (i=0; i<tally; i++) {
	    OBJ keyAndValuePair = keysAndValues->schemeVector.elements[i];
	    OBJ key = keyAndValuePair->schemeCons.car;
	    OBJ value = keyAndValuePair->schemeCons.cdr;

	    if (key == keyToSearch) {
		return keyAndValuePair->schemeCons.cdr;
	    }
	}
	theEnv = theEnv->schemeEnvironment.parentEnvironment;
    }
    schemeError_o("missing key: ", keyToSearch);
}

void
setBinding(OBJ env, OBJ keyToSearch, OBJ newValue) {
    OBJ theEnv = env;

    while (theEnv != SCHEME_NIL) {
	int tally = theEnv->schemeEnvironment.tally;
	OBJ keysAndValues = theEnv->schemeEnvironment.keysAndValues;
	int i;

	for (i=0; i<tally; i++) {
	    OBJ keyAndValuePair = keysAndValues->schemeVector.elements[i];
	    OBJ key = keyAndValuePair->schemeCons.car;
	    OBJ value = keyAndValuePair->schemeCons.cdr;

	    if (key == keyToSearch) {
		keyAndValuePair->schemeCons.cdr = newValue;
		return;
	    }
	}
	theEnv = theEnv->schemeEnvironment.parentEnvironment;
    }
    schemeError_o("missing key: ", keyToSearch);
}

void
dumpEnvironment(OBJ env) {
    OBJ theEnv = env;

    while (theEnv != SCHEME_NIL) {
	int tally = env->schemeEnvironment.tally;
	OBJ keysAndValues = env->schemeEnvironment.keysAndValues;
	int i;

	for (i=0; i<tally; i++) {
	    OBJ keyAndValuePair = keysAndValues->schemeVector.elements[i];
	    OBJ key = keyAndValuePair->schemeCons.car;
	    OBJ value = keyAndValuePair->schemeCons.cdr;

	    printf("key: "); D(key);
	    printf(" value: ");
	    if (tag(value) == TAG_ENVIRONMENT) {
		printf("env...");
	    } else {
		D(value);
	    }
	    printf("\n");
	}
	printf("\n");
	theEnv = theEnv->schemeEnvironment.parentEnvironment;
    }
}
