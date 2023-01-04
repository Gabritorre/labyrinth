/*
file contentente: funzioni di stampe particolarmente lunghe (menu), funzioni utilizzate da entrambe le modalità di gioco
*/

// rimuove tutti i caratteri TAIL dalla mappa
void clear_map_tail(map* map_info, char map[map_info->row][map_info->column]) {
	for (int row = 0; row < map_info->row; row++) {
		for (int column = 0; column < map_info->column; column++) {
			if (map[row][column] == TAIL) {
				map[row][column] = ' ';
			}
		}
	}
}

/*inserisce la coda nella mappa*/
void insert_tail_in_map(map* map_info, char map[map_info->row][map_info->column], vector* tail) {
	if (tail == NULL) // fine lista, cioè lista vuota
		return;
	if(tail->row != -1)
		map[tail->row][tail->column] = TAIL;

	insert_tail_in_map(map_info, map, tail->next);
}


/*appende la mossa appena fatta alla sequenza di mosse
 * parametri: la sequenza di mosse fatte, la dimensione della stringa, la mossa da appendere*/
char* build_sequence (char **steps, int *max_step_size, char move) {
	if (strlen(*steps) >= *max_step_size){
/*		printf("\n\trealloco\n");*/
		*max_step_size += 5;
	}

	char *sequence = (char*) calloc(*max_step_size, sizeof(char));
	strncat(sequence, *steps, strlen(*steps));
	strncat(sequence, &move, 1);
	strcat(sequence, "\0");
/*	printf("\nmax_step_size: %d\n", *max_step_size);*/
/*	printf("old_sequence_len: %ld\n", strlen(*steps));*/
/*	printf("sequence: %s\n", sequence);*/
/*	printf("sequence_len: %ld\n", strlen(sequence));*/

	free(*steps);
	return sequence;
}

