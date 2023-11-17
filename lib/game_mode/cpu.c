/**
* @file cpu.c
* @brief Contiene le funzioni utilizzate dalla modalità CPU
*/

/**
Questo metodo serve in sostanza per raggirare i muri.\n
Il metodo crea un fantasma del personaggio e lo manda in una direzione "move" fino a che non trova una strada libera che rispetti "vert_value" e "horiz_value":\n
 * vert_value = 1 per controllare il sud, vert_value = -1 per controllare il nord,\n
 * horiz_value = 1 per controllare est, horiz_value = -1 per controllare a ovest\n
 * ES. se voglio controllare est -> move = NORD/SUD, vert_value = 0, horiz_value = 1\n
 * se voglio controllare sud -> move = EST/OVEST, vert_value = 1, horiz_value = 0\n
 * ovest -> move = NORD/SUD, vert_value = 0, horiz_value = -1\n
 * nord -> move = EST/OVEST, vert_value = -1, horiz_value = 0\n
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param move direzione in cui si deve muovere il fantasma
 * @param vert_value serve a calcolare la posizione in cui deve guardare il fantasma
  * @param horiz_value serve a calcolare la posizione in cui deve guardare il fantasma
 * @return il metodo ritorna quante volte si è spostato in direzione "move" prima di trovare una strada libera in direzione vert_value e horiz_value
 */
int run_ghost(map map_info, char map[map_info.row][map_info.column], char move, int vert_value, int horiz_value) {
	int next_row = map_info.player_row + vert_value; // calcola la riga su qui deve guardare
	int next_column = map_info.player_column + horiz_value; // calcola la colonna su cui deve guardare
	//se vicolo ceco o fuori dai bordi della mappa
	if (next_row < 0 || next_row > map_info.row || next_column < 0 || next_column > map_info.column) {
		return 9999;
	}

	if (map[next_row][next_column] != WALL && map[next_row][next_column] != FLAG) { // se nel punto da guardare è libero allora fermati
		return 0;
	}
/*	continua a muoverti finchè trovi degli ostacoli dove stai guardando o dove ti stai muovendo*/
	if (move == NORD) {
		if (map[next_row-1][map_info.player_column] == WALL) {
			return 9999;
		}
		return run_ghost(map_info, map, move, vert_value - 1, horiz_value) + 1;
	}

	if (move == EST) {
		if (map[map_info.player_row][next_column+1] == WALL) {
			return 9999;
		}
		return run_ghost(map_info, map, move, vert_value, horiz_value + 1) + 1;
	}

	if (move == SUD) {
		if (map[next_row+1][map_info.player_column] == WALL) {
			return 9999;
		}
		return run_ghost(map_info, map, move, vert_value + 1, horiz_value) + 1;
	}

	if (move == OVEST) {
		if (map[map_info.player_row][next_column-1] == WALL) {
			return 9999;
		}
		return run_ghost(map_info, map, move, vert_value, horiz_value - 1) + 1;
	}
	return 9999;
}

/**
 * Controlla che ci sia una via libera retta tra il player e il target da raggiungere
 * Il metodo controlla tutte le posizione partendo dall'uscita fino ad arrivare da Snake.
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param target_col valore della colonna del target
 * @param target_row valore della riga del target
 * @param direction direzione in cui si può accedere al target ('h' per accesso orizzontale, 'v' per accesso verticale)
 * @return vero se non ci sono ostacoli tra il Snake e il target, altrimenti ritorna falso
 */
