#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "script.h"
#include "string.h"
#include "utils/ht.h"

char *nextWord(FILE *f);
char *readLine(FILE *f);
void skipSpaces(FILE *f);
int nextLine(FILE *f);
void skipLine(FILE *f);

char *readScript(FILE *f, Script *s) {
  int line = 1;
  int allocated = 64;
  s->currentInstruction = 0;
  s->instructionsCount = 0;
  s->instructions = malloc(sizeof(Instruction) * allocated);
  s->labels = HashTableCreate();

  // TODO: free
  IfArgs **ifStack = NULL; // Array of IfArgs pointers
  int ifStackSize = 0;

  while (!feof(f)) {
    char *word = nextWord(f);

    Instruction instr;
    int insertInstr = 1;

    if (word[0] == '#' || word[0] == '\0') {
      // Comment or blank line, skip
      skipLine(f);
      insertInstr = 0;
    } else if (strcmp(word, "bgload") == 0) {
      BGLoadArgs *args = malloc(sizeof(BGLoadArgs));
      args->filename = nextWord(f);

      char *fadeStr = nextWord(f);
      if (fadeStr[0] == '\0') {
        args->fadein = 16;  // Default value when fade is ommited
      } else {
        args->fadein = atoi(fadeStr);
      }
      free(fadeStr);

      instr.args = args;
      instr.type = InstructionBGLoad;
    } else if (strcmp(word, "setimg") == 0) {
      SetIMGArgs *args = malloc(sizeof(SetIMGArgs));
      args->filename = nextWord(f);
      char *xStr = nextWord(f);
      char *yStr = nextWord(f);
      args->x = atoi(xStr);
      args->y = atoi(yStr);
      free(xStr);
      free(yStr);
      instr.type = InstructionSetIMG;
      instr.args = args;
    } else if (strcmp(word, "sound") == 0) {
      SoundArgs *args = malloc(sizeof(SoundArgs));
      args->filename = nextWord(f);

      char *timesStr = nextWord(f);
      if (timesStr[0] == '\0') {
        args->times = 1;  // Default value when fade is ommited
      } else {
        args->times = atoi(timesStr);
      }
      free(timesStr);

      instr.args = args;
      instr.type = InstructionSound;
    } else if (strcmp(word, "music") == 0) {
      MusicArgs *args = malloc(sizeof(MusicArgs));
      args->filename = nextWord(f);
      instr.args = args;
      instr.type = InstructionMusic;
    } else if (strcmp(word, "text") == 0) {
      TextArgs *args = malloc(sizeof(TextArgs));
      args->text = readLine(f);
      instr.args = args;
      instr.type = InstructionText;
    } else if (strcmp(word, "cleartext") == 0) {
      ClearTextArgs *args = malloc(sizeof(ClearTextArgs));
      char *hist = nextWord(f);
      args->shouldClearHistory = strcmp("!", hist) == 0 ? 1 : 0;
      free(hist);
      instr.args = args;
      instr.type = InstructionClearText;
    } else if (strcmp(word, "choice") == 0) {
      ChoiceArgs *args = malloc(sizeof(ChoiceArgs));
      args->choices = readLine(f);
      instr.type = InstructionChoice;
      instr.args = args;
    } else if (strcmp(word, "setvar") == 0 || strcmp(word, "gsetvar") == 0) {
      SetVarArgs *args = malloc(sizeof(SetVarArgs));
      args->variable = nextWord(f);

      char *op = nextWord(f);
      if (strcmp("=", op) == 0) {
        args->operation = ValueSet;
      } else if (strcmp("+", op) == 0) {
        args->operation = ValueSum;
      } else if (strcmp("-", op) == 0) {
        args->operation = ValueSub;
      } else if (strcmp("~", op) == 0) {
        args->operation = ValueClear;
      } else {
        char error[255];
        snprintf(error, sizeof(error), "unknown operation: \"%s\"", op);
        free(word);
        free(op);
        free(args->variable);
        free(args);
        return strdup(error);
      }
      free(op);

      args->value = nextWord(f);

      instr.args = args;
      instr.type = word[0] == 'g' ? InstructionGSetVar : InstructionSetVar;
    } else if (strcmp(word, "if") == 0) {
      IfArgs *args = malloc(sizeof(IfArgs));
      args->falseGoto = -1;
      args->variable = nextWord(f);

      char *op = nextWord(f);
      if (strcmp("==", op) == 0) {
        args->operation = TestEquals;
      } else if (strcmp("!=", op) == 0) {
        args->operation = TestNotEquals;
      } else if (strcmp("<", op) == 0) {
        args->operation = TestLess;
      } else if (strcmp("<=", op) == 0) {
        args->operation = TestLessOrEquals;
      } else if (strcmp(">", op) == 0) {
        args->operation = TestGreater;
      } else if (strcmp(">=", op) == 0) {
        args->operation = TestGreaterOrEquals;
      } else {
        char error[255];
        snprintf(error, sizeof(error), "unknown operation: \"%s\"", op);
        free(word);
        free(op);
        free(args->variable);
        free(args);
        return strdup(error);
      }
      free(op);

      args->right = nextWord(f);

      ifStackSize++;
      ifStack = realloc(ifStack, ifStackSize * sizeof(IfArgs*));
      ifStack[ifStackSize-1] = args;

      instr.type = InstructionIf;
      instr.args = args;
    } else if (strcmp(word, "fi") == 0) {
      // Close the last if, pop it from the stack
      insertInstr = 0;
      ifStack[ifStackSize-1]->falseGoto = s->instructionsCount;
      ifStackSize--;
      ifStack = realloc(ifStack, ifStackSize * sizeof(IfArgs*));
    } else if (strcmp(word, "jump") == 0) {
      JumpArgs *args = malloc(sizeof(JumpArgs));
      args->filename = nextWord(f);
      args->label = nextWord(f);
      instr.type = InstructionJump;
      instr.args = args;
    } else if (strcmp(word, "delay") == 0) {
      DelayArgs *args = malloc(sizeof(args));
      char *delayStr = nextWord(f);
      args->delay = atoi(delayStr);
      free(delayStr);
      instr.args = args;
      instr.type = InstructionDelay;
    } else if (strcmp(word, "label") == 0) {
      insertInstr = 0;
      char *label = nextWord(f);
      int *index = malloc(sizeof(int));
      *index = s->instructionsCount;
      HashTableSet(s->labels, label, index);
      free(label);
    } else if (strcmp(word, "goto") == 0) {
      GotoArgs *args = malloc(sizeof(GotoArgs));
      args->label = nextWord(f);
      instr.type = InstructionGoto;
      instr.args = args;
    } else {
      char error[255];
      snprintf(error, sizeof(error), "unknown instruction: \"%s\"", word);
      free(word);
      return strdup(error);
    }

    line++;
    if (insertInstr) {
      s->instructions[s->instructionsCount] = instr;
      s->instructionsCount++;
      if (s->instructionsCount >= allocated) {
        allocated *= 2;
        s->instructions = realloc(s->instructions, sizeof(Instruction) * allocated);
      }
    }

    free(word);
    if (!nextLine(f)) {
      char c = getc(f);
      if (c == '#') {
        // Rest of the line is a comment, skip
        skipLine(f);
      } else {
        char error[255];
        snprintf(error, sizeof(error), "unexpected token: '%c'", c);
        return strdup(error);
      }
    }
  }

  s->instructions =
      realloc(s->instructions, sizeof(Instruction) * s->instructionsCount);
  return NULL;
}