/*controlla il contenuto della prossima cella che si vuole visitare*/
void check_next_step(map* map_info, char map[map_info->row][map_info->column], char next_step, bool *win, int *points, vector** tail, int next_row, int next_column) {
	if (next_step == BONUS_POINTS) {
		*points += QUANTITY_BONUS;
		vector_append(tail, map_info);
	}
	else{
		if (next_step == EXIT) {
			*win = true;
		}

		else if (next_step == HALF_POINTS && map_info->tail_len > 0) {
			//calcolo la metà della coda
			int middle_node = map_info->tail_len/2;
			// trovo il nodo che sta in mezzo alla coda
			if(map_info->tail_len % 2 != 0) {
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
			reset_nodes_till(tail, next_row, next_column, points, map_info, map);
		}
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
	}
	printf("coda: ");
	print_vector(*tail);
	printf("\nlen coda %d\n", map_info->tail_len);
}


/*Si occupa di eseguire una mossa che gli viene passata come parametro, assicurandosi che sia una mossa legale. Inoltre modifica il punteggio, la sequenza di mosse e aggiorno la mappa con le relative informazioni
 * parametri: le info della mappa, la mappa, la mossa da fare, i punti, la stringa contenente i passi fatti fino ad ora, la dimensione della stringa dei passi, un flag che cambia il ritorno in caso di chiamata del metodo dalla modalità ia oppure dalla modalità interattiva, la coda
 * Se la modalità ai è attiva il ritorno significa che la mossa è stata fatto correttamente
 * altrimenti il ritorno indica se il giocatore ha vinto la partità o no*/
int run_move(map* map_info, char map[map_info->row][map_info->column], char move, int *points, char **steps, int *step_size, bool ai_flag, vector** tail) {
	bool win = false;
	char next_step; //conterrà il contenuto della cella del passo successivo
	switch (move) {
		case NORD:
			if(map_info->player_row - 1 >= 0){
				next_step = map[map_info->player_row - 1][map_info->player_column]; // ottengo il carattere contenuto nel nuovo passo da fare
				if(next_step != WALL || map_info->drill_counter > 0) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row - 1, map_info->player_column);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row -= 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;

					*steps = build_sequence(steps, step_size, NORD);
					if(ai_flag) {
						return 1;
					}
				}
			}
			if(ai_flag) {
				return 0;
			}
			break;
		case EST:
			if(map_info->player_column + 1 < map_info->column){
				next_step = map[map_info->player_row][map_info->player_column + 1];
				if(next_step != WALL || map_info->drill_counter > 0) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row, map_info->player_column + 1);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_column += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
					*steps = build_sequence(steps, step_size, EST);
					if(ai_flag) {
						return 1;
					}
				}
			}
			if(ai_flag) {
				return 0;
			}
			break;
		case SUD:
			if(map_info->player_row + 1 < map_info->row){
				next_step = map[map_info->player_row + 1][map_info->player_column];
				if(next_step != WALL || map_info->drill_counter > 0) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row + 1, map_info->player_column);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER, &tail;
					*steps = build_sequence(steps, step_size, SUD);
					if(ai_flag) {
						return 1;
					}
				}
			}
			if(ai_flag) {
				return 0;
			}
			break;
		case OVEST:
			if(map_info->player_column - 1 >= 0){
				next_step = map[map_info->player_row][map_info->player_column - 1];
				if(next_step != WALL || map_info->drill_counter > 0) {
					check_next_step(map_info, map, next_step, &win, points, tail, map_info->player_row, map_info->player_column - 1);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_column -= 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
					*steps = build_sequence(steps, step_size, OVEST);
					if(ai_flag) {
						return 1;
					}
				}
			}
			if(ai_flag) {
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

void title() {
	printf(" -------------------\n");
	printf("|  SNAKE LABYRINTH  |\n");
	printf(" -------------------\n");
}

void main_menu() {
	printf("Scegli modalita' di gioco:\n");
	printf("1: Modalita' interattiva\n");
	printf("2: Modalita' IA\n");
	printf("3: Info sul gioco\n");
	printf("4: Esci\n");
	printf("Premi il numero corrispondente: ");
}

void input_type_menu() {
	printf("Scegli tipo di input della mappa:\n");
	printf("1: Scegli mappe del gioco\n");
	printf("2: Inserisci mappa da linea di comando\n");
	printf("3: Indietro\n");
	printf("Premi il numero corrispondente: ");
}


void print_game_info() {
	title();
	printf("Snake labyrinth è un gioco che presenta due modalità:\n");
	printf("\tModalità interattiva: si tratta di far uscire il proprio personaggio dal labirinto muovendonsi in quattro direzioni.\n"
			"\tLe direzioni sono associate ai seguenti tasti:\n"
				"\t\t- 'N'/'n' nord (movimento verso l'alto)\n"
				"\t\t- 'E'/'e' est (movimento verso destra)\n"
				"\t\t- 'S'/'s' sud (movimento verso il basso)\n"
				"\t\t- 'O'/'o' ovest (movimento verso sinistra)\n");
	printf("\tIl personaggio parte da una base di 1000 punti. Una volta usciti dal labirinto si ottiene un punteggio che si basa su quanti movimenti sono stati fatti e in base a dei modificatori che si trovano nella mappa:\n"
			"\tI modificatori sono i seguenti:\n"
				"\t\t- '$' aggiunge 10 punti al punteggio\n"
				"\t\t- '!' dimezza il proprio punteggio\n"
				"\t\t- ogni movimento toglie un punto\n");
	printf("\tAltri simboli che sono presenti nel gioco sono:\n"
				"\t\t- 'o' il proprio personaggio\n"
				"\t\t- '#' parete (non si può passare attraverso)\n"
				"\t\t- '_' uscita della mappa\n\n");
	printf("La seconda modalità: todo\n");
	printf("è possibili aggiungere le proprie mappe aggiungendole al file di testo: todo\n\n");
}

void print_map(map* map_info, char map[map_info->row][map_info->column]) {
	for (int row = 0; row < map_info->row; row++) {
		printf("\n");
		for (int column = 0; column < map_info->column; column++) {
			printf("%c", map[row][column]);
		}
	}
	clear_map_tail(map_info, map);
}
