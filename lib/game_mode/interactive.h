
void start_interactive_mode(struct Map* map_info, char map[map_info->row][map_info->column]) {
	print_map(map_info, map);
	char move;
	printf("\ninserisci mossa: ");
	scanf(" %c", &move);
}
