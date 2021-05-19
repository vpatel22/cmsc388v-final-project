include config.mk

CC = cc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -Wno-unused-parameter -O3 \
				 ${FFTW_CFLAGS} ${SDL_CFLAGS}
LDLIBS = ${FFTW_LDLIBS} ${SDL_LDLIBS} -lm

OBJ = spectrm.o gui_sdl.o fb.o font.o rb.o util.o convert_freq.o
OBJ_JACK = ${OBJ} audio_jack.o

BINS = \
	${JACK_BINS}

all: ${BINS}

pitch: ${OBJ_JACK}
	${CC} ${CFLAGS} ${JACK_CFLAGS} -o $@ ${OBJ_JACK} ${LDLIBS} ${JACK_LDLIBS}

spectrm.o: spectrm.h gui.h fb.h rb.h audio.h util.h convert_freq.h
audio_jack.o: spectrm.h rb.h util.h
gui_sdl.o: fb.h util.h
fb.o: fb.h
rb.o: rb.h
convert_freq.o: convert_freq.h

CFLAGS.audio_jack.o = ${JACK_CFLAGS}

.c.o:
	${CC} ${CFLAGS} ${CFLAGS.$@} -c $<

clean:
	rm -f ${BINS}
	find . -type f -name '*.o' ! -name 'miniaudio.o' -delete

nuke: clean
	rm -f *.o

.PHONY: all clean
