#include "snake.h"

#include <whitgl/input.h>
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
	snake.size = 8;
	return snake;
}
game_snake game_snake_update(game_snake snake)
{
	whitgl_ivec move = whitgl_ivec_zero;
	if(whitgl_input_pressed(WHITGL_INPUT_UP))
		move.y--;
	if(whitgl_input_pressed(WHITGL_INPUT_RIGHT))
		move.x++;
	if(whitgl_input_pressed(WHITGL_INPUT_DOWN))
		move.y++;
	if(whitgl_input_pressed(WHITGL_INPUT_LEFT))
		move.x--;
	if(move.x != 0 && move.y != 0)
		move.y = 0;
	if(move.x == 0 && move.y == 0)
		return snake;
	whitgl_ivec new_pos = whitgl_ivec_add(snake.pos[0], move);
	whitgl_int i;
	for(i=1; i<snake.size; i++)
		if(whitgl_ivec_eq(new_pos, snake.pos[i]))
			return snake;
	for(i=snake.size-1; i>0; i--)
		snake.pos[i] = snake.pos[i-1];
	snake.pos[0] = new_pos;
	return snake;
}
void game_snake_draw(game_snake snake)
{
	whitgl_sprite snake_sprite = {IMAGE_SPRITES, {0,0}, {8,8}};

	whitgl_int i;
	for(i=0; i<snake.size; i++)
	{
		whitgl_ivec draw_pos = whitgl_ivec_scale(snake.pos[i], snake_sprite.size);
		whitgl_int flag = 0;

		if(i == 0)
		{
			whitgl_int dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[0],snake.pos[1]));
			flag += whitgl_fpow(2, dir);
		} else if(i == snake.size-1)
		{
			whitgl_int dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i-1],snake.pos[i]));
			flag += whitgl_fpow(2, dir);
		} else
		{
			whitgl_int in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i+1],snake.pos[i]));
			whitgl_int out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i-1],snake.pos[i]));
			flag += whitgl_fpow(2, in_dir);
			flag += whitgl_fpow(2, out_dir);
		}
		whitgl_ivec frame = whitgl_ivec_zero;
		switch(flag)
		{
			case  1: frame.x=0; frame.y=0; break;
			case  2: frame.x=1; frame.y=0; break;
			case  4: frame.x=2; frame.y=0; break;
			case  8: frame.x=3; frame.y=0; break;
			case  3: frame.x=0; frame.y=2; break;
			case  6: frame.x=1; frame.y=2; break;
			case 12: frame.x=2; frame.y=2; break;
			case  9: frame.x=3; frame.y=2; break;
			case  5: frame.x=0; frame.y=3; break;
			case 10: frame.x=1; frame.y=3; break;
			default: break;
		}
		if(i == snake.size-1)
			frame.y += 1;
		whitgl_sys_draw_sprite(snake_sprite, frame, draw_pos);
	}
}
