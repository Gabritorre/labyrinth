void horizontal_move(struct Map* map_info, char *move, int *points, char *all_steps, char map[map_info->row][map_info->column]) {
	if (map_info->exit_column > map_info->player_column) {
		*move = EST;
	}
	else if (map_info->exit_column < map_info->player_column){
		*move = OVEST;
	}
	else {
		if(map_info->exit_column - 1 >= 0) {
			*move = OVEST;
		}
		else {
			*move = EST;
		}
	}
	run_move(map_info, map, move, points, all_steps, true);
}

void vertical_move(struct Map* map_info, char *move, int *points, char *all_steps, char map[map_info->row][map_info->column]) {
	if (map_info->exit_row > map_info->player_row) {
		*move = SUD;
	}
	else if (map_info->exit_row < map_info->player_row) {
		*move = NORD;
	}
	else {
		if(map_info->exit_row - 1 >= 0){
			*move = NORD;
		}
		else {
			*move = SUD;
		}
	}
	run_move(map_info, map, move, points, all_steps, true);
}

/*Questo metodo funziona quando non sono presenti muri interni nella mappa.
 * Quello che fa è allineare la posizione del personaggio con l'uscita per poi procedere dritto verso l'uscita*/
char* no_wall_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], int *points) {

	char *all_steps = (char *)malloc(sizeof(char) * (map_info->row + map_info->column));
	char move;
	char exit_direction = 'h'; // serve a capire da quale direzione (verticale od orizzontale) si può accedere all'uscita
	char left_exit = '?', right_exit = '?';
	if (map_info->exit_column - 1 >= 0) {
		left_exit = map[map_info->exit_row][map_info->exit_column - 1];
	}
	if (map_info->exit_column + 1 < map_info->column) {
		right_exit = map[map_info->exit_row][map_info->exit_column + 1];
	}

	if (left_exit == WALL || right_exit == WALL) {
		exit_direction = 'v';
		printf("sinistra:%c destra:%c", left_exit, right_exit);
	}
	int counter = 0;
	while (map_info->player_row != map_info->exit_row && map_info->player_column != map_info->exit_column || counter < 1) {
		counter++;
		if (exit_direction == 'h') { // vogliamo far corrispondere la riga del giocatore con quella dell'uscita
			if (map_info->exit_row > map_info->player_row) {
				//mi sposto giù
				move = 'S';
				if(!run_move(map_info, map, &move, points, all_steps, true)) {
					//se non è possibile controllo se l'uscita è più a destra o più a sinistra e mi sposto più vicino con la colonna
					horizontal_move(map_info, &move, points, all_steps, map);
				}
			}
			else {
				//mi sposto sù
				move = 'N';
				if(!run_move(map_info, map, &move, points, all_steps, true)) {
					//se non è possibile controllo se l'uscita è più a destra o più a sinistra e mi sposto più vicino con la colonna
					horizontal_move(map_info, &move, points, all_steps, map);
				}
			}
		}
		else if (exit_direction == 'v') { // vogliamo far corrispondere la colonna del giocatore con quella dell'uscita
			if (map_info->exit_column > map_info->player_column) {
				//mi sposto a destra
				move = 'E';
				if(!run_move(map_info, map, &move, points, all_steps,  true)) {
					//se non è possibile controllo se l'uscita è più in alto o più in basso e mi sposto più vicino con la riga
					vertical_move(map_info, &move, points, all_steps, map);
				}
			}
			else {
				//mi sposto a sinistra
				move = 'O';
				if(!run_move(map_info, map, &move, points, all_steps, true)) {
					//se non è possibile controllo se l'uscita è più in alto o più in basso e mi sposto più vicino con la riga
					vertical_move(map_info, &move, points, all_steps, map);
				}
			}
		}
		print_map(map_info, map);
	}

	int steps_to_exit = 0;
	if (exit_direction == 'h') {
		steps_to_exit = abs(map_info->exit_column - map_info->player_column);
	}
	else if (exit_direction == 'v') {
		steps_to_exit = abs(map_info->exit_row - map_info->player_row);
	}

	for (int step = 0; step < steps_to_exit; step++) {
		if (exit_direction == 'h') {
			if (map_info->exit_column > map_info->player_column) {
				move = 'E';
			}
			else {
				move = 'O';
			}
		}
		else if (exit_direction == 'v') {
			if (map_info->exit_row > map_info->player_row) {
				move = 'S';
			}
			else {
				move = 'N';
			}
		}
		run_move(map_info, map, &move, points, all_steps, true);
		print_map(map_info, map);
	}
	return all_steps;
}


/*Questo metodo funziona quando non sono presenti muri interni nella mappa.
 * Quello che fa è allineare la posizione del personaggio con le eventuali monete per poi allinearsi con l'uscita procedendo infine dritto verso l'uscita*/
int no_wall_coin_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char* return_type) {
	return 0;
}

int random_algorithm() {
	return 0;
}

