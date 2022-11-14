
/* 1 = uscito dalla partita
 * 2 = comando inserito non valido
 * 0 = corretto*/
int command_interpreter (char command, char *moves) {
	printf("comando: %d\n", command);
	if (command == QUIT) {
		return 1;
	}
	else {
		if (toupper(command) != NORD && toupper(command) != SUD && toupper(command) != EST && toupper(command) != OVEST) {
			printf("\nerrore dato da: %d %c\n", command, command);
			return 2;
		}
		*moves = toupper(command);
	}
	return 0;
}

/*se il flag ai è attivo ritorna 1 se la mossa è stata eseguita altrimenti ritorna 0*/
/*se il flag ai è disattivo ritorna 1 se il giocatore ha raggiunto l'uscita altrimenti ritorna 0*/
int run_move(struct Map* map_info, char map[map_info->row][map_info->column], char *moves, int *points, bool ai_flag) {
	bool win = false;
	switch (*moves) {
		case NORD:
			if(map_info->player_row - 1 >= 0){
				if(map[map_info->player_row - 1][map_info->player_column] != WALL) {
					if (map[map_info->player_row - 1][map_info->player_column] == EXIT) {
						win = true;
					}
					if (map[map_info->player_row - 1][map_info->player_column] == BONUS_POINTS) {
						*points += QUANTITY_BONUS;
					}
					if (map[map_info->player_row - 1][map_info->player_column] == HALF_POINTS) {
						if (*points < 0) {
							*points = *points * 2;
						}
						else {
							*points = (int) *points/2;
						}
					}
					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row -= 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
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
				if(map[map_info->player_row][map_info->player_column + 1] != WALL) {
					if (map[map_info->player_row][map_info->player_column + 1] == EXIT) {
						win = true;
					}
					if (map[map_info->player_row][map_info->player_column + 1] == BONUS_POINTS) {
						*points += QUANTITY_BONUS;
					}
					if (map[map_info->player_row][map_info->player_column + 1] == HALF_POINTS) {
						if (*points < 0) {
							*points = *points * 2;
						}
						else {
							*points = (int) *points/2;
						}
					}
					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_column += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
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
				if(map[map_info->player_row + 1][map_info->player_column] != WALL) {
					if (map[map_info->player_row + 1][map_info->player_column] == EXIT) {
						win = true;
					}
					if (map[map_info->player_row + 1][map_info->player_column] == BONUS_POINTS) {
						*points += QUANTITY_BONUS;
					}
					if (map[map_info->player_row + 1][map_info->player_column] == HALF_POINTS) {
						if (*points < 0) {
							*points = *points * 2;
						}
						else {
							*points = (int) *points/2;
						}
					}
					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_row += 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
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
				if(map[map_info->player_row][map_info->player_column - 1] != WALL) {
					if (map[map_info->player_row][map_info->player_column - 1] == EXIT) {
						win = true;
					}
					if (map[map_info->player_row][map_info->player_column - 1] == BONUS_POINTS) {
						*points += QUANTITY_BONUS;
					}
					if (map[map_info->player_row][map_info->player_column - 1] == HALF_POINTS) {
						if (*points < 0) {
							*points = *points * 2;
						}
						else {
							*points = (int) *points/2;
						}
					}
					*points -= 1;
					map[map_info->player_row][map_info->player_column] = STEP;
					map_info->player_column -= 1;
					map[map_info->player_row][map_info->player_column] = PLAYER;
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
	while(playing) {
		printf("\n\n---------------\n\n");
		print_map(map_info, map);
		printf("\nPUNTEGGIO: %d\n", points);
		char *move = (char*) calloc(1, sizeof(char));
		char command;
		printf("Inserisci mossa: ");
		scanf(" %c", &command);
		int ci_result = command_interpreter(command, move);
		if(ci_result == 1) {
			printf("Uscito dalla partita\n");
			playing = false;
		}
		else if (ci_result == 2) {
			printf("La sequenza inserita contiene valori non corretti, riprova");
		}
		else{
			if(run_move(map_info, map, move, &points, false)) {
				print_map(map_info, map);
				printf("\n\tHAI RAGGIUNTO L'USCITA!\n\tHai fatto %d punti", points);
				playing = false;
			}
		}
		free(move);
	}
}
