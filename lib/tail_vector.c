/*
file contentente le funzioni per gestire la coda del personaggio
*/

vector* vector_create(int row, int column) {
	vector *list = (vector*) malloc(sizeof(vector));
	if (!list)
		exit(EXIT_FAILURE);
	list->row = row;
	list->column = column;
	list->next = NULL;
	return list;
}

void vector_append(vector** list, map* map_info) {
	if (*list == NULL) {
		*list = vector_create(map_info->player_row, map_info->player_column);
		map_info->tail_len += 1;
	}
	else {
		vector_append(&((*list)->next), map_info);
	}
}

//muove la coda in modo tale che segua snake, ogni nodo copierà le coordinate del nodo che gli sta davanti fino a che l'ultimo nodo
//copia l'ultima posizione di snake
void move_tail(map* map_info, vector** tail) {
	// se la coda non è ancora presente
	if ((*tail) == NULL){
		return;
	}
	// se siamo nell'ultimo pezzo di coda (quello più vicino a snake)
	if (((*tail)->next) == NULL) {
		(*tail)->row = map_info->player_row;
		(*tail)->column = map_info->player_column;
		return;
	}
	// se siamo in un nodo che è ancora valido, gli faccio copiare le coordinate del nodo successivo
	if((*tail)->row != -1){
		(*tail)->row = ((*tail)->next)->row;
		(*tail)->column = ((*tail)->next)->column;
	}
	move_tail(map_info, &((*tail)->next));
}

void print_vector(vector *list) {
	if (list == NULL)
		return;
	printf(" (%d %d)", list->row, list->column);
	print_vector(list->next);
}

// resetta il contenuto dei nodi fino al raggiungimento di un determinato nodo
void reset_nodes_till(vector **list, int node_row, int node_col, int *points, map* map_info) {
	if ((*list)->row != -1){
		*points -= 10;
		map_info->tail_len -= 1;
	}
	if ((*list)->row == node_row && (*list)->column == node_col) {
		(*list)->row = -1;
		(*list)->column = -1;
		return;
	}
	(*list)->row = -1;
	(*list)->column = -1;
	reset_nodes_till(&((*list)->next), node_row, node_col, points, map_info);
}

// resetta la coda fino a metà
void delete_half_tail(vector **list, int middle_node, int *points, map* map_info) {
	if ((*list)->row != -1){
		*points -= 10;
		middle_node -= 1;
		map_info->tail_len -= 1;
	}
	if (middle_node <= 0) {
		(*list)->row = -1;
		(*list)->column = -1;
		return;
	}
	(*list)->row = -1;
	(*list)->column = -1;
	delete_half_tail(&((*list)->next), middle_node, points, map_info);
}

void dealloc_vector(vector **list) {
	if ((*list) == NULL)
		return;
	dealloc_vector(&((*list)->next));
	free((*list));
}

