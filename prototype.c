#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	EMPTY = 0,
	OBSTACLE = 1,
	FULL = 2
} tile_type;

typedef struct s_tile* p_map_tile;

typedef struct s_tile {
	int x;
	int y;
	tile_type type;
	// ptr to max 4 neighbours
} map_tile;

char	ft_tile_type_to_char(tile_type type) {

	if (type == OBSTACLE)
		return 'o';
	else if (type == FULL)
		return 'x';

	return '.'; // default

}

void	ft_print_map(p_map_tile** map, int size) {

	int x = 0;
	int y = 0;
	int obstacles = 0;
	int filled = 0;

	while (x < size)
	{

		y = 0;
		while (y < size)
		{

			if (map[y][x]->type == OBSTACLE)
				obstacles++;
			else if (map[y][x]->type == FULL)
				filled++;

			printf("%c", ft_tile_type_to_char(map[y][x]->type));
			y++;

		}

		printf("\n");
		x++;
	}

	printf("\n");
	printf("Printed total of %d characters\n", size * size);
	printf("Total of %d obstacles\n", obstacles);
	printf("Total of %d filled\n", filled);

}

void	ft_draw_rect(p_map_tile** map, int start_x, int start_y, int width, int height)
{
	int x = start_x;
	int y = start_y;
	width += start_x;
	height += start_y;

	while (x < width)
	{
		y = start_y;
		while (y < height)
		{

			map[x][y]->type = FULL;
			y++;
		}
		x++;
	}

}

map_tile	*ft_create_tile(int x, int y)
{
	map_tile* cur_tile; // = { x, y, empty };
	cur_tile = malloc(sizeof(map_tile));

	cur_tile->x = x;
	cur_tile->y = y;
	cur_tile->type = EMPTY;

	return cur_tile;
}

int	main(void)
{
	int x = 0;
	int y = 0;
	int	size = 16; // 0 - 15 * 0 - 15
	p_map_tile** map;

	map = (p_map_tile**)malloc(sizeof(map_tile) * size * size);

	while (x < size)
	{

		map[x] = (p_map_tile*)malloc(sizeof(map_tile) * size);

		y = 0;
		while (y < size)
		{

			map[x][y] = ft_create_tile(x, y);
			y++;

		}

		x++;
	}

	// random obstacles

	map[1][1]->type = OBSTACLE;
	map[14][14]->type = OBSTACLE;
	map[1][14]->type = OBSTACLE;
	map[14][1]->type = OBSTACLE;
	//map[2][3]->type = OBSTACLE;

	ft_draw_rect(map, 4, 4, 8, 6);


	ft_print_map(map, size);

	// Free memory

	x = 0;
	while (x < size)
	{
		free(map[x]);
		y = 0;
		while (y < size)
		{
			//free(map[x][y]);
			y++;
		}
		x++;
	}
	free(map);

	return(0);
}
