#ifndef GAME_MAP_
#define GAME_MAP_

#include <whitgl/math.h>

typedef enum
{
	TILE_EMPTY,
	TILE_WALL,
} game_map_tile;

#define MAP_WIDTH (20)
#define MAP_HEIGHT (13)
#define NUM_TILES (MAP_WIDTH*MAP_HEIGHT)
typedef struct
{
	game_map_tile tiles[NUM_TILES];
} game_map;

game_map game_map_zero();
void game_map_draw(const game_map* map);
game_map_tile game_map_get_tile(const game_map* map, whitgl_ivec pos);
whitgl_ivec game_map_pos_from_index(whitgl_int i);
whitgl_int game_map_index_from_pos(whitgl_ivec v);

#endif // GAME_MAP_
