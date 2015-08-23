#ifndef GAME_MAP_
#define GAME_MAP_

#include <whitgl/math.h>

typedef enum
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_VINE,
	TILE_VINE_WALL,
	TILE_SPAWN,
	TILE_WALKER,
	TILE_PICKUP,
	TILE_MAX,
} game_map_tile;

#define SCREEN_W (20)
#define SCREEN_H (11)
#define MAP_WIDTH (SCREEN_W*8)
#define MAP_HEIGHT (SCREEN_H*2)
#define NUM_TILES (MAP_WIDTH*MAP_HEIGHT)

typedef struct
{
	game_map_tile tiles[NUM_TILES];
	char decorations[NUM_TILES];
} game_map;

game_map game_map_zero();
void game_map_draw(const game_map* map, whitgl_bool editor, whitgl_ivec screen_size, whitgl_ivec camera);
game_map_tile game_map_get_tile(const game_map* map, whitgl_ivec pos);
char game_map_get_decoration(const game_map* map, whitgl_ivec pos);
whitgl_ivec game_map_pos_from_index(whitgl_int i);
whitgl_int game_map_index_from_pos(whitgl_ivec v);
whitgl_bool game_map_collide(const game_map* map, whitgl_faabb box);

#endif // GAME_MAP_
