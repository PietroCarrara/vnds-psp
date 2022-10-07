#include "choice.h"

#include <math.h>

#define clamp(min, x, max) (x < min ? min : (x > max ? max : x))

typedef struct {
  ChoiceArgs* args;
  int choice;
} ChoiceData;

void engineGotoChoice(VisualNovel* vn, ChoiceArgs* args) {
  vn->state = StateChoice;
  ChoiceData* data = malloc(sizeof(ChoiceData));
  vn->stateData = data;

  data->args = args;
  data->choice = 0;
}

void engineUpdateChoice(VisualNovel* vn) {
  ChoiceData* data = vn->stateData;

  // TODO: Use SDL input
  // if (osl_keys->pressed.up) {
  //   data->choice--;
  // } else if (osl_keys->pressed.down) {
  //   data->choice++;
  // }
  data->choice = clamp(0, data->choice, data->args->len);

  // if (osl_keys->pressed.cross) {
  //   char chosen[13];
  //   snprintf(chosen, sizeof(chosen), "%d", data->choice);
  //   contextSetVariable(vn->context, "selected", chosen);

  //   free(data);
  //   vn->stateData = NULL;
  //   engineGotoBase(vn);
  // }
}

void engineDrawChoice(VisualNovel* vn) {
  engineDrawBase(vn);
  // ChoiceArgs* args = ins.args;
  // int chosen = 1, j;
  // char chosenStr[13];
  // for (j = 0; j < args->len; j++) {
  //   char* option = evalString(args->choices[j], vn->context, vn->globalContext);
  //   printf("%d: %s\n", j + 1, option);
  //   free(option);
  // }
  // scanf("%d", &chosen);
  // snprintf(chosenStr, sizeof(chosenStr), "%d", chosen);
  // contextSetVariable(vn->context, "selected", chosenStr);
}
