TARGET = VNDS_PSP
OBJS   = main.o context.o script.o string.o utils/callback.o utils/ht.o

INCDIR   =
CFLAGS   = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS  = $(CFLAGS)

USE_PSPSDK_LIBC = 1

LIBDIR  =
LDFLAGS =
LIBS    = -lm

BUILD_PRX = 1

EXTRA_TARGETS    = EBOOT.PBP
PSP_EBOOT_TITLE  = VNDS PSP
# PSP_EBOOT_ICON  = ICON0.png
# PSP_EBOOT_PIC1  = PIC1.png
# PSP_EBOOT_SND0  = SND0.at3

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak