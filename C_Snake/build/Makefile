.PHONY : clean
VPATH = ../src
TARGET=$(cmd)
ifeq ($(TARGET),)		#for release
	CFLAGS = -O -Wall
else ifeq ($(TARGET), debug)	#for develop
	CFLAGS = -g -Wall -DDEBUG
endif

CC = gcc -std=gnu99
OBJECTS = main.o stack.o snake.o get_conf.o
HEADERS = stack.h get_conf.h snake.h
LDFLAGS = -lcurses

build   : main
ifeq ($(TARGET),)
	$(shell if [ ! -d "release" ]; then mkdir "release";fi;)
	@mv main ./release/SnakeGame_Release
	@echo "Build the release version success"
	@cp -R ../conf ./release
else ifeq ($(TARGET), debug)
	$(shell if [ ! -d "debug" ]; then mkdir "debug";fi;)
	@mv main ./debug/SnakeGame_Debug
	@echo "Build the debug version success"
	@cp -R ../conf ./debug
endif

rebuild : clean main
	@echo "Rebuild success."
	
main : $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(LDFLAGS) $(OBJECTS)	
$(OBJECTS) : $(HEADERS)

clean : 
	-rm -fr $(OBJECTS)
	-rm -fr ./release/*
	-rm -fr ./debug/*
	@echo "Clean success"
