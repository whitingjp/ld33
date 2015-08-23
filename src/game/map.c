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
			map.tiles[game_map_index_from_pos(i)] = TILE_EMPTY;
			map.decorations[game_map_index_from_pos(i)] = 0;
		}
	}
	return map;
}

whitgl_iaabb _world_bounds(whitgl_ivec pos, whitgl_ivec sprite_size, whitgl_ivec screen_size)
{
	whitgl_iaabb bounds;
	bounds.a = whitgl_ivec_inverse(whitgl_ivec_divide(pos, sprite_size));
	bounds.b = whitgl_ivec_add(bounds.a, whitgl_ivec_divide(screen_size, sprite_size));
	bounds.a.x-=2; bounds.a.y-=2;
	bounds.b.x+=2; bounds.b.y+=2;
	return bounds;
}

void game_map_draw(const game_map* map, whitgl_bool editor, whitgl_ivec screen_size, whitgl_ivec camera)
{
	whitgl_sprite map_sprite = {IMAGE_SPRITES, {0,32}, {8,8}};
	whitgl_ivec i;
	whitgl_iaabb bounds = _world_bounds(camera, map_sprite.size, screen_size);
	for(i.x=bounds.a.x; i.x<bounds.b.x; i.x++)
	{
		for(i.y=bounds.a.y; i.y<bounds.b.y; i.y++)
		{
			game_map_tile tile = game_map_get_tile(map, i);
			whitgl_ivec draw_pos = whitgl_ivec_scale(i, map_sprite.size);
			draw_pos = whitgl_ivec_add(draw_pos, camera);

			char decoration = game_map_get_decoration(map, i);
			if(decoration > 0)
			{
				char d = decoration-1;
				whitgl_sprite decoration_sprite = {IMAGE_SPRITES, {32,56}, {8,8}};
				whitgl_ivec frame = {d%4, d/4};
				whitgl_sys_draw_sprite(decoration_sprite, frame, draw_pos);
				continue;
			}
			if(tile == TILE_EMPTY)
				continue;
			if(tile >= TILE_SPAWN)
			{
				whitgl_ivec frame = {tile-TILE_SPAWN, 4};
				if(editor)
					whitgl_sys_draw_sprite(map_sprite, frame, draw_pos);
				continue;
			}
			whitgl_ivec vine_frame = {4,1};
			if(tile == TILE_VINE)
			{
				whitgl_sys_draw_sprite(map_sprite, vine_frame, draw_pos);
				continue;
			}

			whitgl_int flags = 0;
			whitgl_int j;
			for(j=0; j<4; j++)
			{
				whitgl_ivec test_pos = whitgl_ivec_add(i, whitgl_facing_to_ivec(j));
				game_map_tile tile = game_map_get_tile(map, test_pos);
				if(tile == TILE_WALL || tile == TILE_VINE_WALL)
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
			if(tile == TILE_VINE_WALL)
				whitgl_sys_draw_sprite(map_sprite, vine_frame, draw_pos);
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
char game_map_get_decoration(const game_map* map, whitgl_ivec pos)
{
	char outer = 0;
	if(pos.x < 0)
		return outer;
	if(pos.x >= MAP_WIDTH)
		return outer;
	if(pos.y < 0)
		return outer;
	if(pos.y >= MAP_HEIGHT)
		return outer;
	return map->decorations[game_map_index_from_pos(pos)];
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

whitgl_bool game_map_collide(const game_map* map, whitgl_faabb box)
{
	// whitgl_fvec scale = {8,8};
	whitgl_iaabb bounds = whitgl_faabb_to_iaabb(box);
	whitgl_ivec p;
	for(p.x=bounds.a.x; p.x<=bounds.b.x; p.x++)
	{
		for(p.y=bounds.a.y; p.y<=bounds.b.y; p.y++)
		{
			whitgl_int i = game_map_index_from_pos(p);
			if(map->tiles[i] == TILE_WALL || map->tiles[i] == TILE_VINE_WALL)
				return true;
		}
	}
	return false;
}

