#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

typedef enum {
	EMPTY = 0,
	OBSTACLE = 1,
	FULL = 2,
	TEMP = 3
} tile_type;

typedef struct s_tile* p_map_tile;

typedef struct s_tile {
	int x;
	int y;
	tile_type type;
	// ptr to max 4 neighbours (up right down left)
} map_tile;

typedef struct s_rect* p_rect;

typedef struct s_rect {
	int x;
	int y;
	int w;
	int h;
	int size;
} rect;

char	ft_tile_type_to_char(tile_type type)
{
	if (type == OBSTACLE)
		return 'o';
	else if (type == FULL)
		return 'x';
	else if (type == TEMP)
		return 'a';
	return '.'; // default

}

void	ft_print_map(p_map_tile** map, int size)
{
	int x = 0;
	int y = 0;
	int obstacles = 0;
	int filled = 0;

	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			if (map[y][x]->type == OBSTACLE)
				obstacles++;
			else if (map[y][x]->type == FULL)
				filled++;
			printf("%c", ft_tile_type_to_char(map[y][x]->type));
			x++;

		}
		printf("\n");
		y++;
	}

	printf("\n");
	//printf("Total of %d obstacles\n", obstacles);
	//printf("Total of %d filled\n", filled);

}

void	ft_draw_rect(p_map_tile** map, int start_x, int start_y, int width, int height)
{
	int x = start_x;
	int y = start_y;
	width += x;
	height += y;

	// safety if width/height is > size ?

	while (y < height)
	{
		x = start_x;
		while (x < width)
		{
			if (map[y][x]->type != OBSTACLE)
				map[y][x]->type = FULL;
			x++;
		}
		y++;
	}
}

rect* ft_create_rect(int x, int y, int width, int height)
{
	rect* rectangle;
	rectangle = malloc(sizeof(rect));

	rectangle->x = x;
	rectangle->y = y;
	rectangle->w = width;
	rectangle->h = height;
	rectangle->size = width * height; // not correct, check width and height

	return rectangle;
}

map_tile	*ft_create_tile(int x, int y)
{
	map_tile* cur_tile; // create new map_tile pointer
	cur_tile = malloc(sizeof(map_tile)); // malloc map_tile

	cur_tile->x = x; // save x and y, might be usefull
	cur_tile->y = y;

	// temp density : can move this logic to elsewhere
	int density = 1;

	if ((rand() % 50) < density) // 1 in 30 (3.3%)
		cur_tile->type = OBSTACLE;
	else
		cur_tile->type = EMPTY;

	return cur_tile;
}

p_map_tile** ft_create_map(int size) {

	int x = 0;
	int y = 0;
	p_map_tile** map;

	time_t t;
	srand((unsigned)time(&t));

	map = (p_map_tile**)malloc(sizeof(map_tile) * size * size);

	while (y < size)
	{

		map[y] = (p_map_tile*)malloc(sizeof(map_tile) * size); // allocate row

		x = 0;
		while (x < size)
		{
			map[y][x] = ft_create_tile(x, y);
			x++;
		}
		y++;
	}

	return map;

}

void	ft_reset_map(p_map_tile **map, int size)
{
	int x = 0;
	int y = 0;
	int removed = 0;

	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			if (map[y][x]->type == FULL || map[y][x]->type == TEMP)
			{
				map[y][x]->type = EMPTY;
				removed++;
			}
			x++;
		}
		y++;
	}

	printf("Removed %d full\n", removed);

}

void	ft_clean(p_map_tile** map, int size)
{
	int x = 0;
	int y = 0;

	x = 0;
	while (y < size)
	{
		free(map[y]);
		x = 0;
		while (x < size)
		{
			x++;
		}
		y++;
	}
	free(map);
}

rect* ft_brute_solve(p_map_tile** map, int size, int start_x, int start_y)
{
	int x = start_x;
	int y = start_y;
	int max_x = size - x;
	int max_y = size - y;

	// Outcome A : 
	// Keep going right until obstacle is met
	// Save current square at previous max_x and continue with new max_x
	// This creates branches and we should choose the one with the largest size in this position
	// Recursion?
	y = start_y;
	while (y < max_y && y < size)
	{
		x = start_x;
		while (x < max_x)
		{
			if (map[y][x]->type == OBSTACLE)
			{
				max_x = x;
				max_y = y;
				goto end; // Exit loop
			}
			x++;
		}
		y++;
	}

end:

	ft_draw_rect(map, start_x, start_y, max_x, max_y);
	return ft_create_rect(start_x, start_y, max_x, max_y);  // width and height are not correct

}

int	main(void)
{
	p_map_tile** map;
	int	size = 16; // 0 - 15 * 0 - 15, maybe add function to use 1-16 (size - 1)
	char input;
	int x = 0;
	int y = 0;
	struct s_rect* rect;
	struct s_rect* big;
	
	big = ft_create_rect(0,0,0,0);
	map = ft_create_map(size);
	input = 0;
	while (y < size)
	{
		ft_reset_map(map, size);
		rect = ft_brute_solve(map, size, x, y);

		if (big->size < rect->size)
		{
			free(big);
			big = rect;
		}

		ft_print_map(map, size);
		//Sleep(500);

		x++;
		if (x == size) {
			x = 0;
			y++;
		}

	}
	
	ft_reset_map(map, size);
	ft_draw_rect(map, big->x, big->y, big->w, big->h);
	ft_print_map(map, size);
	printf("Largest rect: x %d, y %d - width %d, height %d\n", big->x, big->y, big->w, big->h);

	ft_clean(map, size);
		
	return(0);
}
