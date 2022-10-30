char NORD = 'N';
char SUD = 'S';
char EST = 'E';
char OVEST = 'O';
char QUIT = 'Z';

struct Map {
	int row;
	int column;

	int player_row;
	int player_column;

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
	printf("\tUna volta usciti dal labirinto si ottiene un punteggio che si basa su quanti movimenti sono stati fatti e in base a dei modificatori che si trovano nella mappa:\n"
			"\tI modificatori sono i seguenti:\n"
				"\t\t- '$' aggiunge 3 punti al punteggio\n"
				"\t\t- '!' dimezza il proprio punteggio\n"
				"\t\t- ogni movimento toglie un punto\n");
	printf("\tAltri simboli che sono presenti nel gioco sono:\n"
				"\t\t- 'o' il proprio personaggio\n"
				"\t\t- '#' parete (non si può passare attraverso)\n"
				"\t\t- '_' uscita della mappa\n\n");
	printf("La seconda modalità: todo\n");
	printf("è possibili aggiungere le proprie mappe aggiungendole al file di testo: todo\n\n");

}

void print_map(struct Map* map_info, char map[map_info->row][map_info->column]) {
	printf("\nmappa:");
	for (int row = 0; row < map_info->row; row++) {
		printf("\n");
		for (int column = 0; column < map_info->column; column++) {
			printf("%c", map[row][column]);
		}
	}
}

