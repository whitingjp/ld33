#include "map.h"

#include <whitgl/sys.h>

#include <resource.h>

game_map game_map_zero()
{
	game_map map;
	whitgl_ivec i;
	for(i.x=0; i.x<MAP_WIDTH; i.x++)
	{
		for(i.y=0; i.y<MAP_HEIGHT; i.y++)
		{
			game_map_tile type = TILE_EMPTY;
			if(i.x == 0 || i.x == MAP_WIDTH-1)
				type = TILE_WALL;
			if(i.y == 0 || i.y == MAP_HEIGHT-1)
				type = TILE_WALL;
			if(whitgl_randfloat() > 0.92)
				type = TILE_WALL;
			map.tiles[game_map_index_from_pos(i)] = type;
		}
	}
	return map;
}
void game_map_draw(const game_map* map)
{
	whitgl_sprite map_sprite = {IMAGE_SPRITES, {0,32}, {8,8}};
	whitgl_ivec i;
	for(i.x=0; i.x<MAP_WIDTH; i.x++)
	{
		for(i.y=0; i.y<MAP_HEIGHT; i.y++)
		{
			if(game_map_get_tile(map, i) == TILE_EMPTY)
				continue;
			whitgl_ivec draw_pos = whitgl_ivec_scale(i, map_sprite.size);
			whitgl_int flags = 0;
			whitgl_int j;
			for(j=0; j<4; j++)
			{
				whitgl_ivec test_pos = whitgl_ivec_add(i, whitgl_facing_to_ivec(j));
				if(game_map_get_tile(map, test_pos) == TILE_WALL)
					flags |= (whitgl_int)whitgl_fpow(2, j);
			}
			whitgl_ivec frame = whitgl_ivec_zero;
			switch(flags)
			{
				case  0: frame.x = 3; frame.y = 0; break;
				case  1: frame.x = 3; frame.y = 3; break;
				case  2: frame.x = 0; frame.y = 0; break;
				case  3: frame.x = 0; frame.y = 3; break;
				case  4: frame.x = 3; frame.y = 1; break;
				case  5: frame.x = 3; frame.y = 2; break;
				case  6: frame.x = 0; frame.y = 1; break;
				case  7: frame.x = 0; frame.y = 2; break;
				case  8: frame.x = 2; frame.y = 0; break;
				case  9: frame.x = 2; frame.y = 3; break;
				case 10: frame.x = 1; frame.y = 0; break;
				case 11: frame.x = 1; frame.y = 3; break;
				case 12: frame.x = 2; frame.y = 1; break;
				case 13: frame.x = 2; frame.y = 2; break;
				case 14: frame.x = 1; frame.y = 1; break;
				case 15: frame.x = 1; frame.y = 2; break;
			}
			whitgl_sys_draw_sprite(map_sprite, frame, draw_pos);
		}
	}
}
game_map_tile game_map_get_tile(const game_map* map, whitgl_ivec pos)
{
	game_map_tile outer = TILE_WALL;
	if(pos.x < 0)
		return outer;
	if(pos.x >= MAP_WIDTH)
		return outer;
	if(pos.y < 0)
		return outer;
	if(pos.y >= MAP_HEIGHT)
		return outer;
	return map->tiles[game_map_index_from_pos(pos)];
}
whitgl_ivec game_map_pos_from_index(whitgl_int i)
{
	whitgl_ivec v = {i%MAP_WIDTH, i/MAP_WIDTH};
	return v;
}
whitgl_int game_map_index_from_pos(whitgl_ivec v)
{
	return v.x + v.y*MAP_WIDTH;
}
