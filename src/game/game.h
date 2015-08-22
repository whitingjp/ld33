#ifndef GAME_GAME_
#define GAME_GAME_

#include <whitgl/math.h>
#include <game/snake.h>

typedef struct
{
	game_snake snake;
} game_game;
game_game game_game_zero();

game_game game_update(game_game game);
void game_draw(game_game game, whitgl_ivec screen_size);

#endif // GAME_GAME_
