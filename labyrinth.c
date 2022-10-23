#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void title() {
	printf(" -------------------\n");
	printf("|  SNAKE LABYRINTH  |\n");
	printf(" -------------------\n");
}

void print_map() {
	FILE *ptr;

	ptr = fopen("maps.txt", "r");
	if (NULL == ptr) {
		printf("Errore nell'apertura del file");
	}

	char map[10][19];
	char character = fgetc(ptr);

	for (int row = 0; character != EOF; row++){
		for (int column = 0; character != EOF; column++){
			if (character == '\n'){
				character = fgetc(ptr);
				break;
			}
			map[row][column] = character;
			printf("%c", character);
			character = fgetc(ptr);
		}
	}

	fclose(ptr);
	printf("\n\nmappa:\n\n");
	for (int row = 0; row<10; row++){
		printf("\n");
		for (int column = 0; column<19; column++){
			printf("%c", map[row][column]);
		}
	}
}

void hello() {
	int row = 10;
	int* p = (int*)calloc(row, sizeof(int));
	for (int i = 0; i < row; i++) {
		p[i] = i;
	}

	for (int i = 0; i < row + 1; i++) {
		if(i == row) {
			p = realloc(p, row+1*sizeof(int));
			p[i] = i;
		}
		printf("%d\n", p[i]);
	}
}

void matrix() {
	int row = 10;
	int column = 10;

	int **p = (int**) calloc(row, sizeof(int*));
	for (int i = 0; i < row; i++) {
		p[i] = (int*) calloc(column, sizeof(int));
	}

	int counter = 1;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			p[i][j] = counter;
			counter++;
			printf("%d:%d = %d\n",i, j, p[i][j]);
			if (i == 7 && j == column - 1) {
				p[i] = realloc(p[i], (column + 1) * sizeof(int));
				p[i][column] = counter;
				counter++;
				printf("%d:%d = %d\n",i, column, p[i][column]);
			}
		}
	}
}


int main() {

	bool play = true;
	while(play) {
		char game_mode[30];
		title();
		printf("Scegli modalita' di gioco:\n");
		printf("1: Modalita' interattiva\n");
		printf("2: Modalita' IA\n");
		printf("3: Esci\n");
		printf("Premi il numero corrispondente: ");
		scanf(" %s", game_mode);

		if (game_mode[0] == '1' && game_mode[1] == 0) {
			printf("hai selezionato modalita' interattiva\n");
			print_map();
			printf("\n\n");
		}

		else if (game_mode[0] == '2' && game_mode[1] == 0) {
			printf("hai selezionato modalita' IA\n");
			//hello();
			matrix();
		}

		else if (game_mode[0] == '3' && game_mode[1] == 0) {
			printf("Ciao ciao!\n");
			play = false;
		}

		else {
			printf("\n\tL'input inserito non e' valido, riprova...\n\n");
		}

	}
	return 0;
}

