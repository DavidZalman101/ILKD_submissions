CC = gcc # Flag for implicit rules
CFLAGS = -Wall -Wextra -Wpedantic # Flag for implicit rules

shell: shell.o utils.o prompt.o commandline.o

shell.o: shell.c

prompt.o: prompt.c

commandline.o: commandline.c

utils.o: utils.c

clean:
	rm *.o shell
