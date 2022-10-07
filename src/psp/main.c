#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>

#include "../utils/callback.h"
#include "engine.h"

#define VERS 1  // Talk about this
#define REVS 0

PSP_MODULE_INFO("VNDS PSP", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12 * 1024);

#undef main
int main(int argc, char *argv[]) {
  setupExitCallback();

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
  IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
  SDL_Window *window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 480, 272, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  VisualNovel *vn = NULL;
  createVisualNovel("Umineko no Naku Koro ni", renderer, &vn);

  int running = 1;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = 0;
          break;
        case SDL_CONTROLLERDEVICEADDED:
          SDL_GameControllerOpen(event.cdevice.which);
          break;
        case SDL_CONTROLLERBUTTONDOWN:
          if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START) running = 0;
          break;
      }
    }

    updateVisualNovel(vn);
    drawVisualNovel(vn);
  }
  destroyVisualNovel(vn);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  sceKernelExitGame();

  return 0;
}