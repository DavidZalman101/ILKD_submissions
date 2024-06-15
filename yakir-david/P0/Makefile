CC = gcc # Flag for implicit rules
CFLAGS = -Wall -Wextra -Wpedantic # Flag for implicit rules

shell: shell.o prompt.o commandline.o

shell.o: shell.c

prompt.o: prompt.c

commandline.o: commandline.c

clean:
	rm *.o shell
