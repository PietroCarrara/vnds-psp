#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "context.h"

typedef enum {
  InstructionBGLoad,
  InstructionSetIMG,
  InstructionSound,
  InstructionMusic,
  InstructionText,
  InstructionClearText,
  InstructionChoice,
  InstructionSetVar,
  InstructionGSetVar,
  InstructionIf,
  InstructionJump,
  InstructionDelay,
  InstructionRandom,
  InstructionGoto,
} InstructionType;

typedef enum {
  ValueSet,
  ValueSum,
  ValueSub,
  ValueClear,
} ValueOperation;

typedef enum {
  TestEquals,
  TestNotEquals,
  TestGreater,
  TestGreaterOrEquals,
  TestLess,
  TestLessOrEquals,
} TestOperation;

typedef struct {
  // What instruction is this struct representing
  InstructionType type;
  // The arguments given to this instruction
  void *args;
} Instruction;

typedef struct {
  char *filename;
  // Fadein in milliseconds
  int fadein;
} BGLoadArgs;

typedef struct {
  char *filename;
  int x, y;
} SetIMGArgs;

typedef struct {
  char *filename;
  int times;
} SoundArgs;

typedef struct {
  char *filename;
} MusicArgs;

typedef struct {
  char *text;
} TextArgs;

typedef struct {
  int shouldClearHistory;
} ClearTextArgs;

typedef struct {
  char **choices;
  int len;
} ChoiceArgs;

// Used for both setvar and gsetvar
typedef struct {
  char *variable, *value;
  ValueOperation operation;
} SetVarArgs;

typedef struct {
  TestOperation operation;
  char *variable, *right;
  int falseGoto;
} IfArgs;

typedef struct {
  char *filename, *label;
} JumpArgs;

typedef struct {
  // Delay in milliseconds
  int delay;
} DelayArgs;

typedef struct {
  char *variable, *low, *high;
} RandomArgs;

typedef struct {
  char *label;
} GotoArgs;

char *instructionToString(Instruction);

// Frees resources used by the instruction
void instructionDestroy(Instruction);

// Returns the options available to choose from in a choice instruction
char **choiceGetOptions(char *list, int *len);

#endif