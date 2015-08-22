#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <whitgl/timer.h>

int main(int argc, char** argv)
{
	WHITGL_LOG("Starting main.");

	bool fullscreen = false;
	bool vsync = true;
	int currentarg;
	for(currentarg=1; currentarg<argc; currentarg++)
	{
		if(strncmp(argv[currentarg], "fullscreen", 10)==0)
			fullscreen = true;
		if(strncmp(argv[currentarg], "no_vsync", 7)==0)
			vsync = false;
	};

	whitgl_sys_setup setup = whitgl_sys_setup_zero;
	setup.size.x = 160;
	setup.size.y = 90;
	setup.pixel_size = 4;
	setup.name = "LD33";
	setup.vsync = vsync;
	setup.fullscreen = fullscreen;

	if(!whitgl_sys_init(&setup))
		return 1;

	whitgl_sound_init();
	whitgl_input_init();

	// whitgl_sys_add_image(0, "data/sprites.png");

	whitgl_timer_init();

	bool running = true;
	while(running)
	{
		whitgl_sound_update();

		whitgl_timer_tick();
		while(whitgl_timer_should_do_frame(60))
		{
			whitgl_input_update();
			if(whitgl_input_pressed(WHITGL_INPUT_ESC))
				running = false;
			if(whitgl_sys_should_close())
				running = false;
		}

		whitgl_sys_draw_init();
		whitgl_iaabb screen = {whitgl_ivec_zero, setup.size};
		whitgl_sys_color background = {0x5a, 0x0f, 0x5f, 0xff};
		whitgl_sys_draw_iaabb(screen, background);
		whitgl_sys_draw_finish();
	}

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