bool green_light_to_target (map* map_info, char map[map_info->row][map_info->column], int target_col, int target_row, char direction) {
	// se player e target non sono allineati in nessun modo
	if (map_info->player_row != target_row && map_info->player_column != target_col) {
		return false;
	}
	// se si è trovata una via libera
	if (map[target_row][target_col] == PLAYER) {
		return true;
	}
	else if (map[target_row][target_col] == WALL) {
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
	return false;
}

void mark_dead_end(map* map_info, char map[map_info->row][map_info->column], int ghost1, int ghost2, int start_row, int start_column, char direction) {
	if (direction == 'v') {
		for (int i = start_row - ghost1; i <= start_row + ghost2; i++) {
			map[i][start_column] = FLAG;
		}
	}
	if (direction == 'h') {
		for (int i = start_column - ghost1; i <= start_column + ghost2; i++) {
			map[start_row][i] = FLAG;
		}
	}

}

/**
 * Metodo invocato quendo si incontra un muro che blocca la strada.
 * in base alla direzione in cui si è incappati nel muro questo metodo lancia i fantasmi (vedi metodo run_ghost) e decide quale strada conviene prendere
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param moving_direction direzione in cui bisogna muoversi per raggirare il muro (v = Snake deve muoversi a NORD oppure a SUD, h = Snake deve muoversi a EST oppure a OVEST)
 * @param move contiene direzione in cui andare per raggirare il muro
 * @param all_steps contiene la lista dei passi fatti fino ad ora
 * @param max_steps_size contiene la dimensione massima attuale della lista dei passi fatti
 * @param points contiene i punti attuali della partita
 * @param tail vettore della coda di Snake
 * @param target_row riga del target
 * @param target_col colonna del target

 * @return quanti step sono stati necessari per raggirare il muro
 */
int go_around_wall(map* map_info, char map[map_info->row][map_info->column], char moving_direction, char move, char** all_steps, int* max_steps_size, int* points, vector** tail, int target_row, int target_col) {

	int ghost1_steps, ghost2_steps, better_path;

	bool dead_end = false;
	int vert_value, horiz_value; // rappresentano la direzione in cui guardare, vedere commento sulla funzione run_ghost
	if (moving_direction == 'v') {
		vert_value = 0;
		horiz_value = 1;
		if (move == OVEST) {
			vert_value = 0;
			horiz_value = -1;
		}

		ghost1_steps = run_ghost(*map_info, map, NORD, vert_value, horiz_value);
		ghost2_steps = run_ghost(*map_info, map, SUD, vert_value, horiz_value);

		// se entrambe le strade non hanno uscita allora vado nella direzione opposta al muro
		if (ghost1_steps >= 9999 && ghost2_steps >= 9999) {
			if (move == EST) {
				move = OVEST;
			}
			else if (move == OVEST) {
				move = EST;
			}
			better_path = 1;
			dead_end = true;
		}
		else {
			move = NORD;
			better_path = ghost1_steps;

			//se ghost2 è migliore
			if (ghost1_steps > ghost2_steps) {
				better_path = ghost2_steps;
				move = SUD;
				// se però ghost1 è una strada possibile e il target è in alto
				if (ghost1_steps < 9999 && target_row < map_info->player_row) {
					better_path = ghost1_steps;
					move = NORD;
				}
			}

			// se ghost1 è meglio però ghost2 è una strada possibile e il target è in basso
			if (ghost2_steps < 9999 && target_row > map_info->player_row) {
				better_path = ghost2_steps;
				move = SUD;
			}
		}
	}
	else if (moving_direction == 'h') {
		vert_value = 1;
		horiz_value = 0;
		if (move == NORD){
			vert_value = -1;
			horiz_value = 0;
		}
		ghost1_steps = run_ghost(*map_info, map, OVEST, vert_value, horiz_value);
		ghost2_steps = run_ghost(*map_info, map, EST, vert_value, horiz_value);
		// se entrambe le strade non hanno uscita allora vado nella direzione opposta al muro

		if (ghost1_steps >= 9999 && ghost2_steps >= 9999) {
			if (move == NORD) {
				move = SUD;
			}
			else if (move == SUD) {
				move = NORD;
			}
			better_path = 1;
			dead_end = true;
		}

		else {
			move = OVEST;
			better_path = ghost1_steps;
			//se ghost2 è migliore
			if (ghost1_steps > ghost2_steps) {
				better_path = ghost2_steps;
				move = EST;
				// se però ghost1 è una strada possibile e il target è a sinistra
				if (ghost1_steps < 9999 && target_col < map_info->player_column) {
					better_path = ghost1_steps;
					move = OVEST;
				}
			}

			// se ghost1 è meglio però ghost2 è una strada possibile e il target è a destra
			if (ghost2_steps < 9999 && target_col > map_info->player_column) {
				better_path = ghost2_steps;
				move = EST;
			}
		}
	}
/*	printf("\nghost1: %d\n", ghost1_steps);*/
/*	printf("ghost2: %d\n", ghost2_steps);*/
	// printf("better_move di h %c\n", move);
	int player_row = map_info->player_row;
	int player_column = map_info->player_column;

	//eseguo il percorso deciso per raggirare il muro
	for (int i = 0; i < better_path; i++) {
		run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail);
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
		print_map(map_info, map);
/*		printf("\nsequenza: %s\n", *all_steps);*/
	}
	if (dead_end) {
		mark_dead_end(map_info, map, ghost1_steps - 9999, ghost2_steps - 9999, player_row, player_column, moving_direction);
	}
	else if (ghost1_steps >= 9999) {
		mark_dead_end(map_info, map, ghost1_steps - 9999, 0, player_row, player_column, moving_direction);
	}
	else if (ghost2_steps >= 9999) {
		mark_dead_end(map_info, map, 0, ghost2_steps - 9999, player_row, player_column, moving_direction);
	}
/*	printf("\nsequenza: %s\n", *all_steps);*/

	return better_path;
}

