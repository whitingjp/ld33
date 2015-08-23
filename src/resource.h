#ifndef RESOURCE_
#define RESOURCE_

typedef enum
{
	IMAGE_SPRITES,
	IMAGE_MAX
} resource_image_enum;

typedef enum
{
	SOUND_SPLAT00,
	SOUND_SPLAT01,
	SOUND_SPLAT02,
	SOUND_SPLAT03,
	SOUND_SPLAT04,
	SOUND_SPLAT05,
	SOUND_SLICK00,
	SOUND_SLICK01,
	SOUND_SLICK02,
	SOUND_SLICK03,
	SOUND_SLICK04,
	SOUND_SLICK05,
	SOUND_CHARGE,
	SOUND_HIT,
	SOUND_LASER,
	SOUND_HURT00,
	SOUND_HURT01,
	SOUND_HURT02,
	SOUND_HURT03,
	SOUND_PLING,
	SOUND_GLANCE,
	SOUND_SWAP,
	SOUND_MUSIC,
	SOUND_MAX
} resource_sound_enum;

void resources_load();

#endif // RESOURCE_
