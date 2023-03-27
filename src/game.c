#include <stdint.h>
#include "SMSlib.h"
#ifdef USEPSGLIB // Set USEPSGLIB := true in Makefile to build with PSGlib for music and sounds
#include "PSGlib.h"
#endif
#include "assets.h"
#include "random.h"
#include "projectiles.h"
#include "meta.h"
#include "game.h"
#include "input.h"
#include "util.h"

#define SCALE 512L
#define COLLISION (SMS_VDPFlags & VDPFLAG_SPRITECOLLISION)
#define COLLISION_NONE 0
#define COLLISION_PLAYER 1
#define COLLISION_BANANA 2
#define N_BANANAS 2
#define N_METAS (N_BANANAS + N_PLAYERS)
#define KEY_HOLD_DELAY 20

Game game = { .points_limit = 3, .mode = GAME_MODE_ARCADE };
Banana bananas[N_BANANAS];
MetaSprite *metas[N_METAS];
bool explosion_occurring = false;

inline void print_dec2(unsigned int x, unsigned int y, int value, unsigned int pad)
{
  SMS_setTileatXY(x, y, value > 9 ? value/10 + NUMBER_TILE_FROM : pad);
  SMS_setTileatXY(x + 1, y, value%10 + NUMBER_TILE_FROM);
}

inline void move_banana(Banana *banana)
{
  if (banana->is_exploding) return;
  if (banana->p.x >= (SCREEN_WIDTH * SCALE))
    banana->p.oob = true;
  if (!projectile_move(&banana->p))
  {
    if (banana->p.y < (SCREEN_TOP * SCALE))
    {
      meta_move_to(&banana->ms, banana->p.x / SCALE, SCREEN_HEIGHT - (banana->p.y / SCALE));
      meta_show(&banana->ms);
    }
    else
    {
      meta_hide(&banana->ms);
    }
  }
  else
  {
    meta_hide(&banana->ms);
  }
}

// unsigned int copy_tile_data(unsigned int dst, unsigned int src)
// {
//   uint8_t buf[32];
//   SMS_readVRAM(&buf, src * 32, 32);
//   SMS_VRAMmemcpy(dst * 32, &buf, 32);
//   return dst;
// }

extern void generate_skyline(void);

inline void display_angle(int player)
{
  print_dec2(7 + (23*player), 0, game.players[player].angle, NUMBER_TILE_FROM);
}

inline void display_power(int player)
{
  print_dec2(7 + (23*player), 1, game.players[player].velocity, NUMBER_TILE_FROM);
}

inline void display_score(int player)
{
  print_dec2(7 + (23*player), 23, game.players[player].score, BLANK_TILE);
}


void display_angle_label(int player)
{
  SMS_setNextTileatXY(player * 23, 0);
  SMS_setTile(BLANK_TILE);
  SMS_setTile(LABEL_TILE_FROM + 0);
  SMS_setTile(LABEL_TILE_FROM + 1);
  SMS_setTile(LABEL_TILE_FROM + 2);
  SMS_setTile(LABEL_TILE_FROM + 3);
  SMS_setTile(LABEL_TILE_FROM + 4);
  SMS_setTile(LABEL_TILE_FROM + 9);
}

void display_power_label(int player)
{
  SMS_setNextTileatXY(player * 23, 1);
  SMS_setTile(BLANK_TILE);
  SMS_setTile(LABEL_TILE_FROM + 5);
  SMS_setTile(LABEL_TILE_FROM + 6);
  SMS_setTile(LABEL_TILE_FROM + 7);
  SMS_setTile(LABEL_TILE_FROM + 4);
  SMS_setTile(LABEL_TILE_FROM + 8);
  SMS_setTile(LABEL_TILE_FROM + 9);
}

void clear_label_area(void)
{
  SMS_setNextTileatXY(0, 0);
  for (int i = 0; i < 15; i++)
    SMS_setTile(BLANK_TILE);
  SMS_setTile(1 | TILE_USE_SPRITE_PALETTE);
  SMS_setTile(2 | TILE_USE_SPRITE_PALETTE);
  for (int i = 0; i < 30; i++)
    SMS_setTile(BLANK_TILE);
  SMS_setTile(3 | TILE_USE_SPRITE_PALETTE);
  SMS_setTile(4 | TILE_USE_SPRITE_PALETTE);
  for (int i = 0; i < 15; i++)
    SMS_setTile(BLANK_TILE);
}

