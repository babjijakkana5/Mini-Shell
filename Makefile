OBJ := $(patsubst %.c, %.o, $(wildcard *.c))

MINISHELL.exe: $(OBJ)
	gcc -o $@ $^

clean:
	rm -f *.o *.exe

