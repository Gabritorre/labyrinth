void horizontal_move(struct Map* map_info, char *move, int *points, char **all_steps, int *step_size, char map[map_info->row][map_info->column], int target_col, int target_row) {
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
	run_move(map_info, map, move, points, all_steps, step_size, true);
}

void vertical_move(struct Map* map_info, char *move, int *points, char **all_steps, int *step_size, char map[map_info->row][map_info->column], int target_col, int target_row) {
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
	run_move(map_info, map, move, points, all_steps, step_size, true);
}

/*Questo metodo funziona quando non sono presenti muri interni nella mappa.
 * Quello che fa è allineare la posizione del personaggio con l'uscita per poi procedere dritto verso l'uscita*/
void no_wall_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int target_col, int target_row, int *points) {

	char move;
	char target_direction = 'h'; // serve a capire da quale direzione (verticale od orizzontale) si può accedere all'uscita
	char left_target = '?', right_target = '?';
	if (target_col - 1 >= 0) {
		left_target = map[target_row][target_col - 1];
	}
	if (target_col + 1 < map_info->column) {
		right_target = map[map_info->exit_row][target_col + 1];
	}

	if (left_target == WALL || right_target == WALL) {
		target_direction = 'v';
	}
	int counter = 0;
	while (map_info->player_row != target_row && map_info->player_column != target_col || counter < 1) {
		counter++;
		if (target_direction == 'h') { // vogliamo far corrispondere la riga del giocatore con quella dell'uscita
			if (target_row > map_info->player_row) {
				//mi sposto giù
				move = 'S';
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					//se non è possibile controllo se l'uscita è più a destra o più a sinistra e mi sposto più vicino con la colonna
					horizontal_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
			else {
				if (target_row == map_info->player_row){
					continue;
				}
				//mi sposto sù
				move = 'N';
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					//se non è possibile controllo se l'uscita è più a destra o più a sinistra e mi sposto più vicino con la colonna
					horizontal_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
		}
		else if (target_direction == 'v') { // vogliamo far corrispondere la colonna del giocatore con quella dell'uscita
			if (target_col > map_info->player_column) {
				//mi sposto a destra
				move = 'E';
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					//se non è possibile controllo se l'uscita è più in alto o più in basso e mi sposto più vicino con la riga
					vertical_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
			else {
				if (target_col == map_info->player_column){
					continue;
				}
				//mi sposto a sinistra
				move = 'O';
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					//se non è possibile controllo se l'uscita è più in alto o più in basso e mi sposto più vicino con la riga
					vertical_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
		}
		print_map(map_info, map);
		printf("\nsequenza: %s\n", *all_steps);
	}

	int steps_to_target = 0;
	if (target_direction == 'h') {
		steps_to_target = abs(target_col - map_info->player_column);
	}
	else if (target_direction == 'v') {
		steps_to_target = abs(target_row - map_info->player_row);
	}

	for (int step = 0; step < steps_to_target; step++) {
		if (target_direction == 'h') {
			if (target_col > map_info->player_column) {
				move = EST;
			}
			else {
				move = OVEST;
			}
		}
		else if (target_direction == 'v') {
			if (target_row > map_info->player_row) {
				move = SUD;
			}
			else {
				move = NORD;
			}
		}
		run_move(map_info, map, &move, points, all_steps, all_steps_size, true);
		print_map(map_info, map);
		printf("\nsequenza: %s\n", *all_steps);
	}
}

/*Questo metodo funziona quando non sono presenti muri interni nella mappa.
 * Quello che fa è allineare la posizione del personaggio con le eventuali monete per poi allinearsi con l'uscita procedendo infine dritto verso l'uscita*/
void no_wall_coin_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int *points) {
	/*generelazzare il metodo no_wall_algorithm mettendo al posto dell'uscita un target generico
	 * in questo metodo viene scannerizzata la mappa dal player all'uscita e in caso si trovano delle moneti si chiama il metodo no_wall_algorithm con target la moneta*/

	int column = map_info->player_column;
	while(column != map_info->exit_column) {
		for(int row = 1; row < map_info->row - 1; row++) {
			if(map[row][column] == BONUS_POINTS) {
				no_wall_algorithm(map_info, map, all_steps, all_steps_size, column, row, points);
			}
		}
		if (column > map_info->exit_column) {
			column -= 1;
		}
		else {
			column += 1;
		}
	}

	no_wall_algorithm(map_info, map, all_steps, all_steps_size, map_info->exit_column, map_info->exit_row, points);
}

