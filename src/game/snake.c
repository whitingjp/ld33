#include "snake.h"

#include <whitgl/sys.h>

#include <resource.h>

game_snake game_snake_zero()
{
	game_snake snake;
	whitgl_int i;
	for(i=0; i<MAX_SEGMENTS; i++)
	{
		whitgl_ivec pos = {i+3, 2};
		snake.pos[i] = pos;
	}
	snake.size = 3;
	return snake;
}
game_snake game_snake_update(game_snake snake)
{
	return snake;
}
void game_snake_draw(game_snake snake)
{
	whitgl_sprite snake_head = {IMAGE_SPRITES, {0,0}, {8,8}};
	whitgl_ivec frame = {0,0};
	whitgl_ivec draw_pos = whitgl_ivec_scale(snake.pos[0], snake_head.size);
	whitgl_sys_draw_sprite(snake_head, frame, draw_pos);
}
