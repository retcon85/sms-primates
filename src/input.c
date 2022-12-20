#include "SMSlib.h"
#include "input.h"
#include "game.h"

#define KEY_HOLD_DELAY 20
#define CPU_HOLD_DELAY 10

CPUPlayer cpu_player;

unsigned int input_read_keys_2player(void)
{
  static int hold1 = KEY_HOLD_DELAY;
  static int hold2 = KEY_HOLD_DELAY;

  unsigned int keys1 = SMS_getKeysPressed();
  unsigned int keys2 = keys1 & 0xffc0;
  keys1 &= 0x003f;
  if (!keys1)
  {
    keys1 = SMS_getKeysHeld() & 0x003f;
    if (!keys1) hold1 = KEY_HOLD_DELAY;
    else if (hold1 != 0)
    {
      keys1 = 0;
      hold1--;
    }
  }
  if (!keys2)
  {
    keys2 = SMS_getKeysHeld() & 0xffc0;
    if (!keys2) hold2 = KEY_HOLD_DELAY;
    else if (hold2 != 0)
    {
      keys2 = 0;
      hold2--;
    }
  }
  return keys1 | keys2;
}

unsigned int input_read_keys_1player(void)
{
  unsigned int player_keys = SMS_getKeysPressed() & 0x003f;
  static int player_hold = KEY_HOLD_DELAY;
  if (!player_keys)
  {
    player_keys = SMS_getKeysHeld() & 0x003f;
    if (!player_keys) player_hold = KEY_HOLD_DELAY;
    else if (player_hold != 0)
    {
      player_keys = 0;
      player_hold--;
    }
  }

  // the CPU player...
  static unsigned int cpu_keys = 0;
  static int cpu_hold = CPU_HOLD_DELAY;
  static int power_delta = 0;
  static int angle_delta = 0;

  if (game.players[1].banana->p.oob)
  {
    cpu_player.target_power += power_delta;
    if (cpu_player.target_power > 99)
    {
      cpu_player.target_power = 99;
      angle_delta = +7;
    }
    cpu_player.target_angle += angle_delta;
    power_delta = 0;
    angle_delta = 0;
  }
  else if (game.players[1].banana->ms.x < game.players[0].ms.x)
  {
    power_delta = -7;
  }
  else
  {
    power_delta = +11;
  }

  if (game.players[0].is_exploding || (game.mode == GAME_MODE_CLASSIC && game.player_turn != 1)) return player_keys;

  int angle_diff = cpu_player.target_angle - game.players[1].angle;
  int power_diff = cpu_player.target_power - game.players[1].velocity;

  if (angle_diff != 0)
  {
    if ((cpu_keys != 0) && (cpu_hold == 0) && (((angle_diff > 0) && (angle_diff <= 5)) || ((angle_diff < 0) && (angle_diff >= -5)))) 
      cpu_keys = 0;
    else cpu_keys = angle_diff > 0 ? PORT_B_KEY_UP : PORT_B_KEY_DOWN;
  }
  else if ((game.mode == GAME_MODE_CLASSIC) && !game.setting_power)
  {
    return player_keys | PORT_B_KEY_1; // select angle
  }
  else if (power_diff != 0)
  {
    if ((cpu_keys != 0) && (cpu_hold == 0) && (((power_diff > 0) && (power_diff <= 5)) || ((power_diff < 0) && (power_diff >= -5)))) 
      cpu_keys = 0;
    else cpu_keys = power_diff > 0 ? PORT_B_KEY_RIGHT : PORT_B_KEY_LEFT;
  }
  else if (game.players[1].banana->p.oob)
  {
    cpu_keys = PORT_B_KEY_1; // fire!
  }

  if (!cpu_keys) cpu_hold = CPU_HOLD_DELAY;
  else
  {
    if (cpu_hold != 0)
      cpu_hold--;
  }

  return player_keys | (cpu_hold == 0 ? cpu_keys : 0);
}
