#ifndef PAUSE_SCREEN_
#define PAUSE_SCREEN_

#include <whitgl/math.h>

typedef struct
{
	whitgl_bool active;
	whitgl_float transition;
	whitgl_int selection;
	whitgl_bool should_exit;
} pause_screen;
static const pause_screen pause_screen_zero = {false, 0, 0, false};
pause_screen pause_screen_update(pause_screen screen);
void pause_screen_draw(pause_screen screen, whitgl_ivec screen_size);


#endif // PAUSE_SCREEN_
