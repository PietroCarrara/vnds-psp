#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspkernel.h>

#include "utils/callback.h"

#define VERS 1  // Talk about this
#define REVS 0

PSP_MODULE_INFO("VNDS PSP", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_MAX();

int main(void) {
  setupExitCallback();

  while (isRunning()) {
    // TODO: Run game...
  }

  sceKernelExitGame();

  return 0;
}