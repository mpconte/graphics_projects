OSTYPE := $(shell uname)
ifeq ($(OSTYPE), Darwin)
    CFLAGS= -DDARWIN
    LIB= -framework OpenGL -framework GLUT -framework Foundation
else
    CFLAGS= -I/usr/include
    LIB= -L/usr/lib -lglut -lGLU -lGL -lXmu -lXi -lXext -lX11 -lm
endif

CC= gcc
CFLAGS +=
SRC= camera.c grid.c mainUI.c model.c vector.c
OBJ= camera.o grid.o mainUI.o model.o vector.o
CMD= camera

$(CMD): $(OBJ)
	$(CC) $(CFLAGS) -o $(CMD) $(OBJ) $(LIB)

clean:
	rm -f $(OBJ) $(CMD)

$(OBJ): camera.h vector.h
