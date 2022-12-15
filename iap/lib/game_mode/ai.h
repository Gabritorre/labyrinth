
/*Questo metodo crea un fantasma del personaggio e lo manda in una direzione "move" fino a che non trova una strada libera che rispetti "vert_value" e "horiz_value":
 * vert_value = 1 per controllare il sud, vert_value = -1 per controllare il nord,
 * horiz_value = 1 per controllare est, horiz_value = -1 per controllare a ovest
 * il metodo ritorna quante volte si è spostato in direzione "move" prima di trovare una strada libera in direzione vert_value e horiz_value
 * Questo metodo serve in sostanza per raggirare i muri
 * ES. se voglio controllare est -> move = NORD/SUD, vert_value = 0, horiz_value = 1
 * se voglio controllare sud -> move = EST/OVEST, vert_value = 1, horiz_value = 0
 * ovest -> move = NORD/SUD, vert_value = 0, horiz_value = -1
 * nord -> move = EST/OVEST, vert_value = -1, horiz_value = 0*/
int run_ghost(struct Map map_info, char map[map_info.row][map_info.column], char move, int vert_value, int horiz_value) {
	int next_row = map_info.player_row + vert_value; // calcola la riga su qui deve guardare
	int next_column = map_info.player_column + horiz_value; // calcola la colonna su cui deve guardare
	if(map[next_row][next_column] != WALL && map[next_row][next_column] != HALF_POINTS) { // se nel punto da guardare è libero allora fermati
		return 0;
	}
	//se vicolo ceco
	if (next_row == 0 || next_row == map_info.row || next_column == 0 || next_column == map_info.column) {
		return -9999;
	}

/*	continua a muoverti finchè trovi degli ostacoli dove stai guardando*/
	if (move == NORD)
		return run_ghost(map_info, map, move, vert_value - 1, horiz_value) + 1;

	if (move == EST)
		return run_ghost(map_info, map, move, vert_value, horiz_value + 1) + 1;

	if (move == SUD)
		return run_ghost(map_info, map, move, vert_value + 1, horiz_value) + 1;

	if (move == OVEST)
		return run_ghost(map_info, map, move, vert_value, horiz_value - 1) + 1;
}

/*controlla che ci sia una via libera retta tra il player e il target da raggiungere
 * Il metodo parte dall'uscita e va verso il player, se trova degli ostacoli ritorna false, se invece
 * riesce a raggiungere il player allora ritorna true*/
bool green_light_to_target (struct Map* map_info, char map[map_info->row][map_info->column], int target_col, int target_row, char direction) {
	// se player e target non sono allineati in nessun modo
	if(map_info->player_row != target_row && map_info->player_column != target_col) {
		return false;
	}
	if (map[target_row][target_col] == PLAYER) {
		return true;
	}
	else if (map[target_row][target_col] == WALL || map[target_row][target_col] == HALF_POINTS) {
		return false;
	}

	if (direction == 'h') {
		if (target_col > map_info->player_column) {
			return green_light_to_target(map_info, map, target_col - 1, target_row, direction);
		}
		else {
			return green_light_to_target(map_info, map, target_col + 1, target_row, direction);
		}
	}
	else if (direction == 'v') {
		if (target_row > map_info->player_row) {
			return green_light_to_target(map_info, map, target_col, target_row - 1, direction);
		}
		else {
			return green_light_to_target(map_info, map, target_col, target_row + 1, direction);
		}
	}

}

/*metodo invocato quendo si incontra un muro che blocca la strada
 * in base alla direzione in cui si è incappati nel muro questo metodo lancia i fantasmi (vedi metodo run_ghost) e decide quale strada conviene prendere
 * parametri:
 * direction = direzione in cui visogna muoversi per raggirare il muro (v = il player deve muoversi o a NORD o a SUD)
 * move = rappresenta il movimento da fare una volta trovata il precorso da prendere per raggirare il muro
 * */
void go_around_wall(struct Map* map_info, char map[map_info->row][map_info->column], char direction, char move, char** all_steps, int* all_steps_size, int* points, vector** tail, int target_row, int target_col) {
	int ghost1_steps, ghost2_steps, better_path;

	printf("lancio i fantasmi\n");
	int vert_value, horiz_value; // rappresentano la direzione in cui guardare, vedere commento sulla funzione run_ghost
	if (direction == 'v') {
		vert_value = 0;
		horiz_value = 1;
		if(move == OVEST){
			vert_value = 0;
			horiz_value = -1;
		}

		ghost1_steps = run_ghost(*map_info, map, NORD, vert_value, horiz_value);
		ghost2_steps = run_ghost(*map_info, map, SUD, vert_value, horiz_value);
		move = NORD;
		better_path = ghost1_steps;
		if(ghost1_steps > ghost2_steps || target_row > map_info->player_row) {
			better_path = ghost2_steps;
			move = SUD;
		}

	}
	else if (direction == 'h') {
		vert_value = 1;
		horiz_value = 0;
		if(move == NORD){
			vert_value = -1;
			horiz_value = 0;
		}
		ghost1_steps = run_ghost(*map_info, map, OVEST, vert_value, horiz_value);
		ghost2_steps = run_ghost(*map_info, map, EST, vert_value, horiz_value);
		move = OVEST;
		better_path = ghost1_steps;
		if(ghost1_steps > ghost2_steps || target_col > map_info->player_column) {
			better_path = ghost2_steps;
			move = EST;
		}

	}

	//eseguo il percorso deciso per raggirare il muro
	for(int i = 0; i < better_path; i++) {
		run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail);
		print_map(map_info, map);
		printf("\nsequenza: %s\n", *all_steps);
	}
}