/**
 * Controlla quanti muri ci nelle celle adiacenti ad un item passato per parametro
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param item_row riga dell'item
 * @param item_col colonna dell'item
 * @return il numero dei muri trovati
*/
int inspect_item(map* map_info, char map[map_info->row][map_info->column], int item_row, int item_col) {
	int counter = 0;

	if (map[item_row + 1][item_col] == WALL && item_row + 1 < map_info->row) {
		counter++;
	}
	if (map[item_row - 1][item_col] == WALL && item_row - 1 >= 0) {
		counter++;
	}
	if (map[item_row][item_col + 1] == WALL && item_col + 1 < map_info->column) {
		counter++;
	}
	if (map[item_row][item_col - 1] == WALL && item_col - 1 >= 0) {
		counter++;
	}
	return counter;
}

/** Sposta la posizione del personaggio verso un determinato target che gli viene passato come parametro
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param all_steps contiene la lista dei passi fatti fino ad ora
 * @param max_steps_size contiene la dimensione massima attuale della lista dei passi fatti
 * @param target_row riga del target
 * @param target_col colonna del target
 * @param points contiene i punti attuali della partita
 * @param tail vettore della coda di Snake

 * @return vero se ha interrotto forzatamente il raggiungimento del target, ritorna falso se lo ha raggiunto correttamente
 * parametri: la colonna del target, la riga del target, il punteggio, la coda
 */
