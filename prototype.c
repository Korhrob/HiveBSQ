#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// This was not my final solution and probably doesn't compile properly
// The ideas and solution themselves are good, so you can reference this for the actualy solving part

typedef enum { // Makes it easier to compare tile types instead of chars etc
	EMPTY = 0,
	OBSTACLE = 1,
	FULL = 2,
} tile_type;

typedef struct s_tile* p_map_tile;

typedef struct s_tile {
	int x; // Not really needed
	int y; // --||--
	tile_type type;
} map_tile;

typedef struct s_rect* p_rect;

typedef struct s_rect { // Used to pass around a lot of arguments
	int x;
	int y;
	int w;
	int h;
	int size;
} rect;

char	ft_tile_type_to_char(tile_type type, char symbols[4])
{
	if (type == OBSTACLE)
		return symbols[1];
	else if (type == FULL)
		return symbols[2];
	return symbols[0]; // default

}

void	ft_print_map(p_map_tile** map, int size, char symbols[4])
{
	int x = 0;
	int y = 0;

	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			printf("%c", ft_tile_type_to_char(map[y][x]->type, symbols));
			x++;

		}
		printf("\n");
		y++;
	}
	printf("\n");

}

void	ft_draw_rect(p_map_tile** map, int start_x, int start_y, int width, int height)
{
	int x = start_x;
	int y = start_y;
	width += x;
	height += y;

	// safety if width/height is > size ?

	while (y <= height) // replace 16 with size
	{
		x = start_x;
		while (x <= width)
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
	rectangle->size = width * height; // I actually never used width * height

	return rectangle;
}

map_tile	*ft_create_tile(int x, int y)
{
	map_tile* cur_tile; // create new map_tile pointer
	cur_tile = malloc(sizeof(map_tile)); // malloc map_tile, might not need
	// check if malloc fails

	cur_tile->x = x; // wasn't actually usefull
	cur_tile->y = y; // wasn't actually usefull

	// create obstacles, used this to generate a map
	// but you should probably just read the string where ever this is called
	// and convert it to enum
	int density = 10;

	if ((rand() % 30) < density) // 1 in 30 (3.3%)
		cur_tile->type = OBSTACLE;
	else
		cur_tile->type = EMPTY;

	return cur_tile;
}

p_map_tile** ft_create_map(int size) { // map can be rectangle, use width and height instead of size

	int x = 0;
	int y = 0;
	p_map_tile** map;

	// used to initialize randomizing
	time_t t;
	srand((unsigned)time(&t));

	map = (p_map_tile**)malloc(sizeof(map_tile) * size * size);
	// check if malloc fails

	while (y < size)
	{

		map[y] = (p_map_tile*)malloc(sizeof(map_tile) * size); // allocate row
		// check if malloc fails
		
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

	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			if (map[y][x]->type == FULL)
				map[y][x]->type = EMPTY;

			x++;
		}
		y++;
	}


}

void	ft_clean(p_map_tile** map, int size) // clean memory if malloc fails at any point
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
			//free(map[y][x]) // might not need
			x++;
		}
		y++;
	}
	free(map);
}

rect* ft_brute_solve(p_map_tile** map, int size, int start_x, int start_y) 
{

	// I recommend making this into a recursive function for 25 lines
	// for optimizing it could also take the square size as an argument
	// and set max_rect as the cur largest square size
	// passing arguments x, y, width, height and size through a rect(struct)
	
	int x = start_x;
	int y = start_y;
	int max_size = 0; // square size
	int flag = 0;

	while (flag == 0) {

		if (start_y + max_size + 1 < size && start_x + max_size + 1 < size)
			max_size++;
		else
			break;

		y = start_y;
		while (y <= (start_y + max_size) && flag == 0)
		{
			x = start_x;
			while (x <= (start_x + max_size))
			{
				if (map[y][x]->type == OBSTACLE)
				{
					max_size--;
					flag++;
					break;
				}
				x++;
			}
			y++;
		}

	}

	//ft_draw_rect(map, start_x, start_y, max_size, max_size); was usefull for debugging
	return ft_create_rect(start_x, start_y, max_size, max_size);

}

int	main(void) 
{
	// turn this into a function and take in arguments width, height, map (as a string) and symbols[4] (string ex. ".ox")
	
	p_map_tile** map;
	int	size = 16; // change to width & height, map can be a rectangle
	int x = 0;
	int y = 0;
	struct s_rect* rect;
	struct s_rect* big;
	
	big = ft_create_rect(0,0,0,0);
	map = ft_create_map(size);
	
	while (y < size)
	{
		//printf("x %d, y %d\n", x, y);
		//ft_reset_map(map, size);
		rect = ft_brute_solve(map, size, x, y);
		//ft_print_map(map, size);

		if (big->size < rect->size)
		{
			free(big);
			big = rect;
		}

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