inline void handle_input_arcade(unsigned int keys)
{
  if (keys & (PORT_A_KEY_UP | PORT_A_KEY_DOWN))
  {
    if (keys & PORT_A_KEY_UP)
    {
      if (game.players[0].angle < 89) game.players[0].angle++;
    }
    else if (game.players[0].angle > 0) game.players[0].angle--;
    display_angle(0);
  }
  if (keys & (PORT_A_KEY_RIGHT | PORT_A_KEY_LEFT))
  {
    if (keys & PORT_A_KEY_RIGHT)
    {
      if (game.players[0].velocity < 99) game.players[0].velocity++;
    }
    else if (game.players[0].velocity > 0) game.players[0].velocity--;
    display_power(0);
  }
  if (keys & (PORT_B_KEY_UP | PORT_B_KEY_DOWN))
  {
    if (keys & PORT_B_KEY_UP)
    {
      if (game.players[1].angle < 89) game.players[1].angle++;
    }
    else if (game.players[1].angle > 0) game.players[1].angle--;
    display_angle(1);
  }
  if (keys & (PORT_B_KEY_RIGHT | PORT_B_KEY_LEFT))
  {
    if (keys & PORT_B_KEY_RIGHT)
    {
      if (game.players[1].velocity < 99) game.players[1].velocity++;
    }
    else if (game.players[1].velocity > 0) game.players[1].velocity--;
    display_power(1);
  }
  if ((keys & (PORT_A_KEY_2 | PORT_A_KEY_1)) && bananas[0].p.oob)
  {
    // hide banana icon in arcade mode when throwing
    SMS_setTileatXY(10, 22, BLANK_TILE);
    projectile_launch_from(
      &bananas[0].p, 
      game.players[0].ms.x * SCALE, 
      (192 - game.players[0].ms.y) * SCALE, 
      game.players[0].angle, 
      game.players[0].velocity
    );
  }
  if ((keys & (PORT_B_KEY_2 | PORT_B_KEY_1)) && bananas[1].p.oob)
  {
    // hide banana icon in arcade mode when throwing
    SMS_setTileatXY(22, 22, BLANK_TILE);
    projectile_launch_from(
      &bananas[1].p, 
      (game.players[1].ms.x + 8) * SCALE, 
      (192 - game.players[1].ms.y) * SCALE, 
      270 + game.players[1].angle, 
      game.players[1].velocity
    );
  }
}

inline void handle_input_classic(unsigned int keys)
{
  static bool setting = false;

  if (!game.setting_angle && (!bananas[0].p.oob || !bananas[1].p.oob || explosion_occurring)) return;
  game.setting_angle = true;
  
  // normalise keys pressed to player 0's keys for convenience
  keys = (game.player_turn == 0 ? keys : keys >> 6) & 0x003f;

  if (game.setting_power)
  {
    display_power_label(game.player_turn);
    if (!setting)
    {
      SMS_setTile(BLANK_TILE);
      SMS_setTile(BLANK_TILE);
    }
    if ((keys & (PORT_A_KEY_UP | PORT_A_KEY_RIGHT)) && (game.players[game.player_turn].velocity < 99))
    {
      game.players[game.player_turn].velocity++;
      setting = true;
    }
    else if ((keys & (PORT_A_KEY_DOWN | PORT_A_KEY_LEFT) && (game.players[game.player_turn].velocity > 0)))
    {
      game.players[game.player_turn].velocity--;
      setting = true;
    }
    else if (setting && (keys & (PORT_A_KEY_2 | PORT_A_KEY_1)))
    {
#ifdef USEPSGLIB
    PSGSFXPlay(classic_throw_psg, SFX_CHANNEL2);
#endif
      projectile_launch_from(
        &bananas[game.player_turn].p, 
        game.players[game.player_turn].ms.x * SCALE, 
        (192 - game.players[game.player_turn].ms.y) * SCALE, 
        (game.player_turn == 0 ? 0 : 270) + game.players[game.player_turn].angle, 
        game.players[game.player_turn].velocity
      );
      game.players[game.player_turn].angle = 0;
      game.players[game.player_turn].velocity = 0;
      clear_label_area();
      game.setting_angle = game.setting_power = false;
      game.player_turn = (game.player_turn + 1) % 2;
      setting = false;
      return;
    }
    if (setting)
      display_power(game.player_turn);
  }
  else
  {
    display_angle_label(game.player_turn);
    if (!setting)
    {
      SMS_setTile(BLANK_TILE);
      SMS_setTile(BLANK_TILE);
    }
    if ((keys & (PORT_A_KEY_UP | PORT_A_KEY_RIGHT)) && (game.players[game.player_turn].angle < 89))
    {
      game.players[game.player_turn].angle++;
      setting = true;
    }
    else if ((keys & (PORT_A_KEY_DOWN | PORT_A_KEY_LEFT) && (game.players[game.player_turn].angle > 0)))
    {
      game.players[game.player_turn].angle--;
      setting = true;
    }
    else if (setting && (keys & (PORT_A_KEY_2 | PORT_A_KEY_1)))
    {
      game.setting_power = true;
      setting = false;
    }
    if (setting)
      display_angle(game.player_turn);
  }
}

