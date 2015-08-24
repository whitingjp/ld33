#include "endscreen.h"

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <resource.h>

#include "stdio.h"

end_screen end_screen_init(whitgl_int score, whitgl_float time)
{
	end_screen screen;
	screen.active = true;
	screen.score = score;
	screen.time_taken = time;
	screen.transition = 0;
	screen.score_up = 0;
	screen.anim = 0;
	screen.leave = false;
	return screen;
}
end_screen end_screen_update(end_screen screen)
{
	if(!screen.active)
		return screen;
	if(screen.leave)
	{
		screen.transition = whitgl_fclamp(screen.transition-0.025, 0, 1);
		if(screen.transition == 0)
			screen.active = false;
		return screen;
	}
	screen.transition = whitgl_fclamp(screen.transition+0.025, 0, 1);
	if(screen.transition == 1)
		screen.score_up = screen.score_up*0.96+0.04;
	if(screen.score_up > 0.997)
		screen.score_up = 1;
	screen.anim = whitgl_fwrap(screen.anim+1/24.0,0,1);
	if(screen.score_up == 1)
	{
		if(whitgl_input_pressed(WHITGL_INPUT_A))
			screen.leave = true;
	}
	return screen;
}

void _draw_string(const char* string, whitgl_ivec pos)
{
	whitgl_sprite sprite = {IMAGE_SPRITES, {8,104},{8,8}};
	while(*string)
	{
		whitgl_ivec frame = whitgl_ivec_zero;
		if(*string >= '0' && *string <= '9')
			frame.x = 1+(*string)-'0';
		if(*string == ':')
			frame.x = 11;
		whitgl_sys_draw_sprite(sprite, frame, pos);
		pos.x += 8;
		string++;
	}
}
void end_screen_draw(end_screen screen, whitgl_ivec screen_size)
{
	if(!screen.active)
		return;
	whitgl_iaabb whole_screen = {whitgl_ivec_zero, screen_size};
	whitgl_sys_color background = {0x30, 0x2c, 0x4b, 0xff*screen.transition};
	whitgl_sys_draw_iaabb(whole_screen, background);

	whitgl_int trans_pos = screen_size.y*(1-screen.transition);
	whitgl_ivec score_pos = {screen_size.x/2-8*2, screen_size.y/2-16-trans_pos};
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "%5d", (int)(screen.score*screen.score_up));
	_draw_string(buffer, score_pos);
	score_pos.x -= 8;
	whitgl_sprite pickup_sprite = {IMAGE_SPRITES, {16,80}, {8,8}};
	whitgl_ivec frame = {screen.anim*3.999, 0};
	whitgl_sys_draw_sprite(pickup_sprite, frame, score_pos);

	whitgl_ivec time_pos = {screen_size.x/2-8*2, screen_size.y/2-trans_pos};
	whitgl_float t = screen.time_taken*screen.score_up;
	int mins = t/60.0;
	int secs = t-mins*60;
	snprintf(buffer, sizeof(buffer), "%2d:%02d", mins, secs);
	_draw_string(buffer, time_pos);
	whitgl_sprite time_sprite = {IMAGE_SPRITES, {16,112}, {8,8}};
	time_pos.x -= 8;
	whitgl_sys_draw_sprite(time_sprite, whitgl_ivec_zero, time_pos);
}
