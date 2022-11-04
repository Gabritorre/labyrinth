
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

int run_move(struct Map* map_info, char map[map_info->row][map_info->column], char *moves) {
	bool win = false;
	switch (*moves) {
		case NORD:
			if(map_info->player_row - 1 >= 0){
				if(map[(map_info->player_row - 1)][map_info->player_column] != '#') {
					if (map[map_info->player_row - 1][map_info->player_column] == '_') {
						win = true;
					}
					map[map_info->player_row][map_info->player_column] = ' ';
					map_info->player_row -= 1;
					map[map_info->player_row][map_info->player_column] = 'o';
				}
			}
			break;
		case EST:
			if(map_info->player_column + 1 < map_info->column){
				if(map[map_info->player_row][map_info->player_column + 1] != '#') {
					if (map[map_info->player_row][map_info->player_column + 1] == '_') {
						win = true;
					}
					map[map_info->player_row][map_info->player_column] = ' ';
					map_info->player_column += 1;
					map[map_info->player_row][map_info->player_column] = 'o';
				}
			}
			break;
		case SUD:
			if(map_info->player_row + 1 < map_info->row){
				if(map[map_info->player_row + 1][map_info->player_column] != '#') {
					if (map[map_info->player_row + 1][map_info->player_column] == '_') {
						win = true;
					}
					map[map_info->player_row][map_info->player_column] = ' ';
					map_info->player_row += 1;
					map[map_info->player_row][map_info->player_column] = 'o';
				}
			}
			break;
		case OVEST:
			if(map_info->player_column - 1 >= 0){
				if(map[map_info->player_row][map_info->player_column - 1] != '#') {
					if (map[map_info->player_row][map_info->player_column - 1] == '_') {
						win = true;
					}
					map[map_info->player_row][map_info->player_column] = ' ';
					map_info->player_column -= 1;
					map[map_info->player_row][map_info->player_column] = 'o';
				}
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
	int moves_index;

	while(playing) {
		printf("\n\n---------------\n\n");
		print_map(map_info, map);
		char *move = (char*) calloc(1, sizeof(char));
		char command;
		printf("\ninserisci mossa: ");
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
			if(run_move(map_info, map, move)) {
				printf("\n\tHAI RAGGIUNTO L'USCITA!\n");
				playing = false;
			}
		}
		free(move);
	}
}