bool goto_target(map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* max_steps_size, int target_row, int target_col, int *points, vector** tail) {

	char move; //contiene la mossa da fare
	char target_direction = 'h'; // serve a capire da quale direzione (verticale od orizzontale) si può accedere all'uscita
	char left_target = '?', right_target = '?';
	int panic_counter = 0; // contatore che se va oltre una certa quantità forza l'algoritmo a interrompere il raggiungimento del target attuale e si dirige all'uscita

	//salva la posizione a sinistra del target
	if (target_col - 1 >= 0) {
		left_target = map[target_row][target_col - 1];
	}
	//salva la posizione a destra del target
	if (target_col + 1 < map_info->column) {
		right_target = map[target_row][target_col + 1];
	}
	// se sia a sinistra che a destra ci sono dei muri allora il target è accessibile verticalmente
	if (left_target == WALL && right_target == WALL) {
		target_direction = 'v';
	}

	bool free_vertical = green_light_to_target(map_info, map, target_col, target_row, 'v');
	bool free_horizontal = green_light_to_target(map_info, map, target_col, target_row, 'h');
	//se il target è una moneta e questa moneta èd è presente una strada retta libera dal player alla moneta
	if (map[target_row][target_col] == BONUS_POINTS && (free_vertical || free_horizontal)) {
		if (target_col == map_info->player_column) {
			target_direction = 'v';
		}
		else if (target_row == map_info->player_row) {
			target_direction = 'h';
		}
	}
	//ignora il target se il target ha 3 o più muri intorno a esso oppure se non c'è una strada libera retta per raggiungerlo
	if (inspect_item(map_info, map, target_row, target_col) >= 3 && !green_light_to_target(map_info, map, target_col, target_row, target_direction)) {
		target_direction = 'i';
	}

	// printf("target_direction %c\n", target_direction);

	//finche il personaggio non trova percorso retto senza ostacoli verso il target e finche siamo nel limite dei passi consentito per raggiungerlo
	while (!green_light_to_target(map_info, map, target_col, target_row, target_direction) && panic_counter <= (map_info->row + map_info->column)*2) {

/*		printf("target_col %d\n", target_col);*/
/*		printf("target_row %d\n", target_row);*/
		panic_counter++;
		if (target_direction == 'h') { // vogliamo far corrispondere la riga del giocatore con quella dell'uscita
			// printf("accesso orizzontale\n");
			if (target_row == map_info->player_row) {
				if (target_col > map_info->player_column) {
					move = EST;
				}
				else if (target_col < map_info->player_column) {
					move = OVEST;
				}
				if (!run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail)) {
					panic_counter += go_around_wall(map_info, map, 'v', move, all_steps, max_steps_size, points, tail, target_row, target_col);
					//una volta raggirato il muro esegue la mossa che mi avvicina al target
					run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail);
					clear_map_tail(map_info, map);
					insert_tail_in_map(map_info, map, *tail);
					print_map(map_info, map);
/*					printf("\nsequenza: %s\n", *all_steps);*/
				}
				clear_map_tail(map_info, map);
				insert_tail_in_map(map_info, map, *tail);
				print_map(map_info, map);
				// printf("\nsequenza: %s\n", *all_steps);
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
			if (!run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail)) {
				panic_counter += go_around_wall(map_info, map, 'h', move, all_steps, max_steps_size, points, tail, target_row, target_col);
				run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail);
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
				if (!run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail)) {
					panic_counter += go_around_wall(map_info, map, 'h', move, all_steps, max_steps_size, points, tail, target_row, target_col);
					//una volta raggirato il muro esegue la mossa che mi avvicina al target
					run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail);
					print_map(map_info, map);
/*					printf("\nsequenza: %s\n", *all_steps);*/
				}
				print_map(map_info, map);
				// printf("\nsequenza: %s\n", *all_steps);
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
			if (!run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail)) {
				panic_counter += go_around_wall(map_info, map, 'v', move, all_steps, max_steps_size, points, tail, target_row, target_col);
				run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail);
			}
		}
		else if (target_direction == 'i') {
			return false;
		}
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
		print_map(map_info, map);
		// printf("\nsequenza: %s\n", *all_steps);
	}

	if (panic_counter >= (map_info->row + map_info->column) * 2) {
		return true;
	}

	// dopo essersi allineati con il target è possibile andare dritto verso di esso
	int steps_to_target = 0; // calcolo quanti passi mancano per raggiungere il target
	if (target_direction == 'h') {
		if (target_col > map_info->player_column) {
			steps_to_target = target_col - map_info->player_column;
		}
		else {
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
		run_move(map_info, map, move, points, all_steps, max_steps_size, true, tail);
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, *tail);
		print_map(map_info, map);
		// printf("\nsequenza: %s\n", *all_steps);
	}
	return false;
}

