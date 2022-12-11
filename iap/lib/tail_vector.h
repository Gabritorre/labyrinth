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

void vector_append(vector** list, int row, int column) {
	if (*list == NULL) {
		*list = vector_create(row, column);
	}
	else {
		vector_append(&((*list)->next), row, column);
	}
}

void print_vector(vector *list) {
	if (list == NULL)
		return;
	else {
		printf(" (%d %d)", list->row, list->column);
		print_vector(list->next);
	}
}

// resetta il contenuto dei nodi fino al raggiungimento di un determinato nodo
void reset_nodes_till(vector **list, int node_row, int node_col, int *points){
	if ((*list)->row != -1)
		*points -= 10;
	if ((*list)->row == node_row && (*list)->column == node_col) {
		(*list)->row = -1;
		(*list)->column = -1;
		return;
	}
	(*list)->row = -1;
	(*list)->column = -1;
	reset_nodes_till(&((*list)->next), node_row, node_col, points);
}
