#include "blood.h"

#include <whitgl/sys.h>
#include <resource.h>

game_blood game_blood_spawn(whitgl_fvec pos, whitgl_fvec speed)
{
	game_blood blood;
	blood.active = true;
	blood.pos.x = whitgl_randfloat()-0.5+pos.x;
	blood.pos.y = whitgl_randfloat()-0.5+pos.y;
	blood.speed.x = speed.x+(whitgl_randfloat()-0.5)/2;
	blood.speed.y = speed.y+(whitgl_randfloat()-0.5)/2;
	blood.frame.x = whitgl_randint(4);
	blood.frame.y = whitgl_randint(2);
	blood.frame.y = 0;
	blood.landed = false;
	return blood;
}
game_blood game_blood_update(game_blood blood, const game_map* map)
{
	if(!blood.active || blood.landed)
		return blood;
	blood.pos = whitgl_fvec_add(blood.pos, blood.speed);
	blood.speed.y = whitgl_fclamp(blood.speed.y+0.01, 0, 2);
	whitgl_fvec mid = whitgl_fvec_add(blood.pos, whitgl_fvec_val(0.5));
	game_map_tile tile = game_map_get_tile(map, whitgl_fvec_to_ivec(mid));
	if(tile == TILE_WALL && whitgl_randfloat() > 0.9)
		blood.landed = true;
	return blood;
}
void game_blood_draw(game_blood blood, whitgl_ivec camera)
{
	if(!blood.active)
		return;
	whitgl_sprite blood_sprite = {IMAGE_SPRITES,{32,16}, {8,8}};
	whitgl_ivec draw_pos = whitgl_fvec_to_ivec(whitgl_fvec_scale(blood.pos, whitgl_fvec_val(8)));
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_sys_draw_sprite(blood_sprite, blood.frame, draw_pos);
}
