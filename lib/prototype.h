/**
 * @file prototype.h 
 * @brief contiene costanti, strutture e le firme delle funzioni
*/

//--------------------- commands
#define NORD 'W'
#define SUD 'S'
#define EST 'D'
#define OVEST 'A'
#define QUIT 'Z'

//--------------------- map items
#define WALL '#'
#define STEP '.'
#define BONUS_POINTS '$'
#define HALF_POINTS '!'
#define DRILL 'T'
#define PLAYER 'o'
#define EXIT '_'
#define TAIL 'x'
#define FLAG '/'

//--------------------- constants values
///contiene la quantità di punti bonus da dare quando si prende una moneta
#define QUANTITY_BONUS 11

//--------------------- structures
/**linked list che serve a gestire la coda di Snake*/
typedef struct List {
	/// Riga del nodo
	int row;
	/// Colonna del nodo
	int column;
	/// Puntatore al nodo successivo
	struct List *next;
} vector;

/**Struttura contenente le informazione più inmportanti della mappa */
typedef struct {
	/// Numero di righe della mappa
	int row;
	/// Numero di colonne della mappa
	int column;

	/// Riga dell'attuale posizione di Snake
	int player_row;
	/// Colonna dell'attuale posizione di Snake
	int player_column;
	/// Lunghezza attuale della coda
	int tail_len;

	/// Contatore di quanti muri si possono rompere
	int drill_counter;

	/// Riga dell'uscita
	int exit_row;
	/// Colonna dell'uscita
	int exit_column;
} map;


vector* vector_create(int row, int column);

void vector_append(vector** list, map* map_info);

void move_tail(map* map_info, vector** tail, char map[map_info->row][map_info->column]);

void print_vector(vector *list);

void reset_nodes_till(vector **list, int node_row, int node_col, int *points, map* map_info, char map[map_info->row][map_info->column]);

void delete_half_tail(vector **list, int middle_node, int *points, map* map_info, char map[map_info->row][map_info->column]);

void dealloc_vector(vector **list);

void clear_map_tail(map* map_info, char map[map_info->row][map_info->column]);

void insert_tail_in_map(map* map_info, char map[map_info->row][map_info->column], vector* tail);

char* build_sequence (char **steps, int *max_step_size, char move);

void check_next_step(map* map_info, char map[map_info->row][map_info->column], char next_step, bool *win, int *points, vector** tail, int next_row, int next_column);

int run_move(map* map_info, char map[map_info->row][map_info->column], char move, int *points, char **steps, int *step_size, bool ai_flag, vector** tail);

void print_map(map* map_info, char map[map_info->row][map_info->column]);

int command_interpreter (char command, char *move);

void start_interactive_mode(map* map_info, char map[map_info->row][map_info->column]);

void dead_end(map *map_info, char map[map_info->row][map_info->column], int ghost1, int ghost2, int start_row, int start_column, char direction);

int run_ghost(map map_info, char map[map_info.row][map_info.column], char move, int vert_value, int horiz_value);

bool green_light_to_target (map* map_info, char map[map_info->row][map_info->column], int target_col, int target_row, char direction);

int go_around_wall(map* map_info, char map[map_info->row][map_info->column], char moving_direction, char move, char** all_steps, int* all_steps_size, int* points, vector** tail, int target_row, int target_col);

int inspect_item(map* map_info, char map[map_info->row][map_info->column], int item_row, int item_col);

bool goto_target(map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int target_col, int target_row, int *points, vector** tail);

void cpu_algorithm(map* map_info, char map[map_info->row][map_info->column], bool force_quit);
