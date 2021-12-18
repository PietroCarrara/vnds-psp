#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef enum {
  InstructionBGLoad,
  InstructionSetIMG,
  InstructionSound,
  InstructionMusic,
  InstructionText,
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
} ValueOperator;

typedef enum {
  TestEquals,
  TestNotEquals,
  TestGreater,
  TestGreaterOrEquals,
  TestLess,
  TestLessOrEquals,
} TestOperator;

typedef struct {
  // What instruction is this struct representing
  InstructionType type;
  // The arguments given to this instruction
  void *args;
} Instruction;

typedef struct {
  char *filename;
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
  char *choices;
} ChoiceArgs;

// Used for both setvar and gsetvar
typedef struct {
  char *variable, *value;
  ValueOperator operator;
} SetVarArgs;

typedef struct {
  TestOperator operation;
  char *variable, *right;
  int falseGoto;
} IfArgs;

typedef struct {
  char *filename, *label;
} JumpArgs;

typedef struct {
  int delay;
} DelayArgs;

typedef struct {
  char *variable, *low, *high;
} RandomArgs;

typedef struct {
  char *label;
} GotoArgs;

#endif