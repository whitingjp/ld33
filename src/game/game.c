#include "game.h"

#include <whitgl/sys.h>
#include <resource.h>

void game_draw(whitgl_ivec screen_size)
{
	whitgl_iaabb screen = {whitgl_ivec_zero, screen_size};
	whitgl_sys_color background = {0x5a, 0x0f, 0x5f, 0xff};
	whitgl_sys_draw_iaabb(screen, background);

	whitgl_sprite snake_head = {IMAGE_SPRITES, {0,0}, {8,8}};
	whitgl_ivec frame = {0,0};
	whitgl_sys_draw_sprite(snake_head, frame, whitgl_ivec_zero);
}
