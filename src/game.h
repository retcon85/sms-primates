#include <stdbool.h>
#include <stdint.h>
#include "meta.h"
#include "projectiles.h"

#define N_PLAYERS 2

typedef struct Banana
{
  Projectile p;
  MetaSprite ms;
  bool collision_check;
  bool is_exploding;
} Banana;

typedef struct Player
{
  MetaSprite ms;
  bool is_exploding;
  uint8_t angle;
  uint8_t velocity;
  uint8_t score;
  Banana *banana;
} Player;

#define GAME_MODE_ARCADE 0
#define GAME_MODE_CLASSIC 1
typedef struct Game
{
  int points_limit;
  int mode;
  bool cpu_opponent;
  Player players[N_PLAYERS];
  int player_turn; // for classic game only
  bool setting_angle; // for classic game only
  bool setting_power; // for classic game only
  bool allow_death_throw; // option - for arcade game only
  bool allow_death_point; // option - for arcade game only
} Game;

extern Game game;

void play_game(void);
