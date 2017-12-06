all: semcontrol.o
	gcc -o test semcontrol.c 

semcontrol.o: semcontrol.c
	gcc -c semcontrol.c 

run: all
	./test 