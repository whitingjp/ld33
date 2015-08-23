#ifndef GAME_GAME_
#define GAME_GAME_

#include <whitgl/math.h>
#include <game/blood.h>
#include <game/map.h>
#include <game/pickup.h>
#include <game/shot.h>
#include <game/snake.h>

typedef struct
{
	whitgl_bool active;
	whitgl_fvec pos;
	whitgl_float speed;
	whitgl_float anim;
	whitgl_float wait;
	whitgl_bool will_shoot;
	whitgl_bool shooting;
	whitgl_int index;
} game_walker;

static const game_walker game_walker_zero = {false, {0,0}, 0.04, 0, 0, false, false, 0};

#define NUM_WALKERS (256)
#define NUM_BLOOD (1024)
#define NUM_SHOTS (256)
#define NUM_PICKUPS (256)
typedef struct
{
	game_snake snake;
	game_walker walkers[NUM_WALKERS];
	game_blood blood[NUM_BLOOD];
	whitgl_int next_blood;
	game_shot shots[NUM_SHOTS];
	whitgl_int next_shot;
	game_pickup pickups[NUM_PICKUPS];
	whitgl_fvec fcamera;
	whitgl_ivec camera;
} game_game;
game_game game_game_zero(const game_map* map, whitgl_ivec screen_size);

game_game game_update(game_game game, const game_map* map, whitgl_ivec screen_size);
void game_draw(game_game game, const game_map* map);
void game_draw_over(game_game game);

#endif // GAME_GAME_