void reset_level(void)
{
  SMS_displayOff();
  meta_hide(&game.players[0].ms);
  meta_hide(&game.players[1].ms);
  UNSAFE_SMS_copySpritestoSAT();

  SMS_setBGScrollX(0);
  SMS_setBGScrollY(0);
  explosion_occurring = false;
  cpu_player.target_angle = 45;
  cpu_player.target_power = 50;
  game.players[0].angle = game.players[1].angle = game.mode == GAME_MODE_ARCADE ? cpu_player.target_angle : 0;
  game.players[0].velocity = game.players[1].velocity = game.mode == GAME_MODE_ARCADE ? cpu_player.target_power : 0;
  game.players[0].banana = &bananas[0];
  game.players[1].banana = &bananas[1];
  game.players[0].is_exploding = game.players[1].is_exploding = false;
  bananas[0].p.oob = bananas[1].p.oob = true;
  bananas[0].is_exploding = bananas[1].is_exploding = false;

  int8_t wind = (31 & get_random()) - 16;
  if (wind < 0) wind++; // to make it symmetrical, i.e. -15 -> +15
  projectile_set_wind_speed(wind);

  clear_label_area();
  if (game.mode == GAME_MODE_ARCADE)
  {
    display_angle_label(0);
    display_angle_label(1);
    display_power_label(0);
    display_power_label(1);
  }

  SMS_setNextTileatXY(1, 22);
  // PLAYER 2
  SMS_setTile(LABEL_TILE_FROM + 5);
  SMS_setTile(LABEL_TILE_FROM + 3);
  SMS_setTile(LABEL_TILE_FROM + 0);
  SMS_setTile(LABEL_TILE_FROM + 10);
  SMS_setTile(LABEL_TILE_FROM + 4);
  SMS_setTile(LABEL_TILE_FROM + 8);
  SMS_setTile(BLANK_TILE);
  SMS_setTile(NUMBER_TILE_FROM + 1);
  for (int i = 0; i < 5; i++)
    SMS_setTile(BLANK_TILE);
  // WIND
  SMS_setTile(LABEL_TILE_FROM + 7);
  SMS_setTile(LABEL_TILE_FROM + 11);
  SMS_setTile(LABEL_TILE_FROM + 1);
  SMS_setTile(LABEL_TILE_FROM + 12);
  for (int i = 0; i < 6; i++)
    SMS_setTile(BLANK_TILE);
  // PLAYER 2
  SMS_setTile(LABEL_TILE_FROM + 5);
  SMS_setTile(LABEL_TILE_FROM + 3);
  SMS_setTile(LABEL_TILE_FROM + 0);
  SMS_setTile(LABEL_TILE_FROM + 10);
  SMS_setTile(LABEL_TILE_FROM + 4);
  SMS_setTile(LABEL_TILE_FROM + 8);
  SMS_setTile(BLANK_TILE);
  SMS_setTile(NUMBER_TILE_FROM + 2);

  // SCORE
  SMS_setTile(BLANK_TILE);
  SMS_setTile(LABEL_TILE_FROM + 13);
  SMS_setTile(LABEL_TILE_FROM + 14);
  SMS_setTile(LABEL_TILE_FROM + 6);
  SMS_setTile(LABEL_TILE_FROM + 8);
  SMS_setTile(LABEL_TILE_FROM + 4);
  for (int i = 0; i < 6; i++)
    SMS_setTile(BLANK_TILE);

  // windometer
  SMS_setTile(wind >= 0 ? WIND_TILE_FROM : (WIND_TILE_FROM + 4) | TILE_FLIPPED_X);
  for (int i = 0; i < 6; i++)  
    SMS_setTile((WIND_TILE_FROM + 3) | (wind >= 0 ? 0 : TILE_FLIPPED_X));
  SMS_setTile(wind >= 0 ? WIND_TILE_FROM + 4 : WIND_TILE_FROM | TILE_FLIPPED_X);

  for (int i = 0; i < 4; i++)
    SMS_setTile(BLANK_TILE);
  // SCORE
  SMS_setTile(LABEL_TILE_FROM + 13);
  SMS_setTile(LABEL_TILE_FROM + 14);
  SMS_setTile(LABEL_TILE_FROM + 6);
  SMS_setTile(LABEL_TILE_FROM + 8);
  SMS_setTile(LABEL_TILE_FROM + 4);

  for (int i = 0; i < 4*32; i++)
    SMS_setTile(BLANK_TILE);

  SMS_mapROMBank(font_bin_bank);
  SMS_loadTiles(GLYPH('0'), NUMBER_TILE_FROM, 32 * 10);
  SMS_loadTiles(GLYPH('A'), LABEL_TILE_FROM + 0, 32);
  SMS_loadTiles(GLYPH('N'), LABEL_TILE_FROM + 1, 32);
  SMS_loadTiles(GLYPH('G'), LABEL_TILE_FROM + 2, 32);
  SMS_loadTiles(GLYPH('L'), LABEL_TILE_FROM + 3, 32);
  SMS_loadTiles(GLYPH('E'), LABEL_TILE_FROM + 4, 32);
  SMS_loadTiles(GLYPH('P'), LABEL_TILE_FROM + 5, 32);
  SMS_loadTiles(GLYPH('O'), LABEL_TILE_FROM + 6, 32);
  SMS_loadTiles(GLYPH('W'), LABEL_TILE_FROM + 7, 32);
  SMS_loadTiles(GLYPH('R'), LABEL_TILE_FROM + 8, 32);
  SMS_loadTiles(GLYPH(':'), LABEL_TILE_FROM + 9, 32);
  SMS_loadTiles(GLYPH('Y'), LABEL_TILE_FROM + 10, 32);
  SMS_loadTiles(GLYPH('I'), LABEL_TILE_FROM + 11, 32);
  SMS_loadTiles(GLYPH('D'), LABEL_TILE_FROM + 12, 32);
  SMS_loadTiles(GLYPH('S'), LABEL_TILE_FROM + 13, 32);
  SMS_loadTiles(GLYPH('C'), LABEL_TILE_FROM + 14, 32);

  // windometer
  SMS_mapROMBank(wind_bin_bank);
  SMS_loadTiles(windometer_tiles + 3*32, WIND_TILE_FROM + 1, 3*32);

  if (wind >= 0)
  {
    SMS_loadTiles(windometer_tiles + ((wind < 1 ? 1 : 2)*32), WIND_TILE_FROM, 32);
    for (int i = 1; i <= 6; i++)
    {
      if (wind == 2*i) SMS_setTileatXY(12 + i, 23, WIND_TILE_FROM + 2);
      else if (wind > 2*i) SMS_setTileatXY(12 + i, 23, WIND_TILE_FROM + 1);
    }
    if (wind < 14) SMS_loadTiles(windometer_tiles + 6*32, WIND_TILE_FROM + 4, 32);
    else if (wind > 14) SMS_loadTiles(windometer_tiles + 7*32, WIND_TILE_FROM + 4, 32);
    else SMS_loadTiles(windometer_tiles + 8*32, WIND_TILE_FROM + 4, 32);
  }
  else
  {
    SMS_loadTiles(windometer_tiles + ((wind > -1 ? 1 : 2)*32), WIND_TILE_FROM, 32);
    for (int i = -1; i >= -6; i--)
    {
      if (wind == 2*i) SMS_setTileatXY(19 + i, 23, (WIND_TILE_FROM + 2) | TILE_FLIPPED_X);
      else if (wind < 2*i) SMS_setTileatXY(19 + i, 23, WIND_TILE_FROM + 1);
    }
    if (wind > -14) SMS_loadTiles(windometer_tiles + 6*32, WIND_TILE_FROM + 4, 32);
    else if (wind < -14) SMS_loadTiles(windometer_tiles + 7*32, WIND_TILE_FROM + 4, 32);
    else SMS_loadTiles(windometer_tiles + 8*32, WIND_TILE_FROM + 4, 32);
  }

  if (game.mode == GAME_MODE_ARCADE)
  {
    for (int i = 0; i < N_PLAYERS; i++)
    {
      display_angle(i);
      display_power(i);
      display_score(i);
    }
  }

  SMS_initSprites();
  UNSAFE_SMS_copySpritestoSAT();
  SMS_waitForVBlank();
  generate_skyline();

  meta_show(&game.players[0].ms);
  meta_show(&game.players[1].ms);
  SMS_waitForVBlank();
  UNSAFE_SMS_copySpritestoSAT();

#ifdef USEPSGLIB
  PSGRestoreVolumes();
  PSGSetMusicVolumeAttenuation(9);
  SMS_mapROMBank(primates_psg_bank);
  if (game.mode == GAME_MODE_ARCADE)
    PSGPlay(&primates_psg);
#endif
}

