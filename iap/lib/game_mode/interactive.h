
/* 1 = uscito dalla partita
 * 2 = comando inserito non valido
 * 0 = corretto*/
int command_interpreter (char command, char *move) {
	printf("comando: %d\n", command);
	if (command == QUIT) {
		return 1;
	}
	else {
		if (toupper(command) != NORD && toupper(command) != SUD && toupper(command) != EST && toupper(command) != OVEST) {
			printf("\nerrore dato da: %d %c\n", command, command);
			return 2;
		}
		*move = toupper(command);
	}
	return 0;
}

/*appende la mossa appena fatta alla sequenza di mosse
 * parametri: la sequenza di mosse fatte, la dimensione della stringa, la mossa da appendere*/
char* build_sequence (char **steps, int *max_step_size, char move) {
	if (strlen(*steps) >= *max_step_size){
/*		printf("\n\trealloco\n");*/
		*max_step_size += 5;
	}

	char *sequence = (char*)malloc(sizeof(char) * *max_step_size);
	strncat(sequence, *steps, strlen(*steps));
	strncat(sequence, &move, 1);
	strcat(sequence, "\0");
/*	printf("\nmax_step_size: %d\n", *max_step_size);*/
/*	printf("old_sequence_len: %ld\n", strlen(*steps));*/
/*	printf("sequence: %s\n", sequence);*/
/*	printf("sequence_len: %ld\n", strlen(sequence));*/

	return sequence;
}

/*inserisce la coda nella mappa*/
void insert_tail_in_map(struct Map* map_info, char map[map_info->row][map_info->column], vector* tail) {
	if (tail == NULL) // fine lista, cioè lista vuota
		return;
	if(tail->row != -1)
		map[tail->row][tail->column] = TAIL;

	insert_tail_in_map(map_info, map, tail->next);
}

void move_tail(struct Map* map_info, vector** tail) {
	if ((*tail) == NULL){
		return;
	}
	if (((*tail)->next) == NULL) {
		(*tail)->row = map_info->player_row;
		(*tail)->column = map_info->player_column;
		return;
	}
	if((*tail)->row != -1){
		(*tail)->row = ((*tail)->next)->row;
		(*tail)->column = ((*tail)->next)->column;
	}
	move_tail(map_info, &((*tail)->next));
}

/*controlla il contenuto della prossima cella che si vuole visitare*/
void check_next_step(struct Map* map_info, char map[map_info->row][map_info->column], char next_step, bool *win, int *points, vector** tail, int next_row, int next_column) {
	if (next_step == BONUS_POINTS) {
		*points += QUANTITY_BONUS;
		vector_append(tail, map_info);
	}
	else{
		if (next_step == EXIT) {
			*win = true;
		}

		else if (next_step == HALF_POINTS) {
/*			if (*points < 0) {*/
/*				*points = *points * 2;*/
/*			}*/
/*			else {*/
/*				*points = (int) *points/2;*/
/*			}*/
			int middle_node = map_info->tail_len/2;
			if(map_info->tail_len % 2 != 0) {
				middle_node = (map_info->tail_len + 1) / 2;
			}
			delete_half_tail(tail, middle_node, points, map_info);
		}

		else if (next_step == DRILL) {
			map_info->drill_counter += 3;
		}

		if (next_step == WALL && map_info->drill_counter > 0) {
			map_info->drill_counter -= 1;
		}
		move_tail(map_info, tail);
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
		if (map[next_row][next_column] == TAIL) {
			reset_nodes_till(tail, next_row, next_column, points, map_info);
		}
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
	}
	printf("coda: ");
	print_vector(*tail);
	printf("\nlen coda %d\n", map_info->tail_len);
}

/*Si occupa di eseguire una mossa che gli viene passata come parametro, assicurandosi che sia una mossa legale. Inoltre modifica il punteggio, la sequenza di mosse e aggiorno la mappa con le relative invormazioni
 * parametri: le info della mappa, la mappa, la mossa da fare, i punti, la stringa contenente i passi fatti fino ad ora, la dimensione della stringa dei passi, un flag che cambia il ritorno in caso di chiamata del metodo dalla modalità ia oppure dalla modalità interattiva
 * Se la modalità ai è attiva il ritorno significa che la mossa è stata fatto correttamente
 * altrimenti il ritorno indica se il giocatore ha vinto la partità o no*/
int run_move(struct Map* map_info, char map[map_info->row][map_info->column], char move, int *points, char **steps, int *step_size, bool ai_flag, vector** tail) {
	bool win = false;
	char next_step; //conterrà il contenuto della cella del passo successivo
	switch (move) {
		case NORD:
			if(map_info->player_row - 1 >= 0){
				next_step = map[map_info->player_row - 1][map_info->player_column];
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

void start_interactive_mode(struct Map* map_info, char map[map_info->row][map_info->column]) {
	bool playing = true;
	int points = 1000;
	vector *tail = NULL;
	map_info->tail_len = 0;
	map_info->drill_counter = 0;
	int max_steps_size = map_info->row + map_info->column;
	char *all_steps = (char*)malloc(sizeof(char) * max_steps_size);

	while(playing) {
		printf("\n\n---------------\n\n");
		insert_tail_in_map(map_info, map, tail);
		print_map(map_info, map);
		printf("\nPUNTEGGIO: %d\n", points);
		printf("TRAPANI: %d\n", map_info->drill_counter);
		char move;
		char command;
		printf("Inserisci mossa: ");
		scanf(" %c", &command);
		int ci_result = command_interpreter(command, &move);
		if(ci_result == 1) {
			printf("Uscito dalla partita\n");
			playing = false;
		}
		else if (ci_result == 2) {
			printf("La sequenza inserita contiene valori non corretti, riprova");
		}
		else{
			if(run_move(map_info, map, move, &points, &all_steps, &max_steps_size, false, &tail)) {
				insert_tail_in_map(map_info, map, tail);
				print_map(map_info, map);
				printf("\n\tHAI RAGGIUNTO L'USCITA!\n\tHai fatto %d punti", points);
				playing = false;
			}
		}
	}
	dealloc_vector(&tail);
	free(all_steps);
}
