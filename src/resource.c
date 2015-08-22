#include "resource.h"

#include <whitgl/sound.h>
#include <whitgl/sys.h>

void resources_load()
{
	whitgl_sys_add_image(IMAGE_SPRITES, "data/image/sprites.png");
}