void init_game(void)
{
  for (int i = 0; i < N_PLAYERS; i++)
    game.players[i].score = 0;

  for (int i = 0; i < N_BANANAS; i++)
  {
    bananas[i].ms.flags = META_FLAG_HIDDEN;
    meta_set_frame(&bananas[i].ms, &banana_frames[0]);
    metas[i] = &bananas[i].ms;
  }
  for (int i = 0; i < N_PLAYERS; i++)
  {
    metas[N_BANANAS + i] = &game.players[i].ms;
  }

  SMS_useFirstHalfTilesforSprites(SPRITE_TILES_FROM_ABS < 256);
  if (game.mode == GAME_MODE_ARCADE)
  {
    SMS_mapROMBank(arcade_sprites_bin_bank);
    SMS_loadTiles(arcade_sprites_tiles, SPRITE_TILES_FROM_ABS, arcade_sprites_tiles_size);
    SMS_mapROMBank(arcade_sprites_palette_bin_bank);
    SMS_loadSpritePalette(arcade_sprites_palette);
  }
  else
  {
    SMS_mapROMBank(classic_sprites_bin_bank);
    SMS_loadTiles(classic_sprites_tiles, SPRITE_TILES_FROM_ABS, classic_sprites_tiles_size);
    SMS_mapROMBank(classic_sprites_palette_bin_bank);
    SMS_loadSpritePalette(classic_sprites_palette);
  }

  SMS_mapROMBank(background_palette_bin_bank);
  SMS_loadBGPalette(background_palette);
  SMS_VDPturnOnFeature(VDPFEATURE_LEFTCOLBLANK | VDPFEATURE_LOCKHSCROLL);
  SMS_setBackdropColor(DARKGRAY);
}