/**
 * Questo metodo inizia la modalità CPU.\n
 * controlla se nel percorso per raggiungere l'uscita si trovano delle monete o dei trapani, una volta finite le monete e i trapani raggiungibili, punta all'uscita
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 * @param force_quit quando questo flag è vero significa che il programma è stato chiamato con il parametro da linea di comando e quindi una volta terminato l'algoritmo, il terminare il programma deve terminare senza ulteriori stampe
*/
void cpu_algorithm(map* map_info, char map[map_info->row][map_info->column], bool force_quit) {
	int points = 1000;
	vector *tail = NULL;
	map_info->tail_len = 0;
	map_info->drill_counter = 0;
	int max_steps_size = map_info->row + map_info->column; //lunghezza della sequenza di passi
	char *all_steps = (char *) calloc(max_steps_size, sizeof(char)); // array che conterrà la sequenza di passi,
	int column = map_info->player_column; // colonna da cui partire per scannerizzare la mappa in presenza di monete
	int further_column = map_info->column - 1; // colonna più lontana da scannerizzare per cercare delle monete
	bool deep_inspect = true; // condizione che rimane vera fino a che non si finisce di scannerizzare la mappa

	//calcolo la colonna di arrivo e di partenza per la scannerizzazione
	if (map_info->player_column > map_info->exit_column) {
		column = map_info->player_column + 10;
		if (map_info->exit_column - 10 > 0) {
			further_column = map_info->exit_column - 10;
		}
		else {
			further_column = 0;
		}
	}
	else {
		column = map_info->player_column - 10;
		if (map_info->exit_column + 10 <= map_info->column) {
			further_column = map_info->exit_column + 10;
		}
		else {
			further_column = map_info->column;
		}
	}

	//evito di uscire dai limiti della mappa
	if (column <= 0) {
		column = 1;
	}
	else if (column >= map_info->column) {
		column = map_info->column - 1;
	}

	//ricerca delle monete e trapani facendo il seguente percorso: dall'alto al basso -> cambio colonna -> dal basso all'alto
	//-> cambio colonna -> dall'alto al basso ecc...
	int start = 0, end = map_info->row - 1;
	bool panic = false; // se si avvera il panic interrompe la scannerizzazione e si dirige verso l'uscita
	while (deep_inspect) {
		if (panic) {
			break;
		}
		int row = start;
		while (row != end) {
			if (panic){
				break;
			}
			if (map[row][column] == DRILL) {
				// printf("trapano in %d %d\n", row, column);
				panic = goto_target(map_info, map, &all_steps, &max_steps_size, row, column, &points, &tail);
			}
			else if (map[row][column] == BONUS_POINTS) {
				// printf("\t\tMONETA IN %d %d\n", row, column);
				panic = goto_target(map_info, map, &all_steps, &max_steps_size, row, column, &points, &tail);
			}
			if (row < end){
				row++;
			}
			else {
				row --;
			}
		}
		if (column < further_column) {
			column += 1;
		}
		else {
			column -= 1;
		}

		if (column == further_column) {
			deep_inspect = false;
		}
		// scambio start con end
		int temp = end;
		end = start;
		start = temp;
	}

	//raggiungimento dell'uscita
	goto_target(map_info, map, &all_steps, &max_steps_size, map_info->exit_row, map_info->exit_column, &points, &tail);
	if (!force_quit) {
		printf("\n\n\nfull path:\n");
		clear_map_tail(map_info, map);
		insert_tail_in_map(map_info, map, tail);
		print_map(map_info, map);
		printf("\n");
	}
	printf("\t- Score: %d\n", points);
	printf("\t- Sequence of steps: %s\n", all_steps);
	printf("\t- Number of steps: %ld\n", strlen(all_steps));
	if (!force_quit) {
		printf("Press enter to continue");
		getchar();
		getchar();
	}
	free(all_steps);
}
