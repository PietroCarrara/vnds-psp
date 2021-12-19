#include "string.h"

#include <stdlib.h>
#include <string.h>

char *evalString(char *str, Context *c, Context *gc) {
  // TODO: Implement
  return strdup(str);
}

char *strdup(const char *str) {
  char *res = malloc(strlen(str) + 1);
  strcpy(res, str);
  return res;
}

int isNumber(const char *str) {
  int i;
  int len = strlen(str);
  for (i = 0; i < len; i++) {
    char c = str[i];
    if (c < '0' && c > '9') {
      // The first character may be a sign
      if (!(c == '-' && i == 0)) {
        return 0;
      }
    }
  }
  return 1;
}