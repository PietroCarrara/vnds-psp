#include "script.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"
#include "utils/ht.h"

char *executeGoto(Script *s, Context *c, Context *gc, GotoArgs *args) {
  char error[255] = {0};

  char *str = evalString(args->label, c, gc);
  int *index = scriptGetLabel(s, str);
  if (index == NULL) {
    snprintf(error, sizeof(error), "Label \"%s\" could not be found", str);
  } else {
    s->currentInstruction = *index;
  }

  free(str);
  return error[0] == '\0' ? NULL : strdup(error);
}

char *executeSetVariable(Script *s, Context *toSet, Context *c, Context *gc,
                         SetVarArgs *args) {
  char error[255] = {0};
  char *value = evalString(args->value, c, gc);

  if (args->operator== ValueSet) {
    if (!contextSetVariable(toSet, args->variable, value)) {
      snprintf(error, sizeof(error), "Could not set variable \"%s\" to \"%s\"",
               args->variable, value);
    }
  } else if (args->operator== ValueClear) {
    if (!contextSetVariable(toSet, args->variable, "0")) {
      snprintf(error, sizeof(error), "Could not set variable \"%s\" to \"0\"",
               args->variable);
    }
  } else {
    char str[13];
    int a = atoi(contextGetVariable(c, args->variable));
    int b = atoi(value);
    int res = a;

    if (args->operator== ValueSub) {
      res = a - b;
    } else {
      res = a + b;
    }
    snprintf(str, sizeof(str), "%d", res);
    if (!contextSetVariable(toSet, args->variable, str)) {
      snprintf(error, sizeof(error), "Could not set variable \"%s\" to \"%s\"",
               args->variable, str);
    }
  }

  free(value);
  return error[0] == '\0' ? NULL : strdup(error);
}

char *executeIf(Script *s, Context *c, Context *gc, IfArgs *args) {
  char error[255];

  char *left = contextGetVariableLocalOrGlobal(c, gc, args->variable);
  if (left == NULL) {
    snprintf(error, sizeof(error), "Could not find variable \"%s\"",
             args->variable);
    return strdup(error);
  }

  char *right = evalString(args->right, c, gc);

  int res = 0;
  if (args->operation == TestEquals || args->operation == TestNotEquals) {
    res = strcmp(left, right);
    if (args->operation == TestEquals) {
      res = !res;
    }
  } else {
    int leftInt = atoi(left);
    int rightInt = atoi(right);

    switch (args->operation) {
      case TestLess:
        res = leftInt < rightInt;
        break;
      case TestLessOrEquals:
        res = leftInt <= rightInt;
        break;
      case TestGreater:
        res = leftInt > rightInt;
        break;
      case TestGreaterOrEquals:
        res = leftInt >= rightInt;
        break;
      default:
        break;
    }
  }

  if (!res) {
    s->currentInstruction = args->falseGoto;
  }

  free(right);
  return NULL;
}

char *executeRandom(Script *s, Context *c, Context *gc, RandomArgs *args) {
  char *lowStr = evalString(args->low, c, gc);
  char *highStr = evalString(args->high, c, gc);

  int low = atoi(lowStr);
  int high = atoi(highStr);

  int r = (rand() % (high - low + 1)) + low;
  char rStr[13];
  snprintf(rStr, sizeof(rStr), "%d", r);
  if (!contextSetVariable(c, args->variable, rStr)) {
    return strdup("Could not set variable to random value");
  }

  return NULL;
}

int *scriptGetLabel(Script *s, const char *label) {
  return HashTableGet(s->labels, label);
}

char *scriptExecuteNext(Script *script, Context *c, Context *gc,
                        Instruction *instr) {
  char *error = NULL;
  Instruction i = script->instructions[script->currentInstruction];
  script->currentInstruction++;
  if (instr != NULL) {
    *instr = i;
  };

  switch (i.type) {
    case InstructionGoto:
      error = executeGoto(script, c, gc, i.args);
      break;
    case InstructionSetVar:
      error = executeSetVariable(script, c, c, gc, i.args);
      break;
    case InstructionGSetVar:
      error = executeSetVariable(script, gc, c, gc, i.args);
      break;
    case InstructionIf:
      error = executeIf(script, c, gc, i.args);
      break;
    case InstructionRandom:
      error = executeRandom(script, c, gc, i.args);
      break;
    default:
      break;
  }

  return error;
}
