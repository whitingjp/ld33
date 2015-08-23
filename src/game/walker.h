#ifndef GAME_WALKER_
#define GAME_WALKER_

#include <game/game.h>
#include <game/map.h>
#include <game/snake.h>


game_walker game_walker_spawn(whitgl_ivec pos, whitgl_int index);
game_walker game_walker_update(game_walker walker, const game_game* game, const game_map* map);
void game_walker_draw(game_walker walker, whitgl_ivec camera);
whitgl_faabb game_walker_collider(game_walker walker);

#endif // GAME_WALKER_
