#include <stdio.h>
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
		printf("dio scroto");
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

