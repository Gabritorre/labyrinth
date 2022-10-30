/* 1 = uscito dalla partita
 * 2 = comando inserito non valido
 * 0 = corretto*/
int command_interpreter (char *command, char *moves, int *moves_index) {
	for (int i = 0; i < 50; i++) {
		printf("%d", command[i]);
	}
	if (toupper(command[0]) == QUIT && toupper(command[1]) == QUIT && command[2] == 0) {
		return 1;
	}
	else {
		for (int i = 0; i < 50; i++) {
			if(command[i] == 0) {
				break;
			}
			if (toupper(command[i]) != NORD && toupper(command[i]) != SUD && toupper(command[i]) != EST && toupper(command[i]) != OVEST) {
				printf("\nerrore dato da: %d %c\n", command[i], command[i]);
				return 2;
			}
			*moves_index += 1;
			moves = realloc(moves, *moves_index);
			moves[i] = toupper(command[i]);
		}
	}
	return 0;
}

void run_moves(struct Map* map_info, char map[map_info->row][map_info->column], char *moves, int *moves_index) {
	printf("\nHello\n");
}

void start_interactive_mode(struct Map* map_info, char map[map_info->row][map_info->column]) {
	bool playing = true;
	int moves_index;

	while(playing) {
		printf("\n\n---------------\n\n");
		print_map(map_info, map);
		char *moves = (char*) calloc(0, sizeof(char));
		char command[50] = {0};
		moves_index = 0;
		printf("\ninserisci mossa: ");
		scanf(" %s", command);
		int ci_result = command_interpreter(command, moves, &moves_index);
		if(ci_result == 1) {
			printf("Uscito dalla partita\n");
			playing = false;
		}
		else if (ci_result == 2) {
			printf("La sequenza inserita contiene valori non corretti, riprova");
		}
		else{
/*			printf("numero di mosse: %d\n", moves_index);*/
/*			printf("MOSSE: ");*/
/*			for (int i = 0; i < moves_index; i++) {*/
/*				printf("%c ", moves[i]);*/
/*			}*/
			run_moves(map_info, map, moves, &moves_index);
		}
		free(moves);
	}
}
