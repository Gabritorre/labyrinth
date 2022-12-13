/*
 * remove_all_to (rimuovere tutti i nodi fino ad un certo nodo)
 * clear vector
 * */

typedef struct List {
	int row;
	int column;
	struct List *next;
} vector;

vector* vector_create(int row, int column) {
	vector *list = (vector*) malloc(sizeof(vector));
	if (!list)
		exit(EXIT_FAILURE);
	list->row = row;
	list->column = column;
	list->next = NULL;
	return list;
}

void vector_append(vector** list, struct Map* map_info) {
	if (*list == NULL) {
		*list = vector_create(map_info->player_row, map_info->player_column);
		map_info->tail_len += 1;
	}
	else {
		vector_append(&((*list)->next), map_info);
	}
}

void print_vector(vector *list) {
	if (list == NULL)
		return;
	printf(" (%d %d)", list->row, list->column);
	print_vector(list->next);
}

// resetta il contenuto dei nodi fino al raggiungimento di un determinato nodo
void reset_nodes_till(vector **list, int node_row, int node_col, int *points, struct Map* map_info){
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

void delete_half_tail(vector **list, int middle_node, int *points, struct Map* map_info) {
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

void dealloc_vector(vector **list){
	if ((*list) == NULL)
		return;
	dealloc_vector(&((*list)->next));
	free((*list));
}
