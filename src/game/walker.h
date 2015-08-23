#ifndef GAME_WALKER_
#define GAME_WALKER_

#include <game/map.h>

typedef struct
{
	whitgl_bool active;
	whitgl_fvec pos;
	whitgl_float speed;
	whitgl_float anim;
	whitgl_float wait;
} game_walker;

static const game_walker game_walker_zero = {false, {0,0}, 0.04, 0, 0};
game_walker game_walker_spawn(whitgl_ivec pos);
game_walker game_walker_update(game_walker walker, const game_map* map);
void game_walker_draw(game_walker walker);
whitgl_faabb game_walker_collider(game_walker walker);

#endif // GAME_WALKER_
