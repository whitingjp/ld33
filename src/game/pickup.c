#include "pickup.h"


#include <whitgl/sys.h>
#include <whitgl/sound.h>
#include <resource.h>

game_pickup game_pickup_spawn(whitgl_ivec pos)
{
	game_pickup pickup;
	pickup.active = true;
	pickup.pos = whitgl_ivec_to_fvec(pos);
	pickup.anim = 0;
	pickup.picked = false;
	return pickup;
}
game_pickup game_pickup_update(game_pickup pickup)
{
	if(!pickup.active)
		return pickup;
	pickup.anim += 1/24.0;
	if(pickup.anim > 1)
	{
		if(pickup.picked)
			pickup.active = false;
		else
			pickup.anim -= 1;
	}
	return pickup;
}
void game_pickup_draw(game_pickup pickup, whitgl_ivec camera)
{
	if(!pickup.active)
		return;
	whitgl_ivec draw_pos = whitgl_fvec_to_ivec(whitgl_fvec_scale(pickup.pos, whitgl_fvec_val(8)));
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sprite pickup_sprite = {IMAGE_SPRITES, {16,80}, {8,8}};
	whitgl_ivec frame = {pickup.anim*3.999, pickup.picked ? 1 : 0};
	whitgl_sys_draw_sprite(pickup_sprite, frame, draw_pos);
}

game_pickup game_pickup_picked(game_pickup pickup)
{
	if(pickup.picked) return pickup;
	whitgl_sound_play(SOUND_PLING, whitgl_randfloat()*0.5+1);
	pickup.picked = true;
	pickup.anim = 0;
	return pickup;
}

whitgl_faabb game_pickup_collider(game_pickup pickup)
{
	whitgl_faabb box = {pickup.pos, {pickup.pos.x+1, pickup.pos.y+1}};
	return box;
}
