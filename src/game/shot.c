#include "shot.h"

#include <whitgl/sys.h>
#include <resource.h>

game_shot game_shot_spawn(whitgl_fvec pos, whitgl_bool facing_left, whitgl_int owner)
{
	game_shot shot;
	shot.active = true;
	shot.pos = pos;
	shot.speed = whitgl_fvec_zero;
	shot.speed.x = facing_left ? -1 : 1;
	shot.speed.x *= 0.2;
	shot.owner = owner;
	return shot;
}
game_shot game_shot_update(game_shot shot, const game_map* map)
{
	if(!shot.active)
		return shot;
	shot.pos = whitgl_fvec_add(shot.pos, shot.speed);
	(void)map;
	return shot;
}
void game_shot_draw(game_shot shot, whitgl_ivec camera)
{
	if(!shot.active)
		return;
	whitgl_sprite shot_sprite = {IMAGE_SPRITES, {16,72}, {8,8}};
	whitgl_ivec draw_pos = whitgl_fvec_to_ivec(whitgl_fvec_scale(shot.pos, whitgl_fvec_val(8)));
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(shot_sprite, whitgl_ivec_zero, draw_pos);
}

whitgl_faabb game_shot_collider(game_shot shot)
{
	whitgl_faabb col_box = {{2/8.0,5/8.0},{6/8.0,6/8.0}};
	whitgl_faabb collider = whitgl_faabb_add(col_box, shot.pos);
	return collider;
}
