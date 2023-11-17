/**
 * @file tail_vector.c
 * @brief Contiene le funzioni per gestire la coda del personaggio
*/

/**
 * Crea un nuovo nodo della coda
 * @param row riga del nodo da aggiungere
 * @param column colonna del nodo da aggiungere
 * @return ritorna il nodo creato
*/
vector* create_tail(int row, int column) {
	vector *node = (vector*) malloc(sizeof(vector));
	if (!node)
		exit(EXIT_FAILURE);
	node->row = row;
	node->column = column;
	node->next = NULL;
	return node;
}

/**
 * Appende un nodo alla coda esistente
 * @param list la coda
 * @param map_info contiene le informazioni della mappa
 */
void tail_append(vector** list, map* map_info) {
	if (*list == NULL) {
		*list = create_tail(map_info->player_row, map_info->player_column);
		map_info->tail_len += 1;
	}
	else {
		tail_append(&((*list)->next), map_info);
	}
}

/**
 * Muove la coda in modo tale che segua snake, ogni nodo copierà le coordinate del nodo successivo.\n
 * L'ultimo nodo copia l'ultima posizione di snake
 * @param map_info contiene le informazioni della mappa
 * @param list la coda
 * @param map la mappa
*/
void move_tail(map* map_info, vector** list, char map[map_info->row][map_info->column]) {
	// se la coda non è ancora presente
	if ((*list) == NULL){
		return;
	}
	// se siamo nell'ultimo pezzo di coda (quello più vicino a snake)
	if (((*list)->next) == NULL && (*list)->row != -1) {
		if(map[(*list)->row][(*list)->column] != FLAG) {
			map[(*list)->row][(*list)->column] = STEP;
		}
		(*list)->row = map_info->player_row;
		(*list)->column = map_info->player_column;
		return;
	}
	// se siamo in un nodo che è ancora valido, gli faccio copiare le coordinate del nodo successivo
	if((*list)->row != -1){
		if(map[(*list)->row][(*list)->column] != FLAG){
			map[(*list)->row][(*list)->column] = STEP;
		}
		(*list)->row = ((*list)->next)->row;
		(*list)->column = ((*list)->next)->column;
	}
	move_tail(map_info, &((*list)->next), map);
}

/**
 * Stampa le coordinate dei nodi della coda
 * @param list la coda
*/
void print_tail(vector *list) {
	if (list == NULL)
		return;
	printf(" (%d %d)", list->row, list->column);
	print_tail(list->next);
}

/**
 * Resetta il contenuto dei nodi fino al raggiungimento di un determinato nodo
 * @param list la coda
 * @param node_row la riga dell'ultimo nodo da eliminare
 * @param node_col la colonna dell'ultimo nodo da eliminare
 * @param points contiene i punti attuali della partita
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 */
void reset_nodes_till(vector **list, int node_row, int node_col, int *points, map* map_info, char map[map_info->row][map_info->column]) {
	// se siamo arrivati all'ultimo nodo da eliminare
	if ((*list)->row == node_row && (*list)->column == node_col) {
		if (map[(*list)->row][(*list)->column] != FLAG){
			map[(*list)->row][(*list)->column] = STEP;
		}
		(*list)->row = -1;
		(*list)->column = -1;
		*points -= 10;
		map_info->tail_len -= 1;

		return;
	}

	// se siamo in un nodo ancora valido (non è stato ancora resettato)
	if((*list)->row != -1){
		if (map[(*list)->row][(*list)->column] != FLAG){
			map[(*list)->row][(*list)->column] = STEP;
		}
		(*list)->row = -1;
		(*list)->column = -1;

		*points -= 10;
		map_info->tail_len -= 1;
	}
	reset_nodes_till(&((*list)->next), node_row, node_col, points, map_info, map);
}

/**
 * Resetta la coda fino a metà
 * @param list la coda
 * @param middle_node counter di quanti nodi sono da eliminare partendo dal nodo della coda più lontano (il primo nodo della lista)
 * @param points contiene i punti attuali della partita
 * @param map_info contiene le informazioni della mappa
 * @param map la mappa
 */
void delete_half_tail(vector **list, int middle_node, int *points, map* map_info, char map[map_info->row][map_info->column]) {
	// se siamo in un nodo ancora valido (non è stato ancora resettato)
	if ((*list)->row != -1){
		*points -= 10;
		middle_node -= 1;
		if(map[(*list)->row][(*list)->column] != FLAG) {
			map[(*list)->row][(*list)->column] = STEP;
		}
		map_info->tail_len -= 1;
	}
	// se siamo arrivati all'ultimo nodo da eliminare
	if (middle_node <= 0) {
		if(map[(*list)->row][(*list)->column] != FLAG) {
			map[(*list)->row][(*list)->column] = STEP;
		}
		(*list)->row = -1;
		(*list)->column = -1;
		return;
	}
	(*list)->row = -1;
	(*list)->column = -1;
	delete_half_tail(&((*list)->next), middle_node, points, map_info, map);
}

/**
 * Dealloca tutti i nodi della coda
 * @param list la coda
*/
void dealloc_vector(vector **list) {
	if ((*list) == NULL)
		return;
	dealloc_vector(&((*list)->next));
	free((*list));
}
