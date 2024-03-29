#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "lib/prototype.h"
#include "lib/general_purpose.c"
#include "lib/tail_vector.c"
#include "lib/game_mode/interactive.c"
#include "lib/game_mode/cpu.c"
/**
* Elenca tutte le mappe che sono presenti nel file di testo "maps.txt".
* @param file file da cui estrarre le mappe.
* @return il numero di mappe trovate.
*/
int list_maps(FILE *file) {
	char character = fgetc(file);
	int map_counter = 0;
	printf("MAPS:\n");

	if (character != EOF) {
		printf("%d)\n", ++map_counter);
		int new_line_counter = 0;
		while (character != EOF) {
			if(new_line_counter >= 2 && character != '\n') {
				printf("%d)\n", ++map_counter);
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

/**
 * Cerca la mappa scelta dall'utente e si salva tutte le informazione necessarie: coordinate di Snake, dell'uscita; le dimensioni della mappa
 * @param file file contentente le mappe
 * @param map_info contiene le informazioni della mappa
 * @param map_number numero della mappa scelta dall'utente
 * @return ritorna la riga in cui inizia la mappa scelta dall'utente
*/
int get_map_info(FILE *file, map* map_info, int map_number) {
	map_info->row = 1;
	map_info->column = 1;
	map_info->player_row = -1;
	map_info->player_column = -1;
	map_info->exit_row = -1;
	map_info->exit_column = -1;

	fseek(file, 0, SEEK_SET); //rileggo il file da capo
	int map_start_char = -1; //indice del caratte di partenza della matrice scelta dall'utente
	char character = fgetc(file);
	int map_counter = 1;
	bool saving = false; // flag che: se vero allora stiamo leggendo la mappa scelta dall'utente

	if (map_number == 1) { // se la mappa scelta dall'utente è la prima allora inizzializzo a vero
		saving = true;
	}

	bool searching = true; // diventa false quando abbiamo già finito di leggere la mappa scelta dall'utente
	int new_line_counter = 0; //conta i '\n'
	int column_counter = 0;
	for (int i = 0; character != EOF && searching; i++) { // interrompe se file è finito o se abbiamo finito di analizzare la mappa scelta dll'utente
		if (map_start_char == -1 && saving) { // salvo la posizione del primo carattere della mappa scelta dall'utente
			map_start_char = i;
		}

		if (character == '\n') {
			new_line_counter++;
			if (saving) {
				map_info->row++;
				if (map_info->column < column_counter) { //bisogna prendere come colonna massima il punto più a destra della mappa (considerando delle sporgenze nel mezzo)
					map_info->column = column_counter;
				}
			}
			if (new_line_counter >= 2) { // se si incontrano 2 '\n' di seguito allora si sta cambiando mappa
				map_counter++;
				if (saving) { // se si stava leggendo la mappa interessata allora la mappa è finita e si può terminare di leggere il file
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
			if (character == 'o' && saving) { // salvo le coordinate del giocatore
				map_info->player_row = map_info->row-1; // map_info->row è un contatore di righe, ma dato che ho bisogno della posizione esatta (partendo da 0) sottraggo 1
				map_info->player_column = column_counter;
			}
			if (character == '_' && saving) {
				map_info->exit_row = map_info->row-1; // map_info->row è un contatore di righe, ma dato che ho bisogno della posizione esatta (partendo da 0) sottraggo 1
				map_info->exit_column = column_counter;
			}
			new_line_counter = 0;
			column_counter++;
		}
		character = fgetc(file);
	}
	// printf("mappa: righe: %d colonne: %d\n", map_info->row, map_info->column);
	// printf("pos. player: righa: %d colonna: %d\n", map_info->player_row, map_info->player_column);
	// printf("pos. uscita: righa: %d colonna: %d\n", map_info->exit_row, map_info->exit_column);

	return map_start_char;
}
/**
 * Salva la mappa scelta dall'utente mettendola nella matrice di gioco
 * @param map_info contiene le informazioni della mappa
 * @param map_line il punto d'inzio della mappa scelta dall'utente
 * @param map la matrice dove salvare la mappa
*/
void save_map(map* map_info, int map_line, char map[map_info->row][map_info->column]) {
	FILE *file;
	file = fopen("maps.txt", "r");
	if (NULL == file) {
		printf("\n\tError while opening the file. \n\tPlease check if the maps.txt file is in the same directory of the executable file\n");
		exit(1);
	}
	fseek(file, map_line, SEEK_SET); //rileggo il file dalla mappa scelta dall'utente
	char character = fgetc(file);
	bool fill_columns = false;

	//salvo la mappa in una matrice
	for (int row = 0; row < map_info->row; row++) {
		for (int column = 0; column < map_info->column; column++) {
			if (!fill_columns) { // finchè si stanno riempiendo le colonne rimanenti non si va avanti a leggere la mappa
				map[row][column] = character;
				character = fgetc(file);
			}
			if (character == '\n') {
				if(column+1 < map_info->column) { // nel caso in cui ci sia un a copo però non sono stato riempite tutte le colonne allora prima di continuare a leggere la mappa si riempiono di spazi le colonne rimanenti
					map[row][column+1] = ' ';
					fill_columns = true;
					continue;
				}
				fill_columns = false;
				character = fgetc(file);
			}
		}
	}
}

/**
 * Controlla che la mappa scelta dall'utente rispetti le proprietà necessarie: avere Snake, avere l'uscita e la mappa deve essere sufficientemente grande
 * @param map_info contiene le informazioni della mappa
 * @return 1 se la mappa non soddisfa i requisiti altrimenti restituisce 0
*/
int check_map_proprieties(map* map_info) {
	if (map_info->row <= 2 || map_info->column <= 2) {
		printf("\tThe map dimensions are not large enough\n");
		return 1;
	}
	if (map_info->player_row == -1 || map_info->player_column == -1) {
		printf("\tPlayer not found in the map\n");
		return 1;
	}
	if (map_info->exit_row == -1 || map_info->exit_column == -1) {
		printf("\tExit not found in the map\n");
		return 1;
	}
	return 0;
}
/**
 * Legge una mappa dallo user input
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
*/
void take_map_cmd_line(map* map_info, char map[map_info->row][map_info->column]) {
	map_info->player_row = -1;
	map_info->player_column = -1;
	map_info->exit_row = -1;
	map_info->exit_column = -1;

	for (int i = 0; i < map_info->row; i++) {
		char line[map_info->column];
		scanf(" %[^\n]%*c", line); //leggi l'input finche non trovi \n e quando viene trovata non viene salvata
		for (int j = 0; j < map_info->column; j++) {
			map[i][j] = line[j];
			if (map[i][j] == PLAYER) {
				map_info->player_row = i;
				map_info->player_column = j;
			}
			if (map[i][j] == EXIT) {
				map_info->exit_row = i;
				map_info->exit_column = j;
			}
		}
	}
}

/**
 * Prende la mappa dal file di testo
 * @param map_info contiene le informazioni della mappa
 * @return il punto nel file di testo dove inizia la mappa scelta dall'utente, altrimenti ritorna -1 in caso di errore
*/
int take_map_text_file(map* map_info) {
	FILE *file;
	file = fopen("maps.txt", "r");
	if (NULL == file) {
		printf("\n\tError while opening the file\n");
		exit(1);
	}
	int map_quantity = list_maps(file);
	int user_map_number;

	if (map_quantity) {
		bool choose_map = true;
		short counter_error = 0; // in caso di input errato il programma resetta per evitare cicli infiniti
		while (choose_map && counter_error != 10) {
			printf("Choose the map using the corresponding number > ");
			scanf(" %d", &user_map_number);
			if (user_map_number > 0 && user_map_number <= map_quantity) {
				choose_map = false;
			}
			else {
				printf("The inserted value is not valid. Please retry...\n");
				counter_error++;
			}
		}
		if (counter_error == 10) {
			printf("Error: Too many invalid values inserted\n");
			getchar(); // pulisce l'input buffer
			return -1;
		}
	}
	else {
		printf("No maps found");
		fclose(file);
		return -1;
	}

	int map_line = get_map_info(file, map_info, user_map_number);
	if (check_map_proprieties(map_info)) { // ripeti se la mappa scelta non soddisfa i requisiti necessari
		fclose(file);
		return -1;
	}

	fclose(file);
	return map_line;
}

/**
 * All'interno del main si navigano i menu
*/
int main(int argc, char *argv[]) {
	bool play = true;
	while (play) {
		printf("Game started!\n");
		char user_selection[50];
		if (argc > 1 && strcmp(argv[1], "--cpu") == 0) {
			user_selection[0] = '2';
			play = false;
		}
		else {
			clear_screen();
			title();
			main_menu();
			scanf(" %s", user_selection);
			clear_screen();
		}

		//modalità interativa
		if (user_selection[0] == '1' && user_selection[1] == 0) {
			printf("\tInteractive mode selected\n");
			map map_info;
			int map_line;
			bool choosing_input = true;
			while (choosing_input) {
				choosing_input = false;
				input_type_menu();
				scanf(" %s", user_selection);
				//prende la mappa dal file di testo
				if (user_selection[0] == '1' && user_selection[1] == 0) {
					map_line = take_map_text_file(&map_info);
					//mappa non valida / troppi valori non validi dati in input / nessuna mappa trovata
					if (map_line == -1) {
						choosing_input = true;
						continue;
					}
					char map[map_info.row][map_info.column];
					save_map(&map_info, map_line, map);
					start_interactive_mode(&map_info, map);
				}
				//prende la mappa dal user input
				else if (user_selection[0] == '2' && user_selection[1] == 0) {
					scanf(" %d", &map_info.column);
					scanf(" %d", &map_info.row);

					char map[map_info.row][map_info.column];
					take_map_cmd_line(&map_info, map);
					if (check_map_proprieties(&map_info)) {
						choosing_input = true;
						continue;
					}
					start_interactive_mode(&map_info, map);
				}
				//torna al menu precedente
				else if (user_selection[0] == '3' && user_selection[1] == 0) {}

				else {
					printf("\n\tInput not valid, please retry...(press enter)\n");
					choosing_input = true;
					getchar();
					getchar();
				}
			}
		}

		//modalità CPU
		else if (user_selection[0] == '2' && user_selection[1] == 0) {
			if (!(argc > 1 && strcmp(argv[1], "--cpu") == 0)) {
				printf("\tCPU mode selected\n");
			}
			map map_info;
			int map_line;
			bool choosing_input = true;
			while (choosing_input) {
				choosing_input = false;
				bool force_quit = false;
				if (argc > 1 && strcmp(argv[1], "--cpu") == 0) {
					user_selection[0] = '2';
					force_quit = true;
				}
				else {
					input_type_menu();
					scanf(" %s", user_selection);
					clear_screen();
				}

				//prende la mappa dal file di testo
				if (user_selection[0] == '1' && user_selection[1] == 0) {
					map_line = take_map_text_file(&map_info);
					//mappa non valida / troppi valori non validi dati in input / nessuna mappa trovata
					if (map_line == -1) {
						choosing_input = true;
						continue;
					}
					char map[map_info.row][map_info.column];
					save_map(&map_info, map_line, map);
					cpu_algorithm(&map_info, map, force_quit);
				}
				//prende la mappa dal user input
				else if (user_selection[0] == '2' && user_selection[1] == 0) {
					scanf(" %d", &map_info.column);
					scanf(" %d", &map_info.row);

					char map[map_info.row][map_info.column];
					take_map_cmd_line(&map_info, map);
					if (check_map_proprieties(&map_info)) {
						choosing_input = true;
						continue;
					}
					cpu_algorithm(&map_info, map, force_quit);
				}
				//torna al menu precedente
				else if (user_selection[0] == '3' && user_selection[1] == 0) {}

				else {
					printf("\n\tInput not valid, please retry...(press enter)\n");
					choosing_input = true;
					getchar();
					getchar();
				}
			}
		}

		else if (user_selection[0] == '3' && user_selection[1] == 0) {
			print_game_info();
			printf("\nPress enter to continue");
			getchar();
			getchar();
		}

		else if (user_selection[0] == '4' && user_selection[1] == 0) {
			printf("bye bye!\n");
			play = false;
		}

		else {
			printf("\n\tInput not valid, please retry...(press enter)\n");
			getchar();
			getchar();
		}
	}
	return 0;
}

