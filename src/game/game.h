#ifndef GAME_GAME_
#define GAME_GAME_

#include <whitgl/math.h>
#include <game/map.h>
#include <game/snake.h>

typedef struct
{
	game_snake snake;
} game_game;
game_game game_game_zero(const game_map* map);

game_game game_update(game_game game, const game_map* map);
void game_draw(game_game game);

#endif // GAME_GAME_
