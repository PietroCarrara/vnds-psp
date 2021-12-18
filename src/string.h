#ifndef I_STRING_H
#define I_STRING_H

#include "context.h"

// Replaces any $<var> with the appropriate variable in the string. Result must
// be freed
char *evalString(char *str, Context *c, Context *gc);

// Allocates a copy of a string. Result must be freed
char *strdup(const char *str);

#endif