# labyrinth

## Execution on linux and MacOS

### Normal

	make && ./iap

### Challenge

	make challenge && ./iap --challenge

## Execution on Windows

### Normal

	gcc -o iap.exe && ./iap.exe

### Challenge

	gcc -std=c99 --pedantic *.c -o iap && iap.exe --challenge
