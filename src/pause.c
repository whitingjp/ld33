#include "pause.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <resource.h>


pause_screen pause_screen_update(pause_screen screen)
{
	if(screen.active)
		screen.transition = whitgl_fclamp(screen.transition+0.1, 0, 1);
	else
		screen.transition = whitgl_fclamp(screen.transition-0.1, 0, 1);
	if(screen.transition < 1)
		return screen;
	if(whitgl_input_pressed(WHITGL_INPUT_DOWN))
		screen.selection = whitgl_iwrap(screen.selection+1, 0, 2);
	if(whitgl_input_pressed(WHITGL_INPUT_UP))
		screen.selection = whitgl_iwrap(screen.selection-1, 0, 2);
	if(whitgl_input_pressed(WHITGL_INPUT_A))
	{
		if(screen.selection == 1)
			screen.should_exit = true;
		screen.active = false;
	}
	return screen;
}
void pause_screen_draw(pause_screen screen, whitgl_ivec screen_size)
{
	if(screen.transition==0)
		return;
	whitgl_iaabb whole_screen = {whitgl_ivec_zero, screen_size};
	whitgl_sys_color background = {0x30, 0x2c, 0x4b, 0xff*screen.transition};
	whitgl_sys_draw_iaabb(whole_screen, background);

	whitgl_sprite option_sprite = {IMAGE_SPRITES, {40,112}, {48,8}};
	whitgl_int trans_pos = screen_size.y*(1-screen.transition);
	whitgl_ivec option_pos = {screen_size.x/2-option_sprite.size.x/2, screen_size.y/2-16-trans_pos};
	whitgl_ivec frame = whitgl_ivec_zero;
	whitgl_sys_draw_sprite(option_sprite, frame, option_pos);
	option_pos.y += 16;
	frame.y++;
	whitgl_sys_draw_sprite(option_sprite, frame, option_pos);
	whitgl_sprite selector_sprite = {IMAGE_SPRITES, {32,112}, {8,8}};
	whitgl_ivec selector_pos = {screen_size.x/2-option_sprite.size.x/2-12, screen_size.y/2-16-trans_pos+16*screen.selection};
	whitgl_sys_draw_sprite(selector_sprite, whitgl_ivec_zero, selector_pos);
}
