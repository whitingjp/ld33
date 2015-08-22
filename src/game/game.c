#include "game.h"

#include <whitgl/sys.h>
#include <resource.h>

game_game game_game_zero(const game_map* map)
{
	game_game game;
	whitgl_ivec snake_spawn = {1,1};
	whitgl_int i;
	for(i=0; i<NUM_TILES; i++)
	{
		if(map->tiles[i] == TILE_SPAWN)
			snake_spawn = game_map_pos_from_index(i);
	}
	game.snake = game_snake_zero(snake_spawn);
	return game;
}

game_game game_update(game_game game, const game_map* map)
{
	game.snake = game_snake_update(game.snake, map);
	return game;
}

void game_draw(game_game game)
{
	game_snake_draw(game.snake);
}
