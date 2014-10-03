SOUNDSERVER = koules.sndsrv.linux
INSTALLSOUND = True
SOUNDDIR = ./sounds/
SOUNDOBJS = sound.o
CC  = /opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc
OPTIMIZE = -O2 -ffast-math -Dlinux -Wall -DHAVEUSLEEP 
CFLAGS = -std=gnu99 -DSDL32BPP -DGCW -DSOUND -I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include/SDL/ -I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include/ \
	-DSOUNDSERVER=\"$(SOUNDDIR)/$(SOUNDSERVER)\" \
	-DSOUNDDIR=\"$(SOUNDDIR)\" \
	-DSOUNDDEV=\"$(SOUNDDEV)\"
LFLAGS =  
PROGS = koules
PROGS_O  = koules.o sound.o menu.o gameplan.o font.o intro.o sdl/SDL_prims.o sdl/init.o sdl/SFont.o framebuffer.o cmap.o rcfiles.o server.o client.o sock.o objectsio.o
LIBS     = -lFLAC -lSDL_mixer -lz -lSDL -lm -lpthread -ldl -L/opt/gcw0-toolchain/usr/mipsel-gcw0-linux/uclibc/sysroot/usr/lib

all:	progs $(SOUNDSERVER)

progs : $(PROGS)

objs:	$(PROGS_O)

.c.o:
	$(CC) $(OPTIMIZE) $(CFLAGS) -c -o $*.o $<

.c.s:
	$(CC) $(OPTIMIZE) $(CFLAGS) -S -o $*.s $<

.o:
	$(CC) $(OPTIMIZE) $(CFLAGS) $(LFLAGS) -o $* $*.o $(LIBS)
	chmod a+rs,go-w $*

font.o: font.c
	@echo please be patient.
	@echo the font.c is big and ugly i ripped it from zgv. 
	@echo do you know about better one?
	$(CC) $(OPTIMIZE) $(CFLAGS) -c  $< 

koules.sndsrv.linux.o: koules.sndsrv.linux.c
	$(CC) $(OPTIMIZE) $(CFLAGS) -c  $< 

koules.o: koules.c
	$(CC) $(OPTIMIZE) $(CFLAGS) -Wall -c  $< 

koules: $(PROGS_O)
	$(CC) $(OPTIMIZE) $(CFLAGS) $(LFLAGS) -o koules $(PROGS_O) $(LIBS)

clean:
	rm -f koules
	rm -f core `find . -name '*.[oas]' -print`
	rm -f core `find . -name '*~' -print`
	rm -f core `find . -name '*.swp' -print`
	rm -f core `find . -type f -name 'core' -print`

indent:
#	indent -di16 *.c *.h svgalib/*.c svgalib/*.h xlib/*.c xlib/*.h
	indent -di16 *.c *.h sdl/*.c sdl/*.h


#include .depend
