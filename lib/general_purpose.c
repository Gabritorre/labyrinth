/**
* @file general_purpose.c
* @brief Contiene le funzioni di stampa particolarmente lunghe (menu), funzioni utilizzate da entrambe le modalità di gioco
*/

/**
 * Rimuove tutti i caratteri TAIL dalla mappa
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 */
void clear_map_tail(map* map_info, char map[map_info->row][map_info->column]) {
	for (int row = 0; row < map_info->row; row++) {
		for (int column = 0; column < map_info->column; column++) {
			if (map[row][column] == TAIL) {
				map[row][column] = ' ';
			}
		}
	}
}

/**
 * Inserisce la coda nella mappa
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param tail vettore contentente i nodi della coda
*/
void insert_tail_in_map(map* map_info, char map[map_info->row][map_info->column], vector* tail) {
	if (tail == NULL) {// fine lista, cioè lista vuota
		return;
	}
	if (tail->row != -1) { // le il pezzo di coda esiste
		if (map[tail->row][tail->column] != FLAG) {
			map[tail->row][tail->column] = TAIL;
		}
	}
	insert_tail_in_map(map_info, map, tail->next);
}


/**
 * Appende la mossa appena fatta alla sequenza di mosse
 * @param steps contiene la lista dei passi fatti fino ad ora
 * @param max_steps_size contiene la dimensione massima attuale della lista dei passi fatti
 * @param move la mossa da appendere
 */
char* build_sequence (char **steps, int *max_steps_size, char move) {
	if (strlen(*steps) >= *max_steps_size-1) {
		*max_steps_size += 5;
	}

	char *sequence = (char*) calloc(*max_steps_size, sizeof(char));
	strncat(sequence, *steps, strlen(*steps));
	strncat(sequence, &move, 1);
	strcat(sequence, "\0");
/*	printf("\nmax_step_size: %d\n", *max_steps_size);*/
/*	printf("old_sequence_len: %ld\n", strlen(*steps));*/
/*	printf("sequence: %s\n", sequence);*/
/*	printf("sequence_len: %ld\n", strlen(sequence));*/

	free(*steps);
	return sequence;
}

/**
 * Controlla il contenuto della prossima cella che si vuole visitare, comportandosi di conseguenza in base al contenuto.\n
 * Vengono gestiti i casi in cui nella prossima cella c'è: l'uscita, una moneta, il malus '!', un trapano, un muro oppure un pezzo di coda.
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param next_step contiene il carattere della prossima posizione in cui andrà Snake
 * @param win flag che diventa vero quando snake ha raggiunto l'uscita, altrimenti è falso
 * @param points contiene i punti attuali della partita
 * @param tail vettore della coda di Snake
 * @param next_row riga successiva in cui andrà snake
 * @param next_column colonna successiva in cui andrà snake
*/
void check_next_step(map* map_info, char map[map_info->row][map_info->column], char next_step, bool *win, int *points, vector** tail, int next_row, int next_column) {
	if (next_step == BONUS_POINTS) {
		*points += QUANTITY_BONUS;
		tail_append(tail, map_info);
	}
	else {
		if (next_step == EXIT) {
			*win = true;
		}

		else if (next_step == HALF_POINTS && map_info->tail_len > 0) {
			//calcolo la metà della coda
			int middle_node = map_info->tail_len/2;
			// trovo il nodo che sta in mezzo alla coda
			if (map_info->tail_len % 2 != 0) {
				middle_node = (map_info->tail_len + 1) / 2;
			}
			// elimino la coda fino a metà
			delete_half_tail(tail, middle_node, points, map_info, map);
		}

		else if (next_step == DRILL) {
			map_info->drill_counter += 3;
		}

		if (next_step == WALL && map_info->drill_counter > 0) {
			map_info->drill_counter -= 1;
		}
		move_tail(map_info, tail, map);
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
		// se incontro un pezzo di coda, resetto i nodi fino al punto incontrato
		if (map[next_row][next_column] == TAIL) {
/*			printf("\ncoda (prima di eliminazione): ");*/
/*			print_vector(*tail);*/
/*			printf("\nlen coda %d\n", map_info->tail_len);*/
			reset_nodes_till(tail, next_row, next_column, points, map_info, map);
		}
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
	}
/*	printf("\ncoda: ");*/
/*	print_vector(*tail);*/
/*	printf("\nlen coda %d\n", map_info->tail_len);*/
}


/**
 * Si occupa di eseguire una mossa che gli viene passata come parametro, assicurandosi che sia una mossa legale. \n
 * Inoltre modifica il punteggio, la sequenza di mosse e aggiorna la mappa con le relative informazioni.
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param move la mossa da eseguire
 * @param points contiene i punti attuali della partita
 * @param steps contiene la lista dei passi fatti fino ad ora
 * @param steps_size contiene la dimensione massima attuale della lista dei passi fatti
 * @param ai_flag un flag che cambia il ritorno in caso di chiamata del metodo dalla modalità ia oppure dalla modalità interattiva
 * @param tail vettore della coda di Snake

 * @return Se la modalità ai è attiva il ritorno significa che la mossa è stata fatto correttamente.\n
 * Altrimenti il ritorno indica se il giocatore ha vinto la partità o no*/
