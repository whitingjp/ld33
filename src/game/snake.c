#include "snake.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
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
	snake.t = 0;
	snake.dir = 3;
	snake.do_reverse = false;
	snake.new_pos = snake.pos[0];
	snake.falling = false;
	snake.fall_timer = 0;
	return snake;
}

whitgl_bool _game_snake_move_valid(game_snake snake, whitgl_ivec pos, const game_map* map)
{
	bool valid = true;
	whitgl_int i;
	for(i=1; i<snake.size-1; i++)
	{
		if(whitgl_ivec_eq(pos, snake.pos[i]))
			valid = false;
		if(game_map_get_tile(map, pos) == TILE_WALL)
			valid = false;
	}
	return valid;
}

game_snake game_snake_update(game_snake snake, const game_map* map)
{
	whitgl_fvec joy = whitgl_input_joystick();
	whitgl_int dir = whitgl_fvec_to_facing(joy);
	bool moving = joy.x != 0 || joy.y != 0;
	if(whitgl_input_pressed(WHITGL_INPUT_A))
		snake.do_reverse = true;

	snake.falling = true;
	whitgl_int i;
	for(i=0 ;i<snake.size; i++)
	{
		whitgl_int j;
		for(j=0; j<4; j++)
		{
			whitgl_ivec test_pos = whitgl_ivec_add(snake.pos[i], whitgl_facing_to_ivec(j));
			if(game_map_get_tile(map, test_pos) == TILE_WALL)
				snake.falling = false;
		}
	}
	if(game_map_get_tile(map, snake.new_pos) == TILE_WALL)
		snake.falling = false;
	if(snake.falling)
	{
		snake.fall_timer += 0.25;
		if(snake.fall_timer > 0.5)
		{
			for(i=0 ;i<snake.size; i++)
			{
				snake.pos[i].y++;
			}
			snake.new_pos.y++;
			snake.fall_timer -= 1;
		}
	} else
	{
		snake.fall_timer = 0;
	}

	if(!moving && !snake.do_reverse )
	{
		return snake;
	}
	bool forward = true;
	if(dir != snake.dir && snake.t < 0.5)
		forward = false;
	if(!_game_snake_move_valid(snake, snake.new_pos, map))
		forward = false;
	whitgl_float inc = forward ? 0.2 : -2;
	whitgl_float old_snake_t = snake.t;
	snake.t = whitgl_fclamp(snake.t+inc, 0, 1);

	if(old_snake_t <= 0.5 && snake.t > 0.5)
	{
		for(i=snake.size-1; i>0; i--)
			snake.pos[i] = snake.pos[i-1];
		snake.pos[0] = snake.new_pos;
	}
	if(snake.t == 0 || snake.t == 1)
	{
		if(snake.do_reverse)
		{
			game_snake new_snake = snake;
			for(i=0; i<snake.size; i++)
				new_snake.pos[i] = snake.pos[snake.size-1-i];
			new_snake.dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-1], snake.pos[snake.size-2]));
			new_snake.do_reverse = false;
			snake = new_snake;
			snake.t = 1;
		}
		whitgl_ivec new_pos = whitgl_ivec_add(snake.pos[0], whitgl_facing_to_ivec(dir));
		if(_game_snake_move_valid(snake, new_pos, map))
		{
			snake.new_pos = new_pos;
			snake.dir = dir;
			snake.t = 0.1;
		}
	}
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
	for(i=1; i<snake.size-1; i++)
	{
		whitgl_ivec draw_pos = whitgl_ivec_scale(snake.pos[i], snake_sprite.size);
		whitgl_int flag = 0;
		whitgl_int in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i+1],snake.pos[i]));
		whitgl_int out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i-1],snake.pos[i]));
		flag += whitgl_fpow(2, in_dir);
		flag += whitgl_fpow(2, out_dir);
		whitgl_ivec frame = _game_snake_flags_to_frame(flag);
		draw_pos.y += snake.fall_timer*8;
		whitgl_sys_draw_sprite(snake_sprite, frame, draw_pos);
	}
	whitgl_float t = snake.t;
	whitgl_ivec draw_pos = whitgl_ivec_scale(snake.pos[0], snake_sprite.size);
	draw_pos.y += snake.fall_timer*8;
	if(t <= 0.5)
	{
		whitgl_int in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[1],snake.pos[0]));
		whitgl_int out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.new_pos,snake.pos[0]));
		whitgl_int underlay_flag = whitgl_fpow(2, in_dir) + whitgl_fpow(2, out_dir);
		whitgl_ivec underlay_frame = _game_snake_flags_to_frame(underlay_flag);

		whitgl_sys_draw_sprite(snake_sprite, underlay_frame, draw_pos);
	}

	whitgl_fvec off = whitgl_facing_to_fvec(snake.dir);
	if(t > 0.5)
	{
		off = whitgl_fvec_inverse(off);
		t = 1-t;
	}
	off = whitgl_fvec_scale(off, whitgl_ivec_to_fvec(snake_sprite.size));
	off = whitgl_fvec_scale(off, whitgl_fvec_val(t));
	draw_pos = whitgl_ivec_add(draw_pos, whitgl_fvec_to_ivec(off));
	whitgl_ivec frame = _game_snake_flags_to_frame(whitgl_fpow(2, snake.dir));
	whitgl_sys_draw_sprite(snake_sprite, frame, draw_pos);

	t = snake.t;
	whitgl_int in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-1],snake.pos[snake.size-2]));
	whitgl_int out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-2],snake.pos[snake.size-1]));
	draw_pos = whitgl_ivec_scale(snake.pos[snake.size-1], snake_sprite.size);
	draw_pos.y += snake.fall_timer*8;
	if(t > 0.5)
	{
		whitgl_int underlay_flag = whitgl_fpow(2, in_dir) + whitgl_fpow(2, out_dir);
		whitgl_ivec underlay_frame = _game_snake_flags_to_frame(underlay_flag);

		whitgl_sys_draw_sprite(snake_sprite, underlay_frame, draw_pos);
	}
	off = whitgl_facing_to_fvec(out_dir);
	if(t > 0.5)
	{
		off = whitgl_fvec_inverse(off);
		t = 1.0-t;
	}
	off = whitgl_fvec_scale(off, whitgl_ivec_to_fvec(snake_sprite.size));
	off = whitgl_fvec_scale(off, whitgl_fvec_val(t));
	draw_pos = whitgl_ivec_add(draw_pos, whitgl_fvec_to_ivec(off));


	out_dir = whitgl_iwrap(out_dir+2, 0, 4);
	frame = _game_snake_flags_to_frame(whitgl_fpow(2, out_dir));
	frame.x += 4;
	frame.y += 1;
	whitgl_sys_draw_sprite(snake_sprite, frame, draw_pos);
}
