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
	snake.size = 16;
	snake.t = 1;
	snake.old_pos = snake.pos[snake.size-1];
	return snake;
}
game_snake game_snake_update(game_snake snake)
{
	whitgl_int i;
	if(snake.t < 1)
	{
		// if(whitgl_input_pressed(WHITGL_INPUT_A))
			snake.t = whitgl_fclamp(snake.t+1.0/8, 0, 1);
		return snake;
	}
	whitgl_ivec move = whitgl_ivec_zero;
	if(whitgl_input_down(WHITGL_INPUT_UP))
		move.y--;
	if(whitgl_input_down(WHITGL_INPUT_RIGHT))
		move.x++;
	if(whitgl_input_down(WHITGL_INPUT_DOWN))
		move.y++;
	if(whitgl_input_down(WHITGL_INPUT_LEFT))
		move.x--;
	if(move.x != 0 && move.y != 0)
		move.y = 0;
	if(move.x == 0 && move.y == 0)
		return snake;
	whitgl_ivec new_pos = whitgl_ivec_add(snake.pos[0], move);
	for(i=1; i<snake.size-1; i++)
		if(whitgl_ivec_eq(new_pos, snake.pos[i]))
			return snake;
	snake.old_pos = snake.pos[snake.size-1];
	for(i=snake.size-1; i>0; i--)
		snake.pos[i] = snake.pos[i-1];
	snake.pos[0] = new_pos;
	snake.t = 0.25;
	return snake;
}

whitgl_ivec _game_snake_flags_to_frame(whitgl_int flags)
{
	whitgl_ivec frame = whitgl_ivec_zero;
	switch(flags)
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
	return frame;
}

void game_snake_draw(game_snake snake)
{
	whitgl_sprite snake_sprite = {IMAGE_SPRITES, {0,0}, {8,8}};

	whitgl_int i;
	for(i=snake.size-2; i>=0; i--)
	{
		whitgl_ivec draw_pos = whitgl_ivec_scale(snake.pos[i], snake_sprite.size);
		whitgl_int flag = 0;

		if(i == 0)
		{
			whitgl_int dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[0],snake.pos[1]));
			whitgl_fvec sub_pos = whitgl_fvec_interpolate(whitgl_ivec_to_fvec(snake.pos[1]), whitgl_ivec_to_fvec(snake.pos[0]), snake.t);
			sub_pos = whitgl_fvec_sub(sub_pos, whitgl_ivec_to_fvec(snake.pos[0]));
			sub_pos = whitgl_fvec_scale(sub_pos, whitgl_ivec_to_fvec(snake_sprite.size));
			draw_pos = whitgl_ivec_add(draw_pos, whitgl_fvec_to_ivec(sub_pos));
			flag += whitgl_fpow(2, dir);
		} else
		{
			whitgl_int in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i+1],snake.pos[i]));
			whitgl_int out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i-1],snake.pos[i]));
			flag += whitgl_fpow(2, in_dir);
			flag += whitgl_fpow(2, out_dir);
		}
		whitgl_ivec frame = _game_snake_flags_to_frame(flag);
		if(i == snake.size-1)
			frame.y += 1;
		whitgl_sys_draw_sprite(snake_sprite, frame, draw_pos);
	}
	whitgl_int tail_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-1], snake.old_pos));
	whitgl_ivec tail_pos = whitgl_ivec_scale(snake.pos[snake.size-1], snake_sprite.size);
	whitgl_sprite tail_sprite = {IMAGE_SPRITES, {0,64}, {8,8}};
	switch(tail_dir)
	{
		case 0:
			tail_sprite.size.y = 16;
			break;
		case 1:
			tail_pos.x -= 8;
			tail_sprite.size.x = 16;
			tail_sprite.top_left.y += 16;
			break;
		case 2:
			tail_pos.y -= 8;
			tail_sprite.top_left.x += 32;
			tail_sprite.size.y = 16;
			break;
		case 3:
			tail_sprite.size.x = 16;
			tail_sprite.top_left.y += 24;
			break;
	}
	whitgl_ivec tail_frame = {snake.t*3.9999, 0};
	whitgl_sys_draw_sprite(tail_sprite, tail_frame, tail_pos);
}
