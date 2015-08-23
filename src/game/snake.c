#include "snake.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <resource.h>

game_snake game_snake_zero(whitgl_ivec start)
{
	game_snake snake;
	whitgl_int i;
	snake.size = 3;
	for(i=0; i<snake.size; i++)
	{
		whitgl_ivec pos = {start.x+i, start.y};
		snake.pos[i] = pos;
	}
	whitgl_ivec old_pos = {start.x+snake.size, start.y};
	snake.old_pos = old_pos;
	snake.t = 0.4;
	snake.dir = 3;
	snake.do_reverse = false;
	whitgl_ivec new_pos = {start.x-1, start.y};
	snake.new_pos = new_pos;
	snake.falling = false;
	snake.fall_timer = 0;
	snake.fall_speed = 0;
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
		if(game_map_get_tile(map, pos) == TILE_BEAM)
			valid = false;
	}
	return valid;
}

game_snake game_snake_update(game_snake snake, const game_map* map)
{
	whitgl_fvec joy = whitgl_input_joystick();
	if(whitgl_input_pressed(WHITGL_INPUT_A))
		snake.do_reverse = true;
	if(snake.do_reverse)
		joy = whitgl_fvec_zero;
	whitgl_int dir = whitgl_fvec_to_facing(joy);
	bool moving = joy.x != 0 || joy.y != 0;

	snake.falling = true;
	whitgl_int i;
	for(i=0 ;i<snake.size; i++)
	{
		whitgl_int j;
		for(j=0; j<4; j++)
		{
			whitgl_ivec test_pos = whitgl_ivec_add(snake.pos[i], whitgl_facing_to_ivec(j));
			if(j!=2 && (i==snake.size-1 || i==0))
				continue;
			if(j==2 && game_map_get_tile(map, test_pos) == TILE_WALL)
				snake.falling = false;
			if(j==2 && game_map_get_tile(map, test_pos) == TILE_PLATFORM)
				snake.falling = false;
		}
		if(game_map_get_tile(map, snake.pos[i]) == TILE_VINE)
			snake.falling = false;
	}
	if(game_map_get_tile(map, snake.new_pos) == TILE_WALL && _game_snake_move_valid(snake, snake.new_pos, map))
		snake.falling = false;
	if(snake.falling)
	{
		snake.fall_speed = whitgl_fclamp(snake.fall_speed+0.02, 0, 0.5);
		snake.fall_timer += snake.fall_speed;
		if(snake.fall_timer > 0.5)
		{
			for(i=0 ;i<MAX_SEGMENTS; i++)
			{
				snake.pos[i].y++;
			}
			snake.new_pos.y++;
			snake.fall_timer -= 1;
		}
	} else
	{
		snake.fall_timer = 0;
		snake.fall_speed = 0;
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
	whitgl_float inc = forward ? 1.0/8.0 : -1.0/4.0;
	whitgl_float old_snake_t = snake.t;
	snake.t = whitgl_fclamp(snake.t+inc, 0, 1);

	if(old_snake_t <= 0.5 && snake.t > 0.5)
	{
		snake.old_pos = snake.pos[snake.size-1];
		for(i=MAX_SEGMENTS-1; i>0; i--)
			snake.pos[i] = snake.pos[i-1];
		snake.pos[0] = snake.new_pos;
		if(whitgl_randfloat() > 0.5)
			whitgl_sound_play(SOUND_SLICK00+whitgl_randint(6), whitgl_randfloat()*0.2+0.9);
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
			new_snake.old_pos = snake.new_pos;
			new_snake.new_pos = snake.old_pos;
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

#define VALID (true)
#define INVALID (false)
#define WIDE (true)
#define NARR (false)
#define HORI (true)
#define VERT (false)
typedef struct
{
	whitgl_bool valid;
	whitgl_bool wide;
	whitgl_bool horizontal;
	whitgl_ivec sprite_offset;
	whitgl_ivec draw_offset;
} snake_anim_data;
const snake_anim_data tail_data_zero = {INVALID, NARR, VERT, {0,0}, {0,0}};

const snake_anim_data all_snake_data[16] =
{
	{  VALID, NARR, HORI, { 0, 0}, { 0, 0}}, // 0, 0
	{  VALID, WIDE, VERT, {48,64}, {-8, 0}}, // 1, 0
	{INVALID, NARR, VERT, { 0, 0}, { 0, 0}}, // 2, 0  INVALID
	{  VALID, WIDE, VERT, {48,32}, { 0, 0}}, // 3, 0
	{  VALID, NARR, HORI, { 0,32}, { 0, 0}}, // 0, 1
	{  VALID, WIDE, HORI, { 0,16}, {-8, 0}}, // 1, 1
	{  VALID, NARR, HORI, { 0,64}, { 0,-8}}, // 2, 1
	{INVALID, NARR, VERT, { 0, 0}, { 0, 0}}, // 3, 1  INVALID
	{INVALID, NARR, VERT, { 0, 0}, { 0, 0}}, // 0, 2  INVALID
	{  VALID, WIDE, VERT, {32,32}, {-8, 0}}, // 1, 2
	{  VALID, NARR, HORI, {32, 0}, { 0,-8}}, // 2, 2
	{  VALID, WIDE, VERT, {32,64}, { 0, 0}}, // 3, 2
	{  VALID, NARR, HORI, { 0,80}, { 0, 0}}, // 0, 3
	{INVALID, NARR, VERT, { 0, 0}, { 0, 0}}, // 1, 3 INVALID
	{  VALID, NARR, HORI, { 0,48}, { 0,-8}}, // 2, 3
	{  VALID, WIDE, HORI, { 0,24}, { 0, 0}}, // 3, 3
};

void game_snake_draw(game_snake snake, whitgl_ivec camera)
{
	whitgl_sprite snake_sprite = {IMAGE_SPRITES, {0,0}, {8,8}};
	whitgl_int i;
	for(i=1; i<snake.size-1; i++)
	{
		if(snake.t < 0.5 && i==snake.size-2)
			continue;
		whitgl_ivec draw_pos = whitgl_ivec_scale(snake.pos[i], snake_sprite.size);
		draw_pos = whitgl_ivec_add(draw_pos, camera);
		whitgl_int flag = 0;
		whitgl_int in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i+1],snake.pos[i]));
		whitgl_int out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[i-1],snake.pos[i]));
		flag += whitgl_fpow(2, in_dir);
		flag += whitgl_fpow(2, out_dir);
		whitgl_ivec frame = _game_snake_flags_to_frame(flag);
		draw_pos.y += snake.fall_timer*8;
		whitgl_sys_draw_sprite(snake_sprite, frame, draw_pos);
	}

	whitgl_ivec draw_pos;
	whitgl_ivec frame;

	whitgl_sprite head_sprite = {IMAGE_SPRITES, {128,0}, {8,8}};
	whitgl_int head_in_dir;
	whitgl_int head_out_dir;
	if(snake.t < 0.5)
	{
		draw_pos = whitgl_ivec_scale(snake.pos[0], snake_sprite.size);

		head_in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[0],snake.new_pos));
		head_out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[1],snake.pos[0]));
	}
	else
	{
		draw_pos = whitgl_ivec_scale(snake.pos[1], snake_sprite.size);
		head_in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[1],snake.pos[0]));
		head_out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[2],snake.pos[1]));
	}
	whitgl_int head_data_index = head_in_dir+head_out_dir*4;

	draw_pos.y += snake.fall_timer*8;
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_int iframe_head = (1-snake.t)*3.99;
	frame = whitgl_ivec_zero;
	snake_anim_data tdata_head = all_snake_data[head_data_index];

	if(tdata_head.wide)
		head_sprite.size.x *= 2;
	else
		head_sprite.size.y *= 2;
	if(tdata_head.horizontal)
		frame.x = iframe_head;
	else
		frame.y = iframe_head;
	head_sprite.top_left = whitgl_ivec_add(head_sprite.top_left, tdata_head.sprite_offset);
	draw_pos = whitgl_ivec_add(draw_pos, tdata_head.draw_offset);

	if(tdata_head.valid)
		whitgl_sys_draw_sprite(head_sprite, frame, draw_pos);

	whitgl_sprite tail_sprite = {IMAGE_SPRITES, {64,0}, {8,8}};
	whitgl_int in_dir;
	whitgl_int out_dir;

	if(snake.t < 0.5)
	{
		draw_pos = whitgl_ivec_scale(snake.pos[snake.size-2], snake_sprite.size);
		in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-2],snake.pos[snake.size-1]));
		out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-3],snake.pos[snake.size-2]));
	}
	else
	{
		draw_pos = whitgl_ivec_scale(snake.pos[snake.size-1], snake_sprite.size);

		in_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-1],snake.old_pos));
		out_dir = whitgl_ivec_to_facing(whitgl_ivec_sub(snake.pos[snake.size-2],snake.pos[snake.size-1]));
	}
	whitgl_int data_index = in_dir+out_dir*4;

	draw_pos.y += snake.fall_timer*8;
	draw_pos = whitgl_ivec_add(draw_pos, camera);
	whitgl_int iframe = snake.t*3.99;
	frame = whitgl_ivec_zero;
	snake_anim_data tdata = all_snake_data[data_index];

	if(tdata.wide)
		tail_sprite.size.x *= 2;
	else
		tail_sprite.size.y *= 2;
	if(tdata.horizontal)
		frame.x = iframe;
	else
		frame.y = iframe;
	tail_sprite.top_left = whitgl_ivec_add(tail_sprite.top_left, tdata.sprite_offset);
	draw_pos = whitgl_ivec_add(draw_pos, tdata.draw_offset);

	if(tdata.valid)
		whitgl_sys_draw_sprite(tail_sprite, frame, draw_pos);

}

whitgl_bool game_snake_collide(game_snake snake, whitgl_faabb box)
{
	whitgl_int j;
	for(j=0; j<snake.size; j++)
	{
		whitgl_faabb snake_col;
		snake_col.a = whitgl_ivec_to_fvec(snake.pos[j]);
		snake_col.b = whitgl_fvec_add(snake_col.a, whitgl_fvec_val(1));
		if(whitgl_faabb_intersects(snake_col, box))
			return true;
	}
	return false;
}
