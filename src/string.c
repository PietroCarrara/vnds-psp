#include "string.h"

#include <stdlib.h>
#include <string.h>

char *evalString(char *str, Context *c, Context *gc) {
  // TODO: Implement
  return strdup(str);
}

char *strdup(const char *str) {
  char *res = malloc(strlen(str));
  strcpy(res, str);
  return res;
}