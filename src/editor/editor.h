#ifndef EDITOR_EDITOR_
#define EDITOR_EDITOR_

#include <game/map.h>

typedef struct
{
	whitgl_ivec camera;
} editor_editor;
static const editor_editor editor_editor_zero = {{0,0}};
editor_editor editor_update(editor_editor editor, game_map* map, whitgl_int pixel_size);

#endif // EDITOR_EDITOR_
