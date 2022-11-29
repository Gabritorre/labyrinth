
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
char* build_sequence (char **steps, int *step_size, char move) {
	if (strlen(*steps) == *step_size){
/*		printf("\n\trealloco\n");*/
		*step_size += 5;
	}

	char *sequence = (char*)malloc(sizeof(char) * *step_size);
	strncat(sequence, *steps, strlen(*steps));
	strncat(sequence, &move, 1);
	strcat(sequence, "\0");
/*	printf("\nstep_size: %d\n", *step_size);*/
/*	printf("str_len: %ld\n", strlen(*steps));*/

	return sequence;
}

/*controlla il contenuto della prossima cella che si vuole visitare*/
void check_next_step(struct Map* map_info, char next_step, bool *win, int *points) {
	if (next_step == EXIT) {
		*win = true;
	}
	else if (next_step == BONUS_POINTS) {
		*points += QUANTITY_BONUS;
	}
	else if (next_step == HALF_POINTS) {
		if (*points < 0) {
			*points = *points * 2;
		}
		else {
			*points = (int) *points/2;
		}
	}
	else if (next_step == DRILL) {
		map_info->drill_counter += 3;
	}

	if (next_step == WALL && map_info->drill_counter > 0) {
		map_info->drill_counter -= 1;
	}
}

/*Si occupa di eseguire una mossa che gli viene passata come parametro, assicurandosi che sia una mossa legale. Inoltre modifica il punteggio, la sequenza di mosse e aggiorno la mappa con le relative invormazioni
 * parametri: le info della mappa, la mappa, la mossa da fare, i punti, la stringa contenente i passi fatti fino ad ora, la dimensione della stringa dei passi, un flag che cambia il ritorno in caso di chiamata del metodo dalla modalità ia oppure dalla modalità interattiva
 * Se la modalità ai è attiva il ritorno significa che la mossa è stata fatto correttamente
 * altrimenti il ritorno indica se il giocatore ha vinto la partità o no*/
int run_move(struct Map* map_info, char map[map_info->row][map_info->column], char move, int *points, char **steps, int *step_size, bool ai_flag) {
	bool win = false;
	char next_step; //conterrà il contenuto della cella del passo successivo
	switch (move) {
		case NORD:
			if(map_info->player_row - 1 >= 0){
				next_step = map[map_info->player_row - 1][map_info->player_column];
				if(next_step != WALL || map_info->drill_counter > 0) {
					check_next_step(map_info, next_step, &win, points);

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
					check_next_step(map_info, next_step, &win, points);

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
					check_next_step(map_info, next_step, &win, points);

					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
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
					check_next_step(map_info, next_step, &win, points);

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
	map_info->drill_counter = 0;
	int all_steps_size = map_info->row + map_info->column;
	char *all_steps = (char*)malloc(sizeof(char) * all_steps_size);

	while(playing) {
		printf("\n\n---------------\n\n");
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
			if(run_move(map_info, map, move, &points, &all_steps, &all_steps_size, false)) {
				print_map(map_info, map);
				printf("\n\tHAI RAGGIUNTO L'USCITA!\n\tHai fatto %d punti", points);
				playing = false;
			}
		}
	}
}
