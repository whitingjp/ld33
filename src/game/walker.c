#include "walker.h"

#include <whitgl/sys.h>
#include <resource.h>

game_walker game_walker_spawn(whitgl_ivec pos)
{
	game_walker walker = game_walker_zero;
	walker.pos = whitgl_ivec_to_fvec(pos);
	walker.active = true;
	return walker;
}
game_walker game_walker_update(game_walker walker, const game_map* map)
{
	if(!walker.active)
		return walker;

	walker.pos.x += walker.speed;
	whitgl_faabb box = {walker.pos, {walker.pos.x+1, walker.pos.y+0.5}};
	whitgl_bool collided = game_map_collide(map, box);
	if(collided)
		walker.speed *= -1;
	walker.anim = whitgl_fwrap(walker.anim+0.05, 0, 1);
	return walker;
}
void game_walker_draw(game_walker walker)
{
	if(!walker.active)
		return;
	whitgl_sprite walker_sprite = {IMAGE_SPRITES, {0,72}, {8,8}};
	whitgl_ivec draw_pos = whitgl_fvec_to_ivec(whitgl_fvec_scale(walker.pos, whitgl_ivec_to_fvec(walker_sprite.size)));
	whitgl_ivec frame = {walker.anim*2, walker.speed > 0 ? 0 : 1};
	whitgl_sys_draw_sprite(walker_sprite, frame, draw_pos);
}
