app: main.c
	$(CC) main.c -o pmd-cli -Wall -Wextra -pedantic -lncurses -lm 