inline void explode_tile(unsigned int tile, int offx, int offy)
{
  uint8_t lines[8][4];
  SMS_readVRAM(lines, tile * 32, 32);
  for (int i = 0; i < 4; i++)
  {
    if (offx >= 0)
    {
      if (offy >= 0 && offy < 8)
        lines[0 + offy][i] &= ~(0x3c >> offx);
      if (offy >= -1 && offy < 7)
        lines[1 + offy][i] &= ~(0x7e >> offx);
      if (offy >= -2 && offy < 6)
        lines[2 + offy][i] &= ~(0xff >> offx);
      if (offy >= -3 && offy < 5)
        lines[3 + offy][i] &= ~(0xff >> offx);
      if (offy >= -4 && offy < 4)
        lines[4 + offy][i] &= ~(0xff >> offx);
      if (offy >= -5 && offy < 3)
        lines[5 + offy][i] &= ~(0xff >> offx);
      if (offy >= -6 && offy < 2)
        lines[6 + offy][i] &= ~(0x7e >> offx);
      if (offy >= -7 && offy < 1)
        lines[7 + offy][i] &= ~(0x3c >> offx);
    }
    else
    {
      if (offy >= 0 && offy < 8)
        lines[0 + offy][i] &= ~(0x3c << -offx);
      if (offy >= -1 && offy < 7)
        lines[1 + offy][i] &= ~(0x7e << -offx);
      if (offy >= -2 && offy < 6)
        lines[2 + offy][i] &= ~(0xff << -offx);
      if (offy >= -3 && offy < 5)
        lines[3 + offy][i] &= ~(0xff << -offx);
      if (offy >= -4 && offy < 4)
        lines[4 + offy][i] &= ~(0xff << -offx);
      if (offy >= -5 && offy < 3)
        lines[5 + offy][i] &= ~(0xff << -offx);
      if (offy >= -6 && offy < 2)
        lines[6 + offy][i] &= ~(0x7e << -offx);
      if (offy >= -7 && offy < 1)
        lines[7 + offy][i] &= ~(0x3c << -offx);
    }
  }
  SMS_loadTiles(lines, tile, 32);
}

