#include "game.h"

#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <resource.h>

whitgl_fvec _game_camera_target(game_game game, whitgl_ivec screen_size)
{
	whitgl_fvec target = whitgl_fvec_zero;
	whitgl_int i;
	for(i=0; i<game.snake.size; i++)
		target = whitgl_fvec_add(target, whitgl_ivec_to_fvec(game.snake.pos[i]));
	target = whitgl_fvec_divide(target, whitgl_fvec_val(game.snake.size));
	whitgl_fvec screen = whitgl_fvec_divide(whitgl_ivec_to_fvec(screen_size), whitgl_fvec_val(8*2));
	target = whitgl_fvec_sub(target, screen);
	whitgl_faabb bounds = {whitgl_fvec_zero, {MAP_WIDTH-screen_size.x/8, MAP_HEIGHT-screen_size.y/8}};
	target = whitgl_fvec_bound(target, bounds);
	target = whitgl_fvec_scale(target, whitgl_fvec_val(8));
	return target;
}

game_game game_game_zero(const game_map* map, whitgl_ivec screen_size)
{
	game_game game;
	whitgl_ivec snake_spawn = {1,1};
	whitgl_int num_walker = 0;
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_zero;
	for(i=0; i<NUM_BLOOD; i++)
		game.blood[i] = game_blood_zero;
	game.next_blood = 0;
	for(i=0; i<NUM_SHOTS; i++)
		game.shots[i] = game_shot_zero;
	game.next_shot = 0;
	for(i=0; i<NUM_TILES; i++)
	{
		whitgl_ivec pos = game_map_pos_from_index(i);
		if(map->tiles[i] == TILE_SPAWN)
			snake_spawn = pos;
		if(map->tiles[i] == TILE_WALKER)
			game.walkers[num_walker++] = game_walker_spawn(pos);
	}
	game.snake = game_snake_zero(snake_spawn);
	game.fcamera = whitgl_fvec_inverse(_game_camera_target(game, screen_size));
	game.camera = whitgl_fvec_to_ivec(game.fcamera);
	return game;
}

game_game game_update(game_game game, const game_map* map, whitgl_ivec screen_size)
{
	game.snake = game_snake_update(game.snake, map);
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_update(game.walkers[i], &game.snake, map);
	for(i=0; i<NUM_BLOOD; i++)
		game.blood[i] = game_blood_update(game.blood[i], map);
	for(i=0; i<NUM_SHOTS; i++)
		game.shots[i] = game_shot_update(game.shots[i], map);

	whitgl_fvec target = _game_camera_target(game, screen_size);
	game.fcamera = whitgl_fvec_interpolate(game.fcamera, whitgl_fvec_inverse(target), 0.08);
	game.camera = whitgl_fvec_to_ivec(game.fcamera);

	for(i=0; i<NUM_WALKERS; i++)
	{
		if(!game.walkers[i].active)
			continue;
		whitgl_faabb walk_col = game_walker_collider(game.walkers[i]);
		if(game_snake_collide(game.snake, walk_col))
		{
			game.walkers[i].active = false;
			game.snake.size++;
			game.snake.old_pos = game.snake.pos[game.snake.size];
			whitgl_sound_play(SOUND_SPLAT00+whitgl_randint(6), whitgl_randfloat()/5+0.9);
			whitgl_int j;
			for(j=0; j<24; j++)
			{
				whitgl_fvec spawn_point = {game.walkers[i].pos.x+0.5,game.walkers[i].pos.y+0.5};
				whitgl_fvec speed = whitgl_fvec_scale(whitgl_facing_to_fvec(game.snake.dir), whitgl_fvec_val(0.3));
				speed.y -= 0.5;
				game.blood[game.next_blood] = game_blood_spawn(spawn_point, speed);
				game.next_blood = whitgl_iwrap(game.next_blood+1, 0, NUM_BLOOD);
			}
		}
	}

	for(i=0; i<NUM_WALKERS; i++)
	{
		if(!game.walkers[i].active)
			continue;
		if(!game.walkers[i].shooting)
			continue;
		game.shots[game.next_shot] = game_shot_spawn(game.walkers[i].pos, game.walkers[i].speed < 0);
		game.next_shot = whitgl_iwrap(game.next_shot+1, 0, NUM_SHOTS);
	}

	if(whitgl_randfloat() > 1-game.snake.size*0.005)
	{
		whitgl_int segment = whitgl_randint(game.snake.size);
		whitgl_fvec spawn_point = {game.snake.pos[segment].x+0.5,game.snake.pos[segment].y+0.5};
		whitgl_fvec speed = whitgl_fvec_zero;
		speed.y -= 0.3;
		game.blood[game.next_blood] = game_blood_spawn(spawn_point, speed);
		game.next_blood = whitgl_iwrap(game.next_blood+1, 0, NUM_BLOOD);
	}

	return game;
}

void game_draw(game_game game, const game_map* map)
{
	game_snake_draw(game.snake, map, game.camera);
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game_walker_draw(game.walkers[i], game.camera);

	// for(i=0; i<NUM_WALKERS; i++)
	// {
	// 	if(!game.walkers[i].active)
	// 		continue;
	// 	whitgl_faabb walk_col = game_walker_collider(game.walkers[i]);
	// 	whitgl_iaabb draw_rect = whitgl_faabb_to_iaabb(whitgl_faabb_scale(walk_col, whitgl_fvec_val(8)));
	// 	whitgl_sys_color col = {0xff,0xff,0xff,0x80};
	// 	whitgl_sys_draw_iaabb(draw_rect, col);
	// }
	// for(i=0; i<game.snake.size; i++)
	// {
	// 	whitgl_faabb snake_col;
	// 	snake_col.a = whitgl_ivec_to_fvec(game.snake.pos[i]);
	// 	snake_col.b = whitgl_fvec_add(snake_col.a, whitgl_fvec_val(1));
	// 	whitgl_iaabb draw_rect = whitgl_faabb_to_iaabb(whitgl_faabb_scale(snake_col, whitgl_fvec_val(8)));
	// 	whitgl_sys_color col = {0x00,0x00,0x00,0x80};
	// 	whitgl_sys_draw_iaabb(draw_rect, col);
	// }
}

void game_draw_over(game_game game)
{
	whitgl_int i;
	for(i=0; i<NUM_BLOOD; i++)
		game_blood_draw(game.blood[i], game.camera);
	for(i=0; i<NUM_SHOTS; i++)
		game_shot_draw(game.shots[i], game.camera);
}