/* Sposta la posizione del personaggio verso un determinato target che gli viene passato come parametro
 * parametri: le informazioni della mappa, la mappa, la stringa contenenti i passi fatti dal personaggio, la lunghezza di quest'utlima stringa, la colonna del target, la riga del target, il punteggio
 * */
void goto_target(struct Map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int target_col, int target_row, int *points, vector** tail) {

	char move; //contine la mossa da fare
	char target_direction = 'h'; // serve a capire da quale direzione (verticale od orizzontale) si può accedere all'uscita
	char left_target = '?', right_target = '?';

	if (target_col - 1 >= 0) {
		left_target = map[target_row][target_col - 1];
	}
	if (target_col + 1 < map_info->column) {
		right_target = map[target_row][target_col + 1];
	}

	if (left_target == WALL && right_target == WALL) {
		target_direction = 'v';
	}

	bool free_vertical = green_light_to_target(map_info, map, target_col, target_row, 'v');
	bool free_horizontal = green_light_to_target(map_info, map, target_col, target_row, 'h');
	if (map[target_row][target_col] == BONUS_POINTS && (free_vertical || free_horizontal)) {
		if(target_col == map_info->player_column){
			target_direction = 'v';
		}
		else if (target_row == map_info->player_row) {
			target_direction = 'h';
		}
	}
/*	printf("target_direction %c\n", target_direction);*/

	//finche il personaggio non trova percorso retto senza ostacoli verso il target
	while (!green_light_to_target(map_info, map, target_col, target_row, target_direction)) {
/*		printf("target_col %d\n", target_col);*/
/*		printf("target_row %d\n", target_row);*/
		if (target_direction == 'h') { // vogliamo far corrispondere la riga del giocatore con quella dell'uscita
			if (target_row == map_info->player_row){
				if (target_col > map_info->player_column) {
					move = EST;
				}
				else if (target_col < map_info->player_column) {
					move = OVEST;
				}
				if(!run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail)) {
					go_around_wall(map_info, map, 'v', move, all_steps, all_steps_size, points, tail, target_row, target_col);
					//una volta raggirato il muro esegue la mossa che mi avvicina al target
					run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail);
/*					print_map(map_info, map);*/
/*					printf("\nsequenza: %s\n", *all_steps);*/
				}
				continue;
			}
			else if (target_row > map_info->player_row) {
				//mi sposto giù
				move = SUD;
			}
			else if (target_row < map_info->player_row) {
				//mi sposto sù
				move = NORD;
			}
			if(!run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail)) {
				go_around_wall(map_info, map, 'h', move, all_steps, all_steps_size, points, tail, target_row, target_col);
				run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail);
			}
		}
		else if (target_direction == 'v') { // vogliamo far corrispondere la colonna del giocatore con quella dell'uscita
			if (target_col == map_info->player_column){
				if (target_row > map_info->player_row) {
					move = SUD;
				}
				else if (target_row < map_info->player_row) {
					move = NORD;
				}
				if(!run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail)) {
					go_around_wall(map_info, map, 'h', move, all_steps, all_steps_size, points, tail, target_row, target_col);
					//una volta raggirato il muro esegue la mossa che mi avvicina al target
					run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail);
/*					print_map(map_info, map);*/
/*					printf("\nsequenza: %s\n", *all_steps);*/
				}
				continue;
			}
			else if (target_col > map_info->player_column) {
				//mi sposto a destra
				move = EST;
			}
			else if (target_col < map_info->player_column) {
				//mi sposto a sinistra
				move = OVEST;
			}
			if(!run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail)) {
				go_around_wall(map_info, map, 'v', move, all_steps, all_steps_size, points, tail, target_row, target_col);
				run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail);
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
		run_move(map_info, map, move, points, all_steps, all_steps_size, true, tail);
		print_map(map_info, map);
		printf("\nsequenza: %s\n", *all_steps);
	}
}

 /* controlla se nel percorso per raggiungere l'uscita si trovano delle monete abbastanza vicine per essere raggiunte, una volta finite le monete raggiungibili punta all'uscita*/
void coin_exit_algorithm(struct Map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int *points, vector** tail) {

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

	//ricerca delle monete facendo il seguente percorso: dall'alto al basso -> cambio colonna -> dal basso all'alto
	//-> cambio colonna -> dall'alto al basso ecc...
	int start = 0, end = map_info->row - 1;
	while(column != map_info->exit_column || deep_inspect) {
		int row = start;
		while(row != end) {
			if(map[row][column] == BONUS_POINTS) {
				printf("MONETA IN %d %d", row, column);
				goto_target(map_info, map, all_steps, all_steps_size, column, row, points, tail);
			}
			if(row < end){
				row++;
			}
			else {
				row --;
			}
		}
		if (column < map_info->exit_column || deep_inspect) {
			column += 1;
		}
		else {
			column -= 1;
		}
		if (deep_inspect) {
			//if (column == further_column || map[map_info->player_row][column] == WALL) {
			if (column == further_column) {
				deep_inspect = false;
			}
		}
		// scambio start con end
		int temp = end;
		end = start;
		start = temp;
	}

	//raggiungimento dell'uscita
	goto_target(map_info, map, all_steps, all_steps_size, map_info->exit_column, map_info->exit_row, points, tail);
}

