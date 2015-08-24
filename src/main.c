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
#include <endscreen.h>

#include <resource.h>

const char* post_src = "\
#version 150\
\n\
in vec2 Texturepos;\
out vec4 outColor;\
uniform sampler2D tex;\
uniform float overdrive;\
float rand(vec2 co){\
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\
}\
void main()\
{\
	float offsetx = (rand( Texturepos )-0.5)*(overdrive);\
	float offsety = (rand( Texturepos.yx )-0.5)*(overdrive);\
	outColor = texture( tex, Texturepos+vec2(offsetx/200,offsety/200) );\
	outColor = outColor*(1+overdrive*2);\
}\
";

int main(int argc, char** argv)
{
	WHITGL_LOG("Starting main.");

	bool fullscreen = true;
	bool vsync = true;
	bool can_edit = false;
	int currentarg;
	for(currentarg=1; currentarg<argc; currentarg++)
	{
		if(strncmp(argv[currentarg], "windowed", 10)==0)
			fullscreen = false;
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

	whitgl_shader post_shader = whitgl_shader_zero;
	post_shader.fragment_src = post_src;
	post_shader.num_uniforms = 1;
	post_shader.uniforms[0] = "overdrive";
	if(!whitgl_change_shader(WHITGL_SHADER_POST, post_shader))
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
	end_screen endscreen = end_screen_zero;
	whitgl_float music_volume = 0;
	whitgl_float map_anim = 0;

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
			{
				editor = editor_update(editor, &map, setup.pixel_size);
			}
			else if(endscreen.active)
			{
				endscreen = end_screen_update(endscreen);
			}
			else
			{
				game = game_update(game, &map, setup.size);
				map_anim = whitgl_fwrap(map_anim+0.05, 0, 1);
				if(game.snake.pos[0].x < 1)
					endscreen = end_screen_init(100, 120);
			}

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
			game_draw(game);
		game_map_draw(&map, editing, setup.size, camera, map_anim);
		if(!editing)
			game_draw_over(game);
		end_screen_draw(endscreen, setup.size);
		whitgl_set_shader_uniform(WHITGL_SHADER_POST, 0, game.camera_shake);
		whitgl_sys_draw_finish();
	}

	whitgl_file_save("../../../../../data/level.lvl", sizeof(game_map), (void*)&map);

	whitgl_input_shutdown();
	whitgl_sound_shutdown();

	whitgl_sys_close();

	return 0;
}
