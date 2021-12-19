#ifndef CONTEXT_H
#define CONTEXT_H

#include "utils/ht.h"

typedef struct Context {
  HashTable *hash;
} Context;

// Creates a new context. Muste be destroyed later
Context *makeContext();

// Destroys a context
void destroyContext(Context *c);

// Wipes all variables inside a context
void contextReset(Context *c);

// Tries getting a variable. Returns NULL if the variable does not exist.
char *contextGetVariable(Context *c, char *name);

// Tries getting a variable first from a local context. If it fails, try again
// from a global context
char *contextGetVariableLocalOrGlobal(Context *c, Context *gc, char *name);

// Tries to assign a value to a variable. Returns 1 on success,
// 0 on fail.
int contextSetVariable(Context *c, char *name, char *value);

#endif