#ifndef GAME_SHOT_
#define GAME_SHOT_

#include <game/map.h>

typedef struct
{
	whitgl_bool active;
	whitgl_fvec pos;
	whitgl_fvec speed;
} game_shot;
static const game_shot game_shot_zero = {false, {0,0}, {0,0}};
game_shot game_shot_spawn(whitgl_fvec pos, whitgl_bool facing_left);
game_shot game_shot_update(game_shot shot, const game_map* map);
void game_shot_draw(game_shot shot, whitgl_ivec camera);
whitgl_faabb game_shot_collider(game_shot shot);

#endif // GAME_SHOT_
