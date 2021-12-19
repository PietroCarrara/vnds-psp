#include "instruction.h"

#include <stdio.h>

#include "string.h"

char valueOperationToChar(ValueOperation op) {
  switch (op) {
    case ValueSet:
      return '=';
    case ValueSum:
      return '+';
    case ValueSub:
      return '-';
    case ValueClear:
      return '~';
    default:
      return '?';
  }
}
char *testOperationToString(TestOperation op) {
  switch (op) {
    case TestEquals:
      return "==";
    case TestNotEquals:
      return "!=";
    case TestLess:
      return "<";
    case TestLessOrEquals:
      return "<=";
    case TestGreater:
      return ">";
    case TestGreaterOrEquals:
      return ">=";
    default:
      return "unknown";
  }
}

char *instructionToString(Instruction i) {
  char buf[1024];
  BGLoadArgs *bgLoadArgs;
  SoundArgs *soundArgs;
  MusicArgs *musicArgs;
  SetVarArgs *setVarArgs;
  IfArgs *ifArgs;
  JumpArgs *jumpArgs;
  ClearTextArgs *clearTextArgs;
  TextArgs *textArgs;
  DelayArgs *delayArgs;
  ChoiceArgs *choiceArgs;
  GotoArgs *gotoArgs;

  switch (i.type) {
    case InstructionBGLoad:
      bgLoadArgs = i.args;
      snprintf(buf, sizeof(buf), "bgload %s %d", bgLoadArgs->filename,
               bgLoadArgs->fadein);
      break;
    case InstructionSound:
      soundArgs = i.args;
      snprintf(buf, sizeof(buf), "sound %s %d", soundArgs->filename,
               soundArgs->times);
      break;
    case InstructionMusic:
      musicArgs = i.args;
      snprintf(buf, sizeof(buf), "music %s", musicArgs->filename);
      break;
    case InstructionText:
      textArgs = i.args;
      snprintf(buf, sizeof(buf), "text %s", textArgs->text);
      break;
    case InstructionSetVar:
    case InstructionGSetVar:
      setVarArgs = i.args;
      snprintf(buf, sizeof(buf), "%ssetvar %s %c %s",
               i.type == InstructionGSetVar ? "g" : "", setVarArgs->variable,
               valueOperationToChar(setVarArgs->operation), setVarArgs->value);
      break;
    case InstructionIf:
      ifArgs = i.args;
      snprintf(buf, sizeof(buf), "if !(%s %s %s) goto %d", ifArgs->variable,
               testOperationToString(ifArgs->operation), ifArgs->right,
               ifArgs->falseGoto);
      break;
    case InstructionJump:
      jumpArgs = i.args;
      snprintf(buf, sizeof(buf), "jump %s %s", jumpArgs->filename, jumpArgs->label);
      break;
    case InstructionClearText:
      clearTextArgs = i.args;
      snprintf(buf, sizeof(buf), "cleartext %s",
               clearTextArgs->shouldClearHistory ? "!" : "");
      break;
    case InstructionDelay:
      delayArgs = i.args;
      snprintf(buf, sizeof(buf), "delay %d", delayArgs->delay);
      break;
    case InstructionChoice:
      choiceArgs = i.args;
      snprintf(buf, sizeof(buf), "choice %s", choiceArgs->choices);
      break;
    case InstructionGoto:
      gotoArgs = i.args;
      snprintf(buf, sizeof(buf), "goto %s", gotoArgs->label);
      break;
    default:
      snprintf(buf, sizeof(buf), "unknown");
  }

  return strdup(buf);
}
