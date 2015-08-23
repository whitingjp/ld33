#ifndef GAME_BLOOD_
#define GAME_BLOOD_

#include <game/map.h>

typedef struct
{
	whitgl_bool active;
	whitgl_fvec pos;
	whitgl_fvec speed;
	whitgl_ivec frame;
	whitgl_bool landed;
} game_blood;
static const game_blood game_blood_zero = {false, {0,0}, {0,0}, {0,0}, false};
game_blood game_blood_spawn(whitgl_fvec pos, whitgl_fvec speed);
game_blood game_blood_update(game_blood blood, const game_map* map);
void game_blood_draw(game_blood blood, whitgl_ivec camera);

#endif // GAME_BLOOD_