int run_move(map* map_info, char map[map_info->row][map_info->column], char move, int *points, char **steps, int *steps_size, bool ai_flag, vector** tail) {
	bool win = false;
	char next_step; //conterrà il contenuto della cella del passo successivo
	switch (move) {
		case NORD:
			if (map_info->player_row - 1 >= 0) {
				next_step = map[map_info->player_row - 1][map_info->player_column]; // ottengo il carattere contenuto nel nuovo passo da fare
				if ((next_step != WALL || map_info->drill_counter > 0) && next_step != FLAG) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row - 1, map_info->player_column);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row -= 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;

					*steps = build_sequence(steps, steps_size, NORD);
					if (ai_flag) {
						return 1;
					}
				}
			}
			if (ai_flag) {
				return 0;
			}
			break;
		case EST:
			if (map_info->player_column + 1 < map_info->column) {
				next_step = map[map_info->player_row][map_info->player_column + 1];
				if ((next_step != WALL || map_info->drill_counter > 0) && next_step != FLAG) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row, map_info->player_column + 1);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_column += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;

					*steps = build_sequence(steps, steps_size, EST);
					if (ai_flag) {
						return 1;
					}
				}
			}
			if (ai_flag) {
				return 0;
			}
			break;
		case SUD:
			if (map_info->player_row + 1 < map_info->row) {
				next_step = map[map_info->player_row + 1][map_info->player_column];
				if ((next_step != WALL || map_info->drill_counter > 0) && next_step != FLAG) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row + 1, map_info->player_column);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;

					*steps = build_sequence(steps, steps_size, SUD);
					if (ai_flag) {
						return 1;
					}
				}
			}
			if (ai_flag) {
				return 0;
			}
			break;
		case OVEST:
			if (map_info->player_column - 1 >= 0) {
				next_step = map[map_info->player_row][map_info->player_column - 1];
				if ((next_step != WALL || map_info->drill_counter > 0) && next_step != FLAG) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row, map_info->player_column - 1);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_column -= 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;

					*steps = build_sequence(steps, steps_size, OVEST);
					if (ai_flag) {
						return 1;
					}
				}
			}
			if (ai_flag) {
				return 0;
			}
			break;
		default:
			break;
	}
	if (win) {
		return 1;
	}
	return 0;
}

//--------------------- menus and long prints

/**Stampa il titolo di gioco*/
void title() {
	printf(" -------------------\n");
	printf("|  SNAKE LABYRINTH  |\n");
	printf(" -------------------\n");
}

/**Stampa il menu principale del gioco*/
void main_menu() {
	printf("Choose the game mode:\n");
	printf("1: Interactive mode\n");
	printf("2: CPU mode\n");
	printf("3: About the game\n");
	printf("4: Quit\n");
	printf("Insert the corresponding number > ");
}

/**Stampa il menu di scelta di come impostare la mappa (se sceglierne una esistente oppure passarne una da input)*/
void input_type_menu() {
	printf("Choose how to insert the map:\n");
	printf("1: Choose from predefined maps\n");
	printf("2: Insert map by command line\n");
	printf("3: Back\n");
	printf("Insert the corresponding number > ");
}

/**Stampa le informazione del gioco*/
void print_game_info() {
	title();
	printf("Snake labyrinth is a game with two game mode:\n");
	printf("\tInteractive mode: your goal is to get Snake out of the labyrinth by moving in four directions.\n"
			"\tThe directions with the corresponding keybindings are:\n"
				"\t\t- 'W'/'w' north (upward movement)\n"
				"\t\t- 'A'/'a' west (leftward movement)\n"
				"\t\t- 'S'/'s' south (downward movement)\n"
				"\t\t- 'D'/'d' east (rightward movement)\n");
	printf("\tYou start with a base of 1000 points.\n\tOnce you exit the labyrinth, you will receive the total score based on your path and on some modifiers that are placed in the map:\n"
			"\tThe modifiers are the following:\n"
				"\t\t- '$' you get 10 extra points, and a piece of tail is added to Snake\n"
				"\t\t- '!' halves the points earned with the coins, and also halves the length of Snake's tail\n"
				"\t\t- Each movement decreases the score by 1\n");
	printf("\tOther symbols present in the game are:\n"
				"\t\t- 'o' The head of Snake\n"
				"\t\t- 'x' A piece of Snake's tail\n" 
				"\t\t- '#' A piece of wall\n"
				"\t\t- '/' (only in CPU mode) A flag placed to mark dead ends\n"
				"\t\t- '_' The exit of the labyrinth\n"
				"\t\t- 'T' Drill, Once you get it, allows you to break three walls\n\n");

	printf("\tCPU Mode: In this mode, an algorithm attempts to solve a given labyrinth\n");
	printf("\t\tIt is possible to add your own maps by including them in the \"maps.txt\" file (ensure to separate each map with an empty line and leave 2 empty line at the end of the file)\n"
			"\t\talternatively, you can insert a temporary map directly through the command line using the following format:\n");

	printf("\t\t<number of columns>\n"
			"\t\t<number of rows>\n"
			"\t\t<lines of the map> one line at a time\n\n");
	printf("\t\texample:\n"
			"\t\t13\n"
			"\t\t5\n"
			"\t\t#o##########\n"
			"\t\t# $      $ #\n"
			"\t\t#   $$$$   #\n"
			"\t\t# $      $ #\n"
			"\t\t#_##########\n");
}

/**
 * Stampa la mappa di gioco
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
*/
void print_map(map* map_info, char map[map_info->row][map_info->column]) {
	for (int row = 0; row < map_info->row; row++) {
		printf("\n");
		for (int column = 0; column < map_info->column; column++) {
			printf("%c", map[row][column]);
		}
	}
	clear_map_tail(map_info, map);
}
/**
 * Pulisce lo schermo
*/
void clear_screen() {
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
