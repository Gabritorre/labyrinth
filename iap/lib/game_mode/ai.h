void horizontal_move(struct Map* map_info, char *move, int *points, char map[map_info->row][map_info->column]) {
	if (map_info->exit_column > map_info->player_column) {
		*move = 'E';
		run_move(map_info, map, move, points, true);
	}
	else {
		*move = 'O';
		run_move(map_info, map, move, points, true);
	}
}

void vertical_move(struct Map* map_info, char *move, int *points, char map[map_info->row][map_info->column]) {
	if (map_info->exit_row > map_info->player_row) {
		*move = 'S';
		run_move(map_info, map, move, points, true);
	}
	else {
		*move = 'N';
		run_move(map_info, map, move, points, true);
	}
}


/*Questo metodo funziona quando non sono presenti muri interni nella mappa.
 * Quello che fa è allineare la posizione del personaggio con l'uscita per poi procedere dritto verso l'uscita*/
int no_wall_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char* return_type) {

	int points = 1000;
	char move;
	char exit_direction = 'h'; // serve a capire da quale direzione (verticale od orizzontale) si può accedere all'uscita
	if (map[map_info->exit_row][map_info->exit_column - 1] == WALL || map[map_info->exit_row][map_info->exit_column + 1] == WALL) {
		exit_direction = 'v';
	}
	printf("%c\n", exit_direction);
	while (map_info->player_row != map_info->exit_row || map_info->player_column != map_info->exit_column) {
		if (exit_direction == 'h') { // vogliamo far corrispondere la riga del giocatore con quella dell'uscita
			if (map_info->exit_row > map_info->player_row) {
				//mi sposto giù
				move = 'S';
				if(!run_move(map_info, map, &move, &points, true)) {
					//se non è possibile controllo se l'uscita è più a destra o più a sinistra e mi sposto più vicino con la colonna
					horizontal_move(map_info, &move, &points, map);
				}
			}
			else {
				//mi sposto sù
				move = 'N';
				if(!run_move(map_info, map, &move, &points, true)) {
					//se non è possibile controllo se l'uscita è più a destra o più a sinistra e mi sposto più vicino con la colonna
					horizontal_move(map_info, &move, &points, map);
				}
			}
		}
		else if (exit_direction == 'v') { // vogliamo far corrispondere la colonna del giocatore con quella dell'uscita
			if (map_info->exit_column > map_info->player_column) {
				//mi sposto a destra
				move = 'E';
				if(!run_move(map_info, map, &move, &points, true)) {
					//se non è possibile controllo se l'uscita è più in alto o più in basso e mi sposto più vicino con la riga
					vertical_move(map_info, &move, &points, map);
				}
			}
			else {
				//mi sposto a sinistra
				move = 'O';
				if(!run_move(map_info, map, &move, &points, true)) {
					//se non è possibile controllo se l'uscita è più in alto o più in basso e mi sposto più vicino con la riga
					vertical_move(map_info, &move, &points, map);
				}
			}
		}
	printf("player [%d %d]\n", map_info->player_row, map_info->player_column);
	}
	printf("allineati\n player [%d %d] exit [%d %d]\n", map_info->player_row, map_info->player_column, map_info->exit_row, map_info->exit_column);

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
/*				map_info->player_column += 1;*/
				move = 'E';
				run_move(map_info, map, &move, &points, true);
			}
			else {
/*				map_info->player_column -= 1;*/
				move = 'O';
				run_move(map_info, map, &move, &points, true);
			}
		}
		else if (exit_direction == 'v') {
			if (map_info->exit_row > map_info->player_row) {
/*				map_info->player_row += 1;*/
				move = 'S';
				run_move(map_info, map, &move, &points, true);
			}
			else {
/*				map_info->player_row -= 1;*/
				move = 'N';
				run_move(map_info, map, &move, &points, true);
			}
		}
	}
	return points;
}


/*Questo metodo funziona quando non sono presenti muri interni nella mappa.
 * Quello che fa è allineare la posizione del personaggio con le eventuali monete per poi allinearsi con l'uscita procedendo infine dritto verso l'uscita*/
int no_wall_coin_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char* return_type) {
	return 0;
}

int random_algorithm() {
	return 0;
}

