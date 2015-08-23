#include "resource.h"

#include <whitgl/sound.h>
#include <whitgl/sys.h>

void resources_load()
{
	whitgl_sys_add_image(IMAGE_SPRITES, "data/image/sprites.png");
	whitgl_sound_add(SOUND_SPLAT00, "data/sound/splat00.ogg");
	whitgl_sound_add(SOUND_SPLAT01, "data/sound/splat01.ogg");
	whitgl_sound_add(SOUND_SPLAT02, "data/sound/splat02.ogg");
	whitgl_sound_add(SOUND_SPLAT03, "data/sound/splat03.ogg");
	whitgl_sound_add(SOUND_SPLAT04, "data/sound/splat04.ogg");
	whitgl_sound_add(SOUND_SPLAT05, "data/sound/splat05.ogg");
	whitgl_sound_add(SOUND_SLICK00, "data/sound/slick00.ogg");
	whitgl_sound_add(SOUND_SLICK01, "data/sound/slick01.ogg");
	whitgl_sound_add(SOUND_SLICK02, "data/sound/slick02.ogg");
	whitgl_sound_add(SOUND_SLICK03, "data/sound/slick03.ogg");
	whitgl_sound_add(SOUND_SLICK04, "data/sound/slick04.ogg");
	whitgl_sound_add(SOUND_SLICK05, "data/sound/slick05.ogg");
	whitgl_sound_add(SOUND_CHARGE, "data/sound/charge.ogg");
	whitgl_sound_add(SOUND_HIT, "data/sound/hit.ogg");
	whitgl_sound_add(SOUND_LASER, "data/sound/laser.ogg");
	whitgl_sound_add(SOUND_HURT00, "data/sound/hurt00.ogg");
	whitgl_sound_add(SOUND_HURT01, "data/sound/hurt01.ogg");
	whitgl_sound_add(SOUND_HURT02, "data/sound/hurt02.ogg");
	whitgl_sound_add(SOUND_HURT03, "data/sound/hurt03.ogg");
	whitgl_sound_add(SOUND_PLING, "data/sound/pling.ogg");
	whitgl_loop_add(SOUND_MUSIC, "data/sound/bloodworm.ogg");
}
