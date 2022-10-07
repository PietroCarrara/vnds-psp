#include "bgload.h"

#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include "../string.h"

typedef struct {
  BGLoadArgs *args;
  SDL_Texture *img;
  int initMillis;
} BGLoadData;

void engineGotoBGLoad(VisualNovel *vn, BGLoadArgs *args) {
  vn->state = StateBGLoad;
  BGLoadData *data = malloc(sizeof(BGLoadData));
  vn->stateData = data;

  char *fname = evalString(args->filename, vn->context, vn->globalContext);
  char path[255];
  snprintf(path, sizeof(path), "%s/background/%s", vn->rootDir, fname);
  free(fname);

  data->args = args;
  data->initMillis = SDL_GetTicks();

  // data->img = SDL_CreateTextureFromSurface(vn->renderer, img);
  // data->img = SDL_CreateTexture(vn->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 200, 200);
  data->img = IMG_LoadTexture(vn->renderer, path);
  SDL_SetTextureBlendMode(data->img, SDL_BLENDMODE_BLEND);
}

void engineUpdateBGLoad(VisualNovel *vn) {
  BGLoadData *data = vn->stateData;

  if (SDL_GetTicks() - data->initMillis >= data->args->fadein) {
    if (vn->background != NULL) {
      SDL_DestroyTexture(vn->background);
    }
    SDL_SetTextureAlphaMod(data->img, 255);
    vn->background = data->img;

    free(vn->stateData);
    vn->stateData = NULL;
    engineGotoBase(vn);
  }
}

void engineDrawBGLoad(VisualNovel *vn) {
  BGLoadData *data = vn->stateData;

  float completion = (SDL_GetTicks() - data->initMillis) / (float)data->args->fadein;
  // Easing: https://easings.net/#easeOutCubic
  completion = 1 - pow(1 - completion, 3);;
  int alpha = (int)(completion*255);
  if (alpha > 255) {
    alpha = 255;
  }

  engineDrawBase(vn);

  SDL_SetTextureAlphaMod(data->img, alpha);
  SDL_RenderCopy(vn->renderer, data->img, NULL, NULL);
}