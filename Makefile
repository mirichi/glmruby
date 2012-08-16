PROG := glmruby.exe
SRCS := $(wildcard ./src/*.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
DEPS := $(SRCS:%.c=%.d)
CFLAGS = -g -O3
ALL_CFLAGS = -Wall -Werror-implicit-function-declaration $(CFLAGS)

INCLUDES = -I./include -I./../mruby/include -Ic:/ruby193/mingw/include/sdl
LIBDIR = -L./../mruby/lib -Lc:/ruby193/minw/lib
LIBS = -lmingw32 -lSDLmain -lSDL -mwindows -lSDL_Image -lmruby
OPTIONS = -D_GNU_SOURCE=1 -Dmain=SDL_main -DHAVE_OPENGL -mwindows

CC := gcc

all: $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(LIBDIR)

%.o: %.c
	$(CC) $(ALL_CFLAGS) -c -MMD $(INCLUDES) $< -o $@

clean:
	rm -f $(PROG) $(OBJS) $(DEPS)

