#include "engine.h"

#include <stdio.h>
#include <stdlib.h>

#include "../string.h"
#include "bgload.h"
#include "choice.h"

char *createVisualNovel(char *root, SDL_Renderer *r, VisualNovel **res) {
  VisualNovel *vn = malloc(sizeof(VisualNovel));

  char path[255];
  snprintf(path, sizeof(path), "%s/script/main.scr", root);
  FILE *f = fopen(path, "r");

  vn->state = StateBase;
  vn->stateData = NULL;
  vn->context = makeContext();
  vn->globalContext = makeContext();
  vn->rootDir = strdup(root);
  vn->renderer = r;

  char *err = readScript(f, &vn->script);
  fclose(f);
  if (err != NULL) {
    return err;
  }

  *res = vn;
  return NULL;
}

void updateVisualNovel(VisualNovel *vn) {
  while (vn->state == StateBase &&
         vn->script->currentInstruction < vn->script->instructionsCount) {
    Instruction ins;

    char *err = scriptExecuteNext(vn->script, vn->context, vn->globalContext, &ins);
    if (err != NULL) {
      printf("%s\n", err);
      free(err);
      // TODO: Throw
    }

    switch (ins.type) {
      case InstructionChoice:
        engineGotoChoice(vn, ins.args);
        break;
      case InstructionBGLoad:
        engineGotoBGLoad(vn, ins.args);
        break;
    }
  }

  switch (vn->state) {
    case StateBGLoad:
      engineUpdateBGLoad(vn);
      break;
    case StateChoice:
      engineUpdateChoice(vn);
      break;
  }

  // if (ins.type == InstructionText) {
  //   TextArgs *args = ins.args;
  //   char *txt = evalString(args->text, c, gc);

  //   if (txt[0] == '@') {
  //     printf("%s\n", txt + 1);
  //   } else if (strcmp("!", txt) == 0) {
  //     // system("clear");
  //   } else if (strcmp("~", txt) == 0) {
  //     // system("read");
  //     // system("clear");
  //   } else {
  //     printf("%s\n", txt);
  //     // system("read");
  //   }
  //   free(txt);
  // } else if (ins.type == InstructionJump) {
  //   JumpArgs *args = ins.args;
  //   char *fname = evalString(args->filename, c, gc);
  //   FILE *f = fopen(fname, "r");
  //   if (f == NULL) {
  //     printf("%s(%s): file not found!\n", args->filename, fname);
  //     free(fname);
  //     break;
  //   }
  //   free(fname);
  //   Script old = s;
  //   char *err = readScript(f, &s);
  //   fclose(f);
  //   if (err != NULL) {
  //     printf("%s:%s\n", argv[i], err);
  //     free(err);
  //     break;
  //   }

  //   if (args->label[0] != '\0') {
  //     char *label = evalString(args->label, c, gc);
  //     int *l = scriptGetLabel(&s, label);
  //     free(label);
  //     if (l != NULL) {
  //       s.currentInstruction = *l;
  //     } else {
  //       printf("Could not find label \"%s\"!\n", args->label);
  //       break;
  //     }
  //   }

  //   scriptDestroy(old);
  // } else {
  //   char *iStr = instructionToString(ins);
  //   printf("%s\n", iStr);
  //   free(iStr);
  // }
}

void drawVisualNovel(VisualNovel *vn) {
  SDL_SetRenderDrawColor(vn->renderer, 100, 149, 237, 255);
  SDL_RenderClear(vn->renderer);

  switch (vn->state) {
    case StateBGLoad:
      engineDrawBGLoad(vn);
      break;
    case StateChoice:
      engineDrawChoice(vn);
      break;
    default:
      engineDrawBase(vn);
  }

  SDL_RenderPresent(vn->renderer);
}

void engineGotoBase(VisualNovel *vn) { vn->state = StateBase; }

void engineDrawBase(VisualNovel *vn) {
  if (vn->background != NULL) {
    SDL_RenderCopy(vn->renderer, vn->background, NULL, NULL);
  }
}

void destroyVisualNovel(VisualNovel *vn) {
  scriptDestroy(*vn->script);
  free(vn->script);

  destroyContext(vn->context);
  destroyContext(vn->globalContext);

  free(vn->stateData);

  free(vn->rootDir);
  free(vn);
}