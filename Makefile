PROGRAM = b-fun
OBJS = gl_setup.o main.o math_utils.o shaders.o terrain_gen.o
HEADERS = gl_setup.h math_utils.h misc.h parameters.h shaders.h terrain_gen.h

CFLAGS += -std=c99 -Wall -Wextra -pedantic `sdl-config --cflags`
LDLIBS += -lm -lGLEW -lGL `sdl-config --libs`

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) $(PROGRAM)

# Rebuilds the whole program when a header file is changed. This is less than
# ideal, but stil the most straight-forward implementation. Plus this program
# is small enough that it doesn't matter.
%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@
