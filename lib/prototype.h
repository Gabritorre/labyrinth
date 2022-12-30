/*
file che contine: costanti, strutture e le firme delle funzioni
*/

//--------------------- commands
#define NORD 'N'
#define SUD 'S'
#define EST 'E'
#define OVEST 'O'
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

//--------------------- constants values
#define QUANTITY_BONUS 11

//--------------------- structures

typedef struct List {
	int row;
	int column;
	struct List *next;
} vector;

typedef struct {
	int row;
	int column;

	int player_row;
	int player_column;
	int tail_len;

	int drill_counter;

	int exit_row;
	int exit_column;
} map;

vector* vector_create(int row, int column);

void vector_append(vector** list, map* map_info);

void move_tail(map* map_info, vector** tail);

void print_vector(vector *list);

void reset_nodes_till(vector **list, int node_row, int node_col, int *points, map* map_info);

void delete_half_tail(vector **list, int middle_node, int *points, map* map_info);

void dealloc_vector(vector **list);

void clear_map_tail(map* map_info, char map[map_info->row][map_info->column]);

void insert_tail_in_map(map* map_info, char map[map_info->row][map_info->column], vector* tail);

char* build_sequence (char **steps, int *max_step_size, char move);

void check_next_step(map* map_info, char map[map_info->row][map_info->column], char next_step, bool *win, int *points, vector** tail, int next_row, int next_column);

int run_move(map* map_info, char map[map_info->row][map_info->column], char move, int *points, char **steps, int *step_size, bool ai_flag, vector** tail);

void print_map(map* map_info, char map[map_info->row][map_info->column]);

int command_interpreter (char command, char *move);

void start_interactive_mode(map* map_info, char map[map_info->row][map_info->column]);

int run_ghost(map map_info, char map[map_info.row][map_info.column], char move, int vert_value, int horiz_value);

bool green_light_to_target (map* map_info, char map[map_info->row][map_info->column], int target_col, int target_row, char direction);

int go_around_wall(map* map_info, char map[map_info->row][map_info->column], char moving_direction, char move, char** all_steps, int* all_steps_size, int* points, vector** tail, int target_row, int target_col);

int inspect_item(map* map_info, char map[map_info->row][map_info->column], int item_row, int item_col);

bool goto_target(map* map_info, char map[map_info->row][map_info->column], char** all_steps, int* all_steps_size, int target_col, int target_row, int *points, vector** tail);

void cpu_algorithm(map* map_info, char map[map_info->row][map_info->column]);