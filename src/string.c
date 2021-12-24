#include "string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getVarNameLength(char *str, int len) {
  int i;
  for (i = 0; i < len; i++) {
    if (str[i] == ' ' || str[i] == '}') {
      return i;
    }
  }

  return i - 1;
}

char *evalString(char *str, Context *c, Context *gc) {
  // TODO: Implement
  int allocated = 16;
  int resLen = 0;
  int originalLen = strlen(str);
  char *res = malloc(allocated);
  int i;

  for (i = 0; i < originalLen; i++) {
    if (str[i] == '$' ||
        (i + 1 < originalLen && str[i] == '{' && str[i + 1] == '$')) {
      // Either $var... or {$var...}
      // Replace the following variable name with it's value
      if (str[i] == '{') {
        i++;
      }

      // Skip '$' and get the variable name
      int varNameLen = getVarNameLength(str + i + 1, originalLen - i);
      char tmp = str[i+1+varNameLen];
      str[i+1+varNameLen] = '\0';
      char *varName = strdup(str + i + 1);
      str[i+1+varNameLen] = tmp;

      char *val = contextGetVariableLocalOrGlobal(c, gc, varName);
      if (val != NULL) {
        // Write the variable value to the result
        int valLen = strlen(val);
        int j;
        for (j = 0; j < valLen; j++) {
          res[resLen] = val[j];
          resLen++;
          if (resLen >= allocated) {
            allocated *= 2;
            res = realloc(res, allocated);
          }
        }
      }

      if (i > 0 && str[i-1] == '{') {
        i++; // Skip closing '}'
      }
      i += varNameLen;  // Skip the variable name
      free(varName);
    } else {
      // Regular character, copy it to the result
      res[resLen] = str[i];
      resLen++;
    }

    if (resLen >= allocated) {
      allocated *= 2;
      res = realloc(res, allocated);
    }
  }

  // Shrink/grow to fit exactly what is needed
  res = realloc(res, resLen + 1);
  res[resLen] = '\0';
  return res;
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