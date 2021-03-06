#ifndef GAME_SNAKE_
#define GAME_SNAKE_

#include <whitgl/math.h>

#include <game/map.h>

#define MAX_SEGMENTS (128)

typedef struct
{
	whitgl_float t;
	whitgl_ivec pos[MAX_SEGMENTS];
	whitgl_int size;
	whitgl_ivec new_pos;
	whitgl_int dir;
	whitgl_bool do_reverse;
	whitgl_bool falling;
	whitgl_float fall_timer;
	whitgl_float fall_speed;

	whitgl_ivec old_pos;
} game_snake;

game_snake game_snake_zero(whitgl_ivec pos);
game_snake game_snake_update(game_snake snake, const game_map* map);
void game_snake_draw(game_snake snake, whitgl_ivec camera);

whitgl_bool game_snake_collide(game_snake snake, whitgl_faabb box);

#endif // GAME_SNAKE_
