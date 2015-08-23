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
	for(i=0; i<NUM_TILES; i++)
	{
		whitgl_ivec pos = game_map_pos_from_index(i);
		if(map->tiles[i] == TILE_SPAWN)
			snake_spawn = pos;
		if(map->tiles[i] == TILE_WALKER)
			game.walkers[num_walker++] = game_walker_spawn(pos);
	}
	game.snake = game_snake_zero(snake_spawn);
	return game;
}

game_game game_update(game_game game, const game_map* map)
{
	game.snake = game_snake_update(game.snake, map);
	whitgl_int i;
	for(i=0; i<NUM_WALKERS; i++)
		game.walkers[i] = game_walker_update(game.walkers[i], map);

	whitgl_int j;
	for(i=0; i<NUM_WALKERS; i++)
	{
		if(!game.walkers[i].active)
			continue;
		whitgl_faabb walk_col = game_walker_collider(game.walkers[i]);
		for(j=0; j<game.snake.size; j++)
		{
			whitgl_faabb snake_col;
			snake_col.a = whitgl_ivec_to_fvec(game.snake.pos[j]);
			snake_col.b = whitgl_fvec_add(snake_col.a, whitgl_fvec_val(1));
			if(whitgl_faabb_intersects(snake_col, walk_col))
			{
				game.walkers[i].active = false;
				game.snake.size++;
				game.snake.old_pos = game.snake.pos[game.snake.size];
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
