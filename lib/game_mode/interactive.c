/**
* @file interactive.c
* @brief Contentente le funzioni utilizzate dalla modalità interativa
*/

/**
 * Interpreta il comando di movimento inserito dall'utente
 * @param command il comando inserito dall'utente
 * @param move è dove il comando dell'utente viene salvato
 * @return 1 = uscito dalla partita \n
 * 2 = comando inserito non valido \n
 * 0 = corretto
 */
int command_interpreter(char command, char *move) {
	// printf("comando: %d\n", command);
	if (command == QUIT) {
		return 1;
	}
	else {
		if (toupper(command) != NORD && toupper(command) != SUD && toupper(command) != EST && toupper(command) != OVEST) {
			// printf("\nerrore dato da: %d %c\n", command, command);
			return 2;
		}
		*move = toupper(command);
	}
	return 0;
}

/** Fa partire la modalità interattiva.
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
*/
void start_interactive_mode(map* map_info, char map[map_info->row][map_info->column]) {
	bool playing = true;
	int points = 1000;
	vector *tail = NULL; // vettore che conterrà i nodi della coda, il primo elemento del vettore sarà la parte più lontana della coda, mentre l'ultimo elemnto del vettore è il nodo più vicino a snake
	map_info->tail_len = 0; // lunghezza della coda iniziale
	map_info->drill_counter = 0;
	int max_steps_size = map_info->row + map_info->column;
	char *all_steps = (char*) calloc(max_steps_size, sizeof(char));
	while (playing) {
		fflush(stdout);
		clear_screen();
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, tail);
		print_map(map_info, map);
		printf("\nSCORE: %d\n", points);
		printf("DRILL: %d\n", map_info->drill_counter);
		char move;
		char command;
		printf("Insert next move > ");
		scanf(" %c", &command);
		int ci_result = command_interpreter(command, &move);
		if (ci_result == 1) {
			printf("Exited the game\n");
			playing = false;
		}
		else if (ci_result == 2) {
			printf("The inserted sequence contains illegal values. Please retry");
		}
		else {
			if (run_move(map_info, map, move, &points, &all_steps, &max_steps_size, false, &tail)) {
				clear_screen();
				clear_map_tail(map_info, map);
				insert_tail_in_map(map_info, map, tail);
				print_map(map_info, map);
				printf("\n\tYOU REACHED THE EXIT!\n");
				printf("\t- You scored %d points\n", points);
				printf("\t- Sequence of steps: %s\n", all_steps);
				printf("\t- Number of steps: %ld\n", strlen(all_steps));
				playing = false;
				while (getchar() != '\n');
				printf("(Press enter to continue)");
				getchar();
			}
		}
	}
	dealloc_vector(&tail);
	free(all_steps);
}
