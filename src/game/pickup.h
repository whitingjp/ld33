#ifndef GAME_PICKUP_
#define GAME_PICKUP_

#include <game/map.h>

typedef struct
{
	whitgl_bool active;
	whitgl_fvec pos;
	whitgl_float anim;
	whitgl_bool picked;
} game_pickup;
static const game_pickup game_pickup_zero = {false, {0,0}, 0, false};
game_pickup game_pickup_spawn(whitgl_ivec pos);
game_pickup game_pickup_update(game_pickup pickup);
void game_pickup_draw(game_pickup pickup, whitgl_ivec camera);
game_pickup game_pickup_picked(game_pickup pickup);
whitgl_faabb game_pickup_collider(game_pickup pickup);

#endif // GAME_PICKUP_