void explode_banana(Banana *b)
{
  for (int dy = 0; dy < 2; dy++)
  {
    for (int dx = 0; dx < 2; dx++)
    {
      int x = (b->ms.x + 8*dx) / 8;
      int y = (b->ms.y + 8*dy) / 8;
      unsigned int tile = 0x1ff & SMS_getTileatXY(x, y);
      if (tile == 0) continue; // if sky tile nothing to do

      // I can't figure out why, but I can't inline the code inside this function for some reason,
      // although decorating the function as inline works - suspect an optimisation bug in SDCC
      explode_tile(tile, b->ms.x - 8*x, b->ms.y - 8*y);

      if ((b->ms.x % 8) == 0) break;
    }
    if ((b->ms.y % 8) == 0) break;
  }
#ifdef USEPSGLIB

  PSGSFXPlay(classic_building_hit_psg, SFX_CHANNEL2);
#endif
}

void do_scenery_collision_check(Banana *b)
{
  for (int dy = 0; dy < 2; dy++)
  {
    for (int dx = 0; dx < 2; dx++)
    {
      unsigned int x = (b->ms.x + 8*dx) / 8;
      unsigned int y = (b->ms.y + 8*dy) / 8;
      unsigned int tile = SMS_getTileatXY(x, y);
      if ((tile & 0x1ff) == 0) continue; // if sky tile nothing to do

      // we can't do proper collision detection if tile isn't in same half of VRAM as sprites
      // so revert to very hacky approximation
      if ((tile & 0x1ff) >= 256)
        tile = (3 & get_random()) == 0 ? 18 : 0;

      b->collision_check = true;
      SMS_addSprite(8 * x, 8 * y, tile);
      if ((b->ms.x % 8) == 0) break;
    }
    if ((b->ms.y % 8) == 0) break;
  }
}

