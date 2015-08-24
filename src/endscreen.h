#ifndef END_SCREEN_
#define END_SCREEN_

#include <whitgl/math.h>

typedef struct
{
	whitgl_bool active;
	whitgl_int score;
	whitgl_float time_taken;
	whitgl_float transition;
	whitgl_float score_up;
	whitgl_float anim;
	whitgl_bool leave;
} end_screen;
static const end_screen end_screen_zero = {false, 0, 0, 0, 0, 0, false};
end_screen end_screen_init(whitgl_int score, whitgl_float time);
end_screen end_screen_update(end_screen screen);
void end_screen_draw(end_screen screen, whitgl_ivec screen_size);


#endif // END_SCREEN_
