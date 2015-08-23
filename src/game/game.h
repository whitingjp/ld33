#ifndef GAME_GAME_
#define GAME_GAME_

#include <whitgl/math.h>
#include <game/blood.h>
#include <game/map.h>
#include <game/snake.h>
#include <game/walker.h>

#define NUM_WALKERS (128)
#define NUM_BLOOD (1024)
typedef struct
{
	game_snake snake;
	game_walker walkers[NUM_WALKERS];
	game_blood blood[NUM_BLOOD];
	whitgl_int next_blood;
	whitgl_fvec fcamera;
	whitgl_ivec camera;
} game_game;
game_game game_game_zero(const game_map* map, whitgl_ivec screen_size);

game_game game_update(game_game game, const game_map* map, whitgl_ivec screen_size);
void game_draw(game_game game, const game_map* map);
void game_draw_over(game_game game);

#endif // GAME_GAME_
