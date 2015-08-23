#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <whitgl/file.h>
#include <whitgl/input.h>
#include <whitgl/logging.h>
#include <whitgl/math.h>
#include <whitgl/sound.h>
#include <whitgl/sys.h>
#include <whitgl/timer.h>

#include <editor/editor.h>
#include <game/game.h>
#include <game/map.h>

#include <resource.h>

int main(int argc, char** argv)
{
	WHITGL_LOG("Starting main.");

	bool fullscreen = false;
	bool vsync = true;
	bool can_edit = false;
	int currentarg;
	for(currentarg=1; currentarg<argc; currentarg++)
	{
		if(strncmp(argv[currentarg], "fullscreen", 10)==0)
			fullscreen = true;
		if(strncmp(argv[currentarg], "no_vsync", 7)==0)
			vsync = false;
		if(strncmp(argv[currentarg], "editor", 6)==0)
			can_edit = true;
	};

	whitgl_sys_setup setup = whitgl_sys_setup_zero;
	setup.size.x = 160;
	setup.size.y = 90;
	setup.pixel_size = 5;
	setup.name = "LD33";
	setup.vsync = vsync;
	setup.fullscreen = fullscreen;
	setup.cursor = can_edit ? CURSOR_SHOW : CURSOR_DISABLE;

	if(!whitgl_sys_init(&setup))
		return 1;

	whitgl_sound_init();
	whitgl_input_init();

	resources_load();

	double now = whitgl_sys_get_time();
	whitgl_randseed(now*10000);

	game_map map = game_map_zero();
	whitgl_file_load("data/level.lvl", sizeof(game_map), (void*)&map);
	game_game game = game_game_zero(&map, setup.size);
	editor_editor editor = editor_editor_zero;
	whitgl_float music_volume = 0;

	whitgl_bool editing = false;

	whitgl_timer_init();

	bool running = true;
	while(running)
	{
		whitgl_sound_update();

		whitgl_timer_tick();
		while(whitgl_timer_should_do_frame(60))
		{
			whitgl_input_update();
			if(can_edit && whitgl_input_pressed(WHITGL_INPUT_START))
			{
				editing = !editing;
				editor.camera = game.camera;
				game = game_game_zero(&map, setup.size);
			}
			if(editing)
				editor = editor_update(editor, &map, setup.pixel_size);
			else
				game = game_update(game, &map, setup.size);

			if(whitgl_input_pressed(WHITGL_INPUT_ESC))
				running = false;
			if(whitgl_sys_should_close())
				running = false;

			if(!can_edit)
				music_volume = whitgl_fclamp(music_volume+0.001, 0, 0.5);
			whitgl_loop_volume(SOUND_MUSIC, music_volume);
		}

		whitgl_sys_draw_init();
		whitgl_iaabb screen = {whitgl_ivec_zero, setup.size};
		whitgl_sys_color background = {0x30, 0x2c, 0x4b, 0xff};
		whitgl_sys_draw_iaabb(screen, background);
		whitgl_ivec camera = editing ? editor.camera : game.camera;
		if(!editing)
			game_draw(game, &map);
		game_map_draw(&map, editing, setup.size, camera);
		if(!editing)
			game_draw_over(game);
		whitgl_sys_draw_finish();
	}

	whitgl_file_save("../../../../../data/level.lvl", sizeof(game_map), (void*)&map);

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
