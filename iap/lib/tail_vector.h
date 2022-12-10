/*
 * remove_all_to (rimuovere tutti i nodi fino ad un certo nodo)
 * clear vector
 * */

typedef struct List {
	int row;
	int column;
	struct List *next;
} vector;

vector* l_create(int row, int column) {
	vector *l = (vector*) malloc(sizeof(vector));
	if (!l)
		exit(EXIT_FAILURE);
	l->row = row;
	l->column = column;
	l->next = NULL;
	return l;
}

void vector_append(vector** l_orig, int row, int column) {
	if (*l_orig == NULL) {
		*l_orig = l_create(row, column);
	}
	else {
		vector_append(&((*l_orig)->next), row, column);
	}
}

void print_vector(vector *l) {
	if (l == NULL) // fine lista, cioè lista vuota
		return;
	else {
		printf(" (%d %d)", l->row, l->column);
		print_vector(l->next);
	}
}


