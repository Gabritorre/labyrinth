//commands
#define NORD 'N'
#define SUD 'S'
#define EST 'E'
#define OVEST 'O'
#define QUIT 'Z'

// map items
#define WALL '#'
#define STEP '.'
#define BONUS_POINTS '$'
#define HALF_POINTS '!'
#define DRILL 'T'
#define PLAYER 'o'
#define EXIT '_'
#define TAIL 'x'


#define QUANTITY_BONUS 11

struct Map {
	int row;
	int column;

	int player_row;
	int player_column;
	int drill_counter;

	int exit_row;
	int exit_column;
};

void title() {
	printf(" -------------------\n");
	printf("|  SNAKE LABYRINTH  |\n");
	printf(" -------------------\n");
}

void print_game_info() {
	title();
	printf("Snake labyrinth è un gioco che presenta due modalità:\n");
	printf("\tModalità interattiva: si tratta di far uscire il proprio personaggio dal labirinto muovendonsi in quattro direzioni.\n"
			"\tLe direzioni sono associate ai seguenti tasti:\n"
				"\t\t- 'N'/'n' nord (movimento verso l'alto)\n"
				"\t\t- 'E'/'e' est (movimento verso destra)\n"
				"\t\t- 'S'/'s' sud (movimento verso il basso)\n"
				"\t\t- 'O'/'o' ovest (movimento verso sinistra)\n");
	printf("\tIl personaggio parte da una base di 1000 punti. Una volta usciti dal labirinto si ottiene un punteggio che si basa su quanti movimenti sono stati fatti e in base a dei modificatori che si trovano nella mappa:\n"
			"\tI modificatori sono i seguenti:\n"
				"\t\t- '$' aggiunge 10 punti al punteggio\n"
				"\t\t- '!' dimezza il proprio punteggio\n"
				"\t\t- ogni movimento toglie un punto\n");
	printf("\tAltri simboli che sono presenti nel gioco sono:\n"
				"\t\t- 'o' il proprio personaggio\n"
				"\t\t- '#' parete (non si può passare attraverso)\n"
				"\t\t- '_' uscita della mappa\n\n");
	printf("La seconda modalità: todo\n");
	printf("è possibili aggiungere le proprie mappe aggiungendole al file di testo: todo\n\n");

}

void clear_map_tail(struct Map* map_info, char map[map_info->row][map_info->column]) {
	for (int row = 0; row < map_info->row; row++) {
		for (int column = 0; column < map_info->column; column++) {
			if (map[row][column] == TAIL) {
				map[row][column] = ' ';
			}
		}
	}
}

void print_map(struct Map* map_info, char map[map_info->row][map_info->column]) {
	for (int row = 0; row < map_info->row; row++) {
		printf("\n");
		for (int column = 0; column < map_info->column; column++) {
			printf("%c", map[row][column]);
		}
	}
	clear_map_tail(map_info, map);
}