void skipSpaces(FILE *f) {
  int peek = getc(f);
  while (peek == ' ' || peek == '\t' || peek == '\r') {
    peek = getc(f);
  }
  ungetc(peek, f);
}

char *nextWord(FILE *f) {
  int i = 0;
  int strLen = 0;
  int allocated = 16;
  char *str = malloc(allocated);

  // Trim starting spaces
  skipSpaces(f);

  int peek = getc(f);
  while (peek != EOF && peek != ' ' && peek != '\n' && peek != '\r') {
    str[strLen] = (char)peek;
    strLen++;

    if (strLen >= allocated) {
      allocated *= 2;
      str = realloc(str, allocated);
    }

    peek = getc(f);
  }
  ungetc(peek, f);

  // Trim ending spaces
  skipSpaces(f);

  // Shrink/resize to fit needed size
  str = realloc(str, strLen + 1);
  str[strLen] = '\0';
  return str;
}

char *readLine(FILE *f) {
  int i = 0;
  int strLen = 0;
  int allocated = 16;
  char *str = malloc(allocated);

  // Trim starting spaces
  skipSpaces(f);

  int peek = getc(f);
  while (peek != EOF && peek != '\n') {
    str[strLen] = (char)peek;
    strLen++;

    if (strLen >= allocated) {
      allocated *= 2;
      str = realloc(str, allocated);
    }

    peek = getc(f);
  }
  ungetc(peek, f);

  // Shrink/resize to fit needed size
  str = realloc(str, strLen + 1);
  str[strLen] = '\0';
  return str;
}

// Tries to move to the next line, fails if it finds non-blank characters.
// EOF still succeeds. Returns 1 on success, 0 on fail.
int nextLine(FILE *f) {
  skipSpaces(f);
  int c = getc(f);
  if (c != '\n' && c != EOF) {
    ungetc(c, f);
    return 0;
  }

  return 1;
}

void skipLine(FILE *f) {
  int c = getc(f);
  while (c != EOF && c != '\n') {
    c = getc(f);
  }
  ungetc(c, f);
}