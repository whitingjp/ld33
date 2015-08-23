#include "game.h"

#include <whitgl/sound.h>
#include <whitgl/sys.h>

#include <game/walker.h>

#include <resource.h>

whitgl_fvec _game_camera_target(game_game game, whitgl_ivec screen_size)
{
	whitgl_fvec target = whitgl_ivec_to_fvec(game.snake.pos[game.snake.size-1]);
	whitgl_int i;
	for(i=game.snake.size-2; i>=0; i--)
		target = whitgl_fvec_interpolate(target, whitgl_ivec_to_fvec(game.snake.pos[i]), 0.3);
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
	whitgl_int num_pickups = 0;
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_zero;
	for(i=0; i<NUM_BLOOD; i++)
		game.blood[i] = game_blood_zero;
	game.next_blood = 0;
	for(i=0; i<NUM_SHOTS; i++)
		game.shots[i] = game_shot_zero;
	game.next_shot = 0;
	for(i=0; i<NUM_PICKUPS; i++)
		game.pickups[i] = game_pickup_zero;
	for(i=0; i<NUM_TILES; i++)
	{
		whitgl_ivec pos = game_map_pos_from_index(i);
		if(map->tiles[i] == TILE_SPAWN)
			snake_spawn = pos;
		if(map->tiles[i] == TILE_WALKER)
		{
			game.walkers[num_walker] = game_walker_spawn(pos, num_walker);
			num_walker++;
		}
		if(map->tiles[i] == TILE_PICKUP)
		{
			game.pickups[num_pickups] = game_pickup_spawn(pos);
			num_pickups++;
		}
	}
	game.snake = game_snake_zero(snake_spawn);
	game.fcamera = whitgl_fvec_inverse(_game_camera_target(game, screen_size));
	game.camera = whitgl_fvec_to_ivec(game.fcamera);
	game.camera_shake = 0;
	return game;
}

game_game _game_spawn_blood(game_game game, whitgl_fvec pos, whitgl_fvec speed)
{
	whitgl_int j;
	for(j=0; j<24; j++)
	{
		whitgl_fvec spawn_point = {pos.x+0.5,pos.y+0.5};
		speed.y -= 0.1;
		game.blood[game.next_blood] = game_blood_spawn(spawn_point, speed);
		game.next_blood = whitgl_iwrap(game.next_blood+1, 0, NUM_BLOOD);
	}
	return game;
}

game_game game_update(game_game game, const game_map* map, whitgl_ivec screen_size)
{
	game.snake = game_snake_update(game.snake, map);
	whitgl_int i, j;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_update(game.walkers[i], &game, map);
	for(i=0; i<NUM_BLOOD; i++)
		game.blood[i] = game_blood_update(game.blood[i], map);
	for(i=0; i<NUM_SHOTS; i++)
		game.shots[i] = game_shot_update(game.shots[i], map);
	for(i=0; i<NUM_PICKUPS; i++)
		game.pickups[i] = game_pickup_update(game.pickups[i]);

	whitgl_fvec target = _game_camera_target(game, screen_size);
	game.fcamera = whitgl_fvec_interpolate(game.fcamera, whitgl_fvec_inverse(target), 0.08);
	whitgl_fvec shake = {(whitgl_randfloat()-0.5)*game.camera_shake*16, (whitgl_randfloat()-0.5)*game.camera_shake*16};
	game.camera = whitgl_fvec_to_ivec(whitgl_fvec_add(game.fcamera, shake));

	game.camera_shake -= 0.05;
	if(game.camera_shake < 0)
		game.camera_shake = 0;

	for(i=0; i<NUM_WALKERS; i++)
	{
		if(!game.walkers[i].active)
			continue;
		whitgl_faabb walk_col = game_walker_collider(game.walkers[i]);
		if(!game_snake_collide(game.snake, walk_col))
			continue;
		game.walkers[i].active = false;
		game.snake.size++;
		game.snake.old_pos = game.snake.pos[game.snake.size];
		whitgl_sound_play(SOUND_SPLAT00+whitgl_randint(6), whitgl_randfloat()/5+0.9);
		whitgl_fvec speed = whitgl_fvec_scale(whitgl_facing_to_fvec(game.snake.dir), whitgl_fvec_val(0.3));
		game = _game_spawn_blood(game, game.walkers[i].pos, speed);
		game.camera_shake += 0.75;
	}

	for(i=0; i<NUM_SHOTS; i++)
	{
		if(!game.shots[i].active)
			continue;
		whitgl_faabb shot_col = game_shot_collider(game.shots[i]);
		if(!game_snake_collide(game.snake, shot_col))
			continue;
		game.shots[i].active = false;
		if(game.snake.size > 3)
			game.snake.size--;
		game.snake.old_pos = game.snake.pos[game.snake.size];
		whitgl_sound_play(SOUND_HURT00+whitgl_randint(4), whitgl_randfloat()*0.5+0.75);
		game = _game_spawn_blood(game, game.shots[i].pos, game.shots[i].speed);
		game.camera_shake += 1;
	}

	for(i=0; i<NUM_WALKERS; i++)
	{
		if(!game.walkers[i].active)
			continue;
		for(j=0; j<NUM_SHOTS; j++)
		{
			if(!game.shots[j].active)
				continue;
			if(game.shots[j].owner == i)
				continue;
			whitgl_faabb shot_col = game_shot_collider(game.shots[j]);
			whitgl_faabb walk_col = game_walker_collider(game.walkers[i]);
			if(!whitgl_faabb_intersects(shot_col, walk_col))
				continue;
			game.shots[j].active = false;
			game.walkers[i].active = false;
			whitgl_sound_play(SOUND_HIT, whitgl_randfloat()*0.5+0.75);
			game = _game_spawn_blood(game, game.shots[j].pos, game.shots[j].speed);
			game.camera_shake += 0.5;
		}
	}

	for(i=0; i<NUM_WALKERS; i++)
	{
		if(!game.walkers[i].active)
			continue;
		if(!game.walkers[i].shooting)
			continue;
		game.shots[game.next_shot] = game_shot_spawn(game.walkers[i].pos, game.walkers[i].speed < 0, i);
		game.next_shot = whitgl_iwrap(game.next_shot+1, 0, NUM_SHOTS);
		whitgl_sound_play(SOUND_LASER, whitgl_randfloat()*0.5+0.75);
		game.camera_shake += 0.2;
	}

	for(i=0; i<NUM_PICKUPS; i++)
	{
		if(!game.pickups[i].active)
			continue;
		if(game.pickups[i].picked)
			continue;
		whitgl_faabb pickup_col = game_pickup_collider(game.pickups[i]);
		if(!game_snake_collide(game.snake, pickup_col))
			continue;
		game.pickups[i] = game_pickup_picked(game.pickups[i]);
		game.camera_shake += 0.1;
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

void game_draw(game_game game)
{
	game_snake_draw(game.snake, game.camera);
	whitgl_int i;
	for(i=0; i<NUM_PICKUPS; i++)
		game_pickup_draw(game.pickups[i], game.camera);
}

void game_draw_over(game_game game)
{
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game_walker_draw(game.walkers[i], game.camera);
	for(i=0; i<NUM_BLOOD; i++)
		game_blood_draw(game.blood[i], game.camera);
	for(i=0; i<NUM_SHOTS; i++)
		game_shot_draw(game.shots[i], game.camera);
}
