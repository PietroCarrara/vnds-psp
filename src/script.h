#ifndef SCRIPT_H
#define SCRIPT_H

#include "context.h"
#include "instruction.h"
#include "utils/ht.h"

typedef struct Script {
  Instruction* instructions;  // Array of instructions
  int instructionsCount;      // Total number of instructions
  int currentInstruction;     // The instruction to be executed now
  HashTable* labels;          // Maps label names to instruction indexes
} Script;

// Gets the index a label points to. Null if label does not exist
int* scriptGetLabel(Script* s, const char* label);

// Tries executing a instruction and returns a string describing if any error
// occurred. If the last argument is not null, it receives the instruction that
// was executed
char* scriptExecuteNext(Script* script, Context* c, Context* gc,
                        Instruction* instr);

#endif