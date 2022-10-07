PSPSDK = $(shell psp-config --pspsdk-path)
PSPBIN = $(shell psp-config --psp-prefix)/bin

TARGET = VNDS_PSP
SDL_CONFIG = $(PSPBIN)/sdl2-config

OBJS   = psp/main.o psp/engine.o psp/bgload.o psp/choice.o \
				 instruction.o context.o script.o script_decode.o string.o \
				 utils/callback.o utils/ht.o

INCDIR =
CFLAGS = $(shell $(SDL_CONFIG) --cflags) -G0 -Wall -O2 -DPSP
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -D"TIXML_USE_STL"
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS    = -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lfreetype -lpng -ljpeg \
					-lpspvfpu -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgum \
				  -lpspgu -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
          -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -lpspvram \
          -lSDL2 -lGL -lGLU -lz \
					$(shell $(SDL_CONFIG) --libs)

BUILD_PRX = 1

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = VNDS PSP
# PSP_EBOOT_ICON= "ICON0.png"
# PSP_EBOOT_PIC1= "PIC1.png"
# PSP_EBOOT_SND0= "SND0.at3"

USE_PSPSDK_LIBC=1
include $(PSPSDK)/lib/build.mak