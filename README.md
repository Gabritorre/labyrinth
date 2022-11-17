# labyrinth

## Execution on linux and MacOS

### Normal

	make && ./labyrinth.out

### Challenge

	make challenge && ./iap --challenge

## Execution on Windows

### Normal

	make && ./labyrinth.out

### Challenge

	gcc -std=c99 --pedantic *.c -o iap && iap.exe --challenge
