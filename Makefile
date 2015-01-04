#--Source code--
OBJ = BulletHell.cpp

#--Compiler used--
CC = g++

#--Libraries we're linking against.--
LIBRARY_LINKS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#--Name of our exectuable--
OBJ_NAME = BulletHell

#--This is the target that compiles our executable--
all : $(OBJS)  
	$(CC) $(C++11) $(OBJ) $(LIBRARY_LINKS) -o $(OBJ_NAME)