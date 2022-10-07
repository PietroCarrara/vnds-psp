#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>

#include "../script.h"

typedef enum {
  StateBase,
  StateChoice,
  StateBGLoad,
} EngineState;

typedef struct SpriteList {
  SDL_Texture *sprite;
  struct SpriteList *next;
} SpriteList;

typedef struct {
  char *rootDir;
  Script *script;
  Context *context;
  Context *globalContext;

  SpriteList *sprites;
  SDL_Texture *background;

  EngineState state;
  void *stateData;

  SDL_Renderer *renderer;
} VisualNovel;

// Root path without trailing slash '/'
char *createVisualNovel(char *root, SDL_Renderer*, VisualNovel **);
void updateVisualNovel(VisualNovel *vn);
void drawVisualNovel(VisualNovel *vn);
void destroyVisualNovel(VisualNovel *vn);

void engineGotoBase(VisualNovel *);
void engineDrawBase(VisualNovel *);

#endif