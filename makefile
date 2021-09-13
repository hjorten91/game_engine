# set this variable to the director in which you saved the common files
commondir = common/

all : main

main : main.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c controls.c
	gcc -Wall -o main -I$(commondir) -Icommon/Linux -DGL_GLEXT_PROTOTYPES main.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c controls.c -lXt -lX11 -lGL -lm

clean :
	rm main
