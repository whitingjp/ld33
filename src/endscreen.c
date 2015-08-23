#include "endscreen.h"

#include <whitgl/logging.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <resource.h>

end_screen end_screen_init(whitgl_int score, whitgl_float time)
{
	end_screen screen;
	screen.active = true;
	screen.score = score;
	screen.time_taken = time;
	screen.transition = 0;
	screen.score_up = 0;
	return screen;
}
end_screen end_screen_update(end_screen screen)
{
	if(!screen.active)
		return screen;
	screen.transition = whitgl_fclamp(screen.transition+0.025, 0, 1);
	if(screen.transition == 1)
		screen.score_up = whitgl_fclamp(screen.score_up+0.001, 0, 1);
	return screen;
}
void end_screen_draw(end_screen screen, whitgl_ivec screen_size)
{
	if(!screen.active)
		return;
	whitgl_iaabb whole_screen = {whitgl_ivec_zero, screen_size};
	whitgl_sys_color background = {0x30, 0x2c, 0x4b, 0xff*screen.transition};
	whitgl_sys_draw_iaabb(whole_screen, background);
}
