#include "context.h"

#include <stdlib.h>
#include <string.h>

#include "string.h"

Context *makeContext() {
  Context *c = malloc(sizeof(Context));
  c->hash = HashTableCreate();
  return c;
}

void destroyContext(Context *c) {
  HashTableIt it = HashTableIterator(c->hash);
  while (HashTableNext(&it)) {
    free(it.value);
  }
  HashTableDestroy(c->hash);
  free(c);
}

void contextReset(Context *c) {
  HashTableIt it = HashTableIterator(c->hash);
  while (HashTableNext(&it)) {
    free(it.value);
  }
  HashTableDestroy(c->hash);
  c->hash = HashTableCreate();
}

char *contextGetVariable(Context *c, char *name) {
  return (char *)HashTableGet(c->hash, name);
}

int contextSetVariable(Context *c, char *name, char *value) {
  char *old = HashTableGet(c->hash, name);
  if (old != NULL) {
    free(old);
  }

  return HashTableSet(c->hash, name, strdup(value)) != NULL;
}

char *contextGetVariableLocalOrGlobal(Context *c, Context *gc, char *name) {
  char* res = contextGetVariable(c, name);
  if (res == NULL) {
    res = contextGetVariable(gc, name);
  }

  return res;
}