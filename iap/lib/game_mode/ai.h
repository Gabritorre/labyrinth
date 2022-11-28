
//controlla se l'uscita è più a destra o più a sinistra rispetto al personaggio e si sposta più vicino con la colonna
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

//controlla se l'uscita è più in alto o più in basso rispetto al personaggio e si sposta più vicino con la riga
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

/* Sposta la posizione del personaggio con un determinato target che gli viene passato come parametro
 * parametri: le informazioni della mappa, la mappa, la stringa contenenti i passi fatti dal personaggio, la lunghezza di quest'utlima stringa, la colonna del target, la riga del target, il punteggio
 * */
void goto_target(struct Map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int target_col, int target_row, int *points) {

	char move; //contine la mossa da fare
	char target_direction = 'h'; // serve a capire da quale direzione (verticale od orizzontale) si può accedere all'uscita
	char left_target = '?', right_target = '?';
	if (target_col - 1 >= 0) {
		left_target = map[target_row][target_col - 1];
	}
	if (target_col + 1 < map_info->column) {
		right_target = map[map_info->exit_row][target_col + 1];
	}

	if (left_target == WALL && right_target == WALL) {
		target_direction = 'v';
	}

	if (map[target_row][target_col] == BONUS_POINTS) {
		if(target_col == map_info->player_column){
			target_direction = 'v';
		}
		else if (target_row == map_info->player_row) {
			target_direction = 'h';
		}
	}
/*	printf("target_direction %c\n", target_direction);*/
	int counter = 0; //contatore che mi obbliga a fare almeno una mossa (per far uscire il personaggio)
	//finche il personaggio non si allinea o per riga o per colonna al target
	while (map_info->player_row != target_row && map_info->player_column != target_col || counter < 1) {
		counter++;
		if (target_direction == 'h') { // vogliamo far corrispondere la riga del giocatore con quella dell'uscita
			if (target_row > map_info->player_row) {
				//mi sposto giù
				move = SUD;
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					horizontal_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
			else {
				if (target_row == map_info->player_row){
					continue;
				}
				//mi sposto sù
				move = NORD;
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					horizontal_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
		}
		else if (target_direction == 'v') { // vogliamo far corrispondere la colonna del giocatore con quella dell'uscita
			if (target_col > map_info->player_column) {
				//mi sposto a destra
				move = EST;
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					vertical_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
			else {
				if (target_col == map_info->player_column){
					continue;
				}
				//mi sposto a sinistra
				move = OVEST;
				if(!run_move(map_info, map, &move, points, all_steps, all_steps_size, true)) {
					vertical_move(map_info, &move, points, all_steps, all_steps_size, map, target_col, target_row);
				}
			}
		}
		print_map(map_info, map);
		printf("\nsequenza: %s\n", *all_steps);
	}

	// dopo essersi allineati con il target è possibile andare dritto verso di esso
	int steps_to_target = 0; // calcolo quanti passi mancano per raggiungere il target
	if (target_direction == 'h') {
		if(target_col > map_info->player_column) {
			steps_to_target = target_col - map_info->player_column;
		}
		else{
			steps_to_target = map_info->player_column - target_col;
		}
	}
	else if (target_direction == 'v') {
		if (target_row > map_info->player_row) {
			steps_to_target = target_row - map_info->player_row;
		}
		else {
			steps_to_target = map_info->player_row - target_row;
		}
	}
	//esegue i passi mancanti
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
 * controlla se nel percorso per raggiungere l'uscita si trovano delle monete abbastanza vicine per essere raggiunte, una volta finite le monete raggiungibili punta all'uscita*/
void no_wall_coin_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int *points) {

	int column = map_info->player_column;
	int further_column = map_info->column - 1; // colonna più lontana da scannerizzare per cercare delle monete
	bool deep_inspect = false; //nel caso in cui il personaggio e l'uscita siano allineati sin dalla partenza allora cerca delle monete nelle colonne adiacenti a qualla per raggiungere l'uscita

	if (map_info->player_column < map_info->exit_column) {
		column = map_info->player_column - 9;
	}
	else if (map_info->player_column > map_info->exit_column) {
		column = map_info->player_column + 9;
	}
	else{
		column = map_info->player_column - 9;
		if(map_info->player_column + 9 < map_info->column - 1){
			further_column = map_info->player_column + 9;
		}
		deep_inspect = true;
	}

	//evito di uscire dai limiti della mappa
	if(column <= 0) {
		column = 1;
	}
	else if (column >= map_info->column) {
		column = map_info->column - 1;
	}

	//ricerca delle monete
	while(column != map_info->exit_column || deep_inspect) {
		for(int row = 1; row < map_info->row - 1; row++) {
			if(map[row][column] == BONUS_POINTS) {
/*				printf("MONETA IN %d %d", row, column);*/
				goto_target(map_info, map, all_steps, all_steps_size, column, row, points);
			}
		}
		if (column < map_info->exit_column || deep_inspect) {
			column += 1;
		}
		else {
			column -= 1;
		}
		if (deep_inspect) {
			if (column == further_column || map[map_info->player_row][column] == WALL) {
				deep_inspect = false;
			}
		}
	}

	//raggiungimento dell'uscita
	goto_target(map_info, map, all_steps, all_steps_size, map_info->exit_column, map_info->exit_row, points);
}

