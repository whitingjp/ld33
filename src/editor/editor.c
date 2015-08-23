#include "editor.h"

#include <whitgl/input.h>

editor_editor editor_update(editor_editor editor, game_map* map, whitgl_int pixel_size)
{
	editor.camera = whitgl_ivec_sub(editor.camera, whitgl_fvec_to_ivec(whitgl_fvec_scale(whitgl_input_joystick(), whitgl_fvec_val(4))));
	whitgl_ivec mouse_pos = whitgl_input_mouse_pos(pixel_size);
	mouse_pos = whitgl_ivec_sub(mouse_pos, editor.camera);
	whitgl_ivec selected = whitgl_ivec_divide(mouse_pos, whitgl_ivec_val(8));
	whitgl_iaabb bounds = {whitgl_ivec_zero, {MAP_WIDTH, MAP_HEIGHT}};
	selected = whitgl_ivec_bound(selected, bounds);
	whitgl_int index = game_map_index_from_pos(selected);
	whitgl_int i;
	for(i=0; i<TILE_MAX; i++)
		if(whitgl_input_down(WHITGL_INPUT_1+i))
			map->tiles[index] = i;
	return editor;
}
