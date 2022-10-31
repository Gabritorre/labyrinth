#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "lib/game_info.h"
#include "lib/game_mode/interactive.h"

char list_maps(FILE *file) {
	char character = fgetc(file);
	int map_counter = 0;
	printf("MAPPE:\n");

	if(character != EOF) {
		printf("%d)\n", map_counter++);
		int new_line_counter = 0;
		while(character != EOF) {
			if(new_line_counter >= 2) {
				printf("%d)\n", map_counter++);
			}
			if (character == '\n') {
				new_line_counter++;
			}
			else {
				new_line_counter = 0;
			}

			printf("%c", character);
			character = fgetc(file);
		}
		return map_counter;
	}
	return 0;
}

// questo metodo ritorna la riga in cui inizia la mappa scelta dall'utente
int get_map_info(FILE *file, struct Map* map_info, int map_number) {
	map_info->row = 1;
	map_info->column = 1;
	fseek(file, 0, SEEK_SET); //rileggo il file da capo
	int map_start_char = -1; //indice del caratte di partenza della matrice scelta dall'utente
	char character = fgetc(file);
	int map_counter = 1;
	bool saving = false; // flag che: se vero allora stiamo leggendo la mappa scelta dall'utente

	if(map_number == 1) { // se la mappa scelta dall'utente è la prima allora inizzializzo a vero
		saving = true;
	}

	bool searching = true; // diventa false quando abbiamo già finito di leggere la mappa scelta dall'utente
	int new_line_counter = 0; //conta i '\n'
	int column_counter = 0;
	for (int i = 0; character != EOF && searching; i++) { // interrompe se file è finito o se abbiamo finito di analizzare la mappa scelta dll'utente
		if(map_start_char == -1 && saving) { // salvo la posizione del primo carattere della mappa scelta dall'utente
			map_start_char = i;
		}

		if (character == '\n') {
			new_line_counter++;
			if(saving) {
				map_info->row++;
				if (map_info->column < column_counter) { //bisogna prendere come colonna massima il punto più a destra della mappa (considerando delle sporgenze nel mezzo)
					map_info->column = column_counter;
				}
			}
			if(new_line_counter >= 2) { // se si incontrano 2 '\n' di seguito allora si sta cambiando mappa
				map_counter++;
				if(saving) { // se si stava leggendo la mappa interessata allora la mappa è finita e si può terminare di leggere il file
					searching = false;
					map_info->row -= 2;	//rimuove le ultime due righe vuote dal conto
					break;
				}
				if (map_counter == map_number) { // se stiamo leggendo la mappa scelta dall'utente attivo il flag
					saving = true;
				}
			}
			column_counter = 0; //resetta il contatore delle colonne ad ogni a capo
		}

		else {
			if (character == 'o') { // salvo le coordinate del giocatore
				map_info->player_row = map_info->row-1; // map_info->row è un contatore di righe, ma dato che ho bisogno della posizione esatta (partendo da 0) sottraggo 1
				map_info->player_column = column_counter;
			}
			if (character == '_') {
				map_info->exit_row = map_info->row-1; // map_info->row è un contatore di righe, ma dato che ho bisogno della posizione esatta (partendo da 0) sottraggo 1
				map_info->exit_column = column_counter;
			}
			new_line_counter = 0;
			column_counter++;
		}
		character = fgetc(file);
	}

	printf("mappa: righe: %d colonne: %d\n", map_info->row, map_info->column);
	printf("pos. player: righa: %d colonna: %d\n", map_info->player_row, map_info->player_column);
	printf("pos. uscita: righa: %d colonna: %d\n", map_info->exit_row, map_info->exit_column);

	return map_start_char;
}

void save_map(FILE *file, struct Map* map_info, int map_line, char map[map_info->row][map_info->column]) {
	fseek(file, map_line, SEEK_SET); //rileggo il file dalla mappa scelta dall'utente
	char character = fgetc(file);

	//salvo la mappa in una matrice
	for (int row = 0; row < map_info->row; row++) {
		for (int column = 0; column < map_info->column; column++) {
			if (character == '\n'){
				character = fgetc(file);
			}
			map[row][column] = character;
			character = fgetc(file);
		}
	}
}

int main() {
	bool play = true;
	while(play) {
		char game_mode[50];
		title();
		printf("Scegli modalita' di gioco:\n");
		printf("1: Modalita' interattiva\n");
		printf("2: Modalita' IA\n");
		printf("3: Info sul gioco\n");
		printf("4: Esci\n");
		printf("Premi il numero corrispondente: ");
		scanf(" %s", game_mode);

		if (game_mode[0] == '1' && game_mode[1] == 0) {
			printf("hai selezionato modalita' interattiva\n");
			FILE *file;
			file = fopen("maps.txt", "r");
			if (NULL == file) {
				printf("\n\tErrore nell'apertura del file\n");
				exit(1);
			}
			int map_quantity = list_maps(file);
			int user_map_number;
			printf("sono presenti %d mappe\n", map_quantity);

			if(map_quantity) {
				bool choose_map = true;
				short counter_error = 0; // in caso di input errato il programma resetta per evitare cicli infiniti
				while(choose_map && counter_error != 10) {
					printf("scegli la mappa con il numero corrispondente: ");
					scanf(" %d", &user_map_number);
					if (user_map_number > 0 && user_map_number <= map_quantity) {
						choose_map = false;
					}
					else {
						printf("valore inserito non valido. Riprova...\n");
						counter_error++;
					}
				}
				if (counter_error == 10) {
					printf("Troppi valori errati inseriti\n");
					getchar(); // pulisce l'input buffer
					continue;
				}

				struct Map map_info;

				int map_line = get_map_info(file, &map_info, user_map_number);
				char map[map_info.row][map_info.column];
				save_map(file, &map_info, map_line, map);
				start_interactive_mode(&map_info, map);
			}
			else {
				printf("Nessuna mappa trovata");
				play = false;
			}
			fclose(file);
			printf("\n\n");
		}

		else if (game_mode[0] == '2' && game_mode[1] == 0) {
			printf("hai selezionato modalita' IA\n");
		}

		else if (game_mode[0] == '3' && game_mode[1] == 0) {
			print_game_info();
		}

		else if (game_mode[0] == '4' && game_mode[1] == 0) {
			printf("Ciao ciao!\n");
			play = false;
		}

		else {
			printf("\n\tL'input inserito non e' valido, riprova...\n\n");
		}
	}
	return 0;
}

