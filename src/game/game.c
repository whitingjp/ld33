#include "game.h"

#include <whitgl/sys.h>
#include <resource.h>

game_game game_game_zero(const game_map* map)
{
	game_game game;
	whitgl_ivec snake_spawn = {1,1};
	whitgl_int num_walker = 0;
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_zero;
	for(i=0; i<NUM_BLOOD; i++)
		game.blood[i] = game_blood_zero;
	for(i=0; i<NUM_TILES; i++)
	{
		whitgl_ivec pos = game_map_pos_from_index(i);
		if(map->tiles[i] == TILE_SPAWN)
			snake_spawn = pos;
		if(map->tiles[i] == TILE_WALKER)
			game.walkers[num_walker++] = game_walker_spawn(pos);
	}
	game.snake = game_snake_zero(snake_spawn);
	game.next_blood = 0;
	return game;
}

game_game game_update(game_game game, const game_map* map)
{
	game.snake = game_snake_update(game.snake, map);
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_update(game.walkers[i], &game.snake, map);
	for(i=0; i<NUM_BLOOD; i++)
		game.blood[i] = game_blood_update(game.blood[i], map);

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

			whitgl_int j;
			for(j=0; j<24; j++)
			{
				whitgl_fvec spawn_point = {game.walkers[i].pos.x+0.5,game.walkers[i].pos.y+0.5};
				whitgl_fvec speed = whitgl_fvec_scale(whitgl_facing_to_fvec(game.snake.dir), whitgl_fvec_val(0.2));
				speed.y -= 0.5;
				game.blood[game.next_blood] = game_blood_spawn(spawn_point, speed);
				game.next_blood = whitgl_iwrap(game.next_blood+1, 0, NUM_BLOOD);
			}
		}
	}

	return game;
}

void game_draw(game_game game, const game_map* map)
{
	game_snake_draw(game.snake, map);
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game_walker_draw(game.walkers[i]);

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
		game_blood_draw(game.blood[i]);
}