void play_game(void)
{
  bool paused = false;
  bool game_over = false;
  int games_played = 0;

  init_game();
  reset_level();

  // main game loop
  int8_t scroll_x = 0;
  int8_t scroll_y = 0;
  while (!game_over)
  {
    SMS_waitForVBlank();
#ifdef USEPSGLIB
    SMS_saveROMBank();
    SMS_mapROMBank(primates_psg_bank);
    PSGFrame();
    SMS_mapROMBank(classic_throw_psg_bank);
    PSGSFXFrame();
    SMS_restoreROMBank();
#endif
    UNSAFE_SMS_copySpritestoSAT();

    if (SMS_queryPauseRequested())
    {
      paused = !paused;
      SMS_resetPauseRequest();
#ifdef USEPSGLIB
      if (paused)
      {
        PSGSilenceChannels();
        PSGStop();
      }
      else 
      {
        PSGResume();
        PSGRestoreVolumes();
      }
#endif
    }
    if (paused) continue;

    SMS_initSprites();

    if (!COLLISION)
    {
      // physics
      for (int i = 0; i < N_BANANAS; i++)
        move_banana(&bananas[i]);

      unsigned int keys = game.cpu_opponent ? input_read_keys_1player() : input_read_keys_2player();
      if (game.mode == GAME_MODE_ARCADE)
        handle_input_arcade(keys);
      else
        handle_input_classic(keys);
    }
    else
    {
      // collision detection
      int collision_type = COLLISION_NONE;
      for (int i = 0; i < N_BANANAS; i++)
      {
        for (int j = 0; j < N_PLAYERS; j++)
        {
          if (!game.players[j].is_exploding && meta_collides(&bananas[i].ms, &game.players[j].ms))
          {
            collision_type |= COLLISION_PLAYER;
            meta_set_frame(&game.players[j].ms, &explosion_frame[0]);
            meta_hide(&bananas[i].ms);
            meta_move_to(&bananas[i].ms, 0, 0);
            bananas[i].p.oob = true;
            game.players[j].is_exploding = true;
            explosion_occurring = true;
            game.players[(j+1)%2].score++;
#ifdef USEPSGLIB
            PSGSFXPlay(classic_celebrate_psg, SFX_CHANNEL2);
#endif
          }
        }
        // if it didn't collide with a gorilla, it might have collided with a banana
        if (collision_type == COLLISION_NONE)
        {
          for (int j = 0; j < N_BANANAS; j++)
          {
            if ((i != j) && meta_collides(&bananas[i].ms, &bananas[j].ms))
            {
              bananas[i].is_exploding = bananas[j].is_exploding = true;
              collision_type |= COLLISION_BANANA;
              explosion_occurring = true;
            }
          }
        }
      }

      if (collision_type == COLLISION_NONE)
      {
        // must be a scenery collision at this point
        // for now just explode any banana which was close to scenery
        // !TODO: later, if two bananas in check state, do individual pixel-level comparison for each one
        for (int i = 0; i < N_BANANAS; i++)
        {
          if (bananas[i].collision_check)
          {
            bananas[i].is_exploding = true;
            explosion_occurring = true;
          }
        }
      }
    }

    // keep players stable if screen is shaking
    for (int i = 0; i < N_PLAYERS; i++)
      meta_move_by(&game.players[i].ms, 0, scroll_y);

    // handle if a banana has hit something
    if (explosion_occurring)
    {
      // screen shake
      if (game.mode == GAME_MODE_ARCADE)
      {
        scroll_x = (3 & get_random()) - 2;
        scroll_y = 3 & get_random();
      }
      explosion_occurring = false;
      for (int i = 0; i < N_PLAYERS; i++)
      {
        // keep playing until the gorilla explosion animation has finished,
        // and the sound effect has stopped (in classic mode)
        // then start a new point / game over
#ifdef USEPSGLIB
        if ((game.players[i].ms.current_frame == NULL) && ((game.mode == GAME_MODE_ARCADE) || (PSGSFXGetStatus() == PSG_STOPPED)))
#else
        if (game.players[i].ms.current_frame == NULL)
#endif
        {
          game.players[i].is_exploding = false;
          games_played++;
#ifdef USEPSGLIB
          PSGSilenceChannels();
          PSGStop();
#endif
          SMS_mapROMBank(background_palette_bin_bank);
          if (game.mode == GAME_MODE_ARCADE)
            fade_out(background_palette_bin, arcade_sprites_palette_bin, arcade_sprites_palette_bin_bank);
          else
            fade_out(background_palette_bin, classic_sprites_palette_bin, classic_sprites_palette_bin_bank);
          if (games_played >= game.points_limit)
            game_over = true;
          else
            reset_level();
          continue;
        }
        else if (game.players[i].is_exploding)
        {
          explosion_occurring = true;
        }
      }
      if (!explosion_occurring)
      {
        meta_set_frame(&game.players[0].ms, &right_gorilla_frame);
        meta_set_frame(&game.players[1].ms, &left_gorilla_frame);
      }

      for (int i = 0; i < N_BANANAS; i++)
        explosion_occurring |= bananas[i].is_exploding;

      if (!explosion_occurring)
        scroll_x = scroll_y = 0;
    }
    SMS_setBGScrollX(scroll_x);
    SMS_setBGScrollY(scroll_y);
    // keep players stable if screen is shaking
    // !TODO: seems less than performant
    for (int i = 0; i < N_PLAYERS; i++)
      meta_move_by(&game.players[i].ms, 0, -scroll_y);

    // render the sprites
    meta_animate_and_render_all(metas, N_METAS);

    // display collision mask if banana(s) near building(s)
    for (int i = 0; i < N_BANANAS; i++)
    {
      bananas[i].collision_check = false;
      if (bananas[i].is_exploding)
      {
        bananas[i].p.oob = true;
        bananas[i].is_exploding = false;
        meta_hide(&bananas[i].ms);
        explode_banana(&bananas[i]);
        continue;
      }
      else if (bananas[i].p.oob || !meta_is_visible(&bananas[i].ms))
      {
        // display banana icon in arcade mode when ready to throw
        if (game.mode == GAME_MODE_ARCADE && game.players[i].banana->p.oob)
          SMS_setTileatXY(12*i + 10, 22, BANANA_ICON | TILE_USE_SPRITE_PALETTE | (i == 0 ? 0 : TILE_FLIPPED_X));
        continue;
      }

      do_scenery_collision_check(&bananas[i]);
    }
  }
}
