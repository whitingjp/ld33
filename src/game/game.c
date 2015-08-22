#include "game.h"

#include <whitgl/sys.h>
#include <resource.h>

game_game game_game_zero()
{
	game_game game;
	game.snake = game_snake_zero();
	return game;
}

game_game game_update(game_game game, const game_map* map)
{
	game.snake = game_snake_update(game.snake, map);
	return game;
}

void game_draw(game_game game, const game_map* map, whitgl_ivec screen_size)
{
	whitgl_iaabb screen = {whitgl_ivec_zero, screen_size};
	whitgl_sys_color background = {0x5a, 0x0f, 0x5f, 0xff};
	whitgl_sys_draw_iaabb(screen, background);
	game_map_draw(map);
	game_snake_draw(game.snake);
}
