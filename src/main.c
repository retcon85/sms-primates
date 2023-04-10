#include <stdint.h>
#include "SMSlib.h"
#ifdef USEPSGLIB
#include "PSGlib.h"
#endif
#include "assets.h"
#include "game.h"
#include "random.h"
#include "util.h"

inline void wait_play_seed(void)
{
  SMS_waitForVBlank();
#ifdef USEPSGLIB
    SMS_saveROMBank();
    SMS_mapROMBank(primates_psg_bank);
    PSGFrame();
    SMS_restoreROMBank();
#endif
  get_random(); // seed
}

inline void wait(int seconds)
{
  for (int i = 0; i < 60*seconds; i++)
  {
    wait_play_seed();
    if (SMS_getKeysHeld()) return; 
  }
}

inline void print_dec2(unsigned int x, unsigned int y, int value, unsigned int pad)
{
  SMS_setTileatXY(x, y, value > 9 ? value/10 + '0' : pad);
  SMS_setTileatXY(x + 1, y, value%10 + '0');
}

void show_splash(void)
{
  SMS_initSprites();
  SMS_copySpritestoSAT();
  SMS_setBackdropColor(0);

  SMS_mapROMBank(retcon_tiles_bin_bank);
  SMS_loadTiles(retcon_tiles_bin, 0, retcon_tiles_bin_size);
  SMS_mapROMBank(retcon_tilemap_bin_bank);
  SMS_loadTileMap(0, 0, retcon_tilemap_bin, retcon_tilemap_bin_size);
  SMS_mapROMBank(font_bin_bank);
  SMS_loadTiles(GLYPH(' '), 32, font_bin_size);

  wait(1);
  SMS_mapROMBank(retcon_palettes_bin_bank);
  fade_in(retcon_palettes_bin, &retcon_palettes_bin[16], retcon_palettes_bin_bank);
  wait(3);
  fade_out(retcon_palettes_bin, &retcon_palettes_bin[16], retcon_palettes_bin_bank);
  clear_screen(" ");
  wait(2);
  SMS_mapROMBank(retcon_palettes_bin_bank);
  SMS_printatXY(5, 11, "Undeveloper_ presents");
  fade_in(retcon_palettes_bin, &retcon_palettes_bin[16], retcon_palettes_bin_bank);
  wait(3);
  fade_out(retcon_palettes_bin, &retcon_palettes_bin[16], retcon_palettes_bin_bank);
  clear_screen(" ");
  wait(2);
  SMS_printatXY(6, 11, "A Retcon production");
  fade_in(retcon_palettes_bin, &retcon_palettes_bin[16], retcon_palettes_bin_bank);
  wait(3);
  fade_out(retcon_palettes_bin, &retcon_palettes_bin[16], retcon_palettes_bin_bank);
  wait(2);
  SMS_mapROMBank(splash_lower_tiles_bin_bank);
  SMS_loadTiles(splash_lower_tiles_bin, 0, splash_lower_tiles_bin_size);
  SMS_mapROMBank(splash_upper_tiles_bin_bank);
  SMS_loadTiles(splash_upper_tiles_bin, 256, splash_upper_tiles_bin_size);
  SMS_mapROMBank(splash_tilemap_bin_bank);
  SMS_loadTileMap(0, 1, splash_tilemap_bin, splash_tilemap_bin_size);
  SMS_setNextTileatLoc(0);
  for (int i = 0; i < 32; i++)
    SMS_setTile(10 | TILE_USE_SPRITE_PALETTE);
  SMS_setNextTileatXY(0, 23);
  for (int i = 0; i < 32; i++)
    SMS_setTile(10 | TILE_USE_SPRITE_PALETTE);
  SMS_mapROMBank(splash_palette_bin_bank);
  fade_in(splash_palette_bin, NULL, 0);
  wait(5);
  fade_out(splash_palette_bin, NULL, 0);
  wait(1);
}

void show_main_menu(void)
{
#ifdef USEPSGLIB
  PSGRestoreVolumes();
  PSGSetMusicVolumeAttenuation(9);
  SMS_mapROMBank(primates_psg_bank);
  PSGPlay(&primates_psg);
#endif

  SMS_mapROMBank(font_bin_bank);
  SMS_loadTiles(GLYPH(' '), 32, font_bin_size);
  clear_screen(" ");
  SMS_printatXY(8, 3, "P R I M A T E S");
  SMS_printatXY(6, 9, "  Play Arcade Mode");
  SMS_printatXY(6, 11, "  Play Classic Mode");

  SMS_mapROMBank(background_palette_bin_bank);
  fade_in(background_palette_bin, NULL, 0);

  game.cpu_opponent = true;

  int sel = 0;
  const char *hints[] = {
    "* Throw as fast as you can!  *                                    UP/DN      = set angle          LEFT/RIGHT = set power          BUTTON 1/2 = FIRE!  ",
    "* Take it in turns to throw  *                                    UP/DN      = increase amount    LEFT/RIGHT = decrease amount    BUTTON 1/2 = confirm"
  };
  while (!(SMS_getKeysPressed() & (PORT_A_KEY_1 | PORT_B_KEY_1 | PORT_A_KEY_2 | PORT_B_KEY_2)))
  {
    wait_play_seed();
    if (SMS_getKeysPressed() & (PORT_A_KEY_UP | PORT_B_KEY_UP | PORT_A_KEY_DOWN | PORT_B_KEY_DOWN))
      sel = (sel + 1) % 2;

    for (int i = 0; i < 2; i++)
    {
      SMS_printatXY(6, 9 + 2*i, i == sel ? ">" : " ");
      if (i == sel) SMS_printatXY(1, 15, hints[i]);
    }
  }
  game.mode = sel;
  while (SMS_getKeysPressed());

  SMS_setNextTileatXY(0, 13);
  for (int i = 0; i < 7*32; i++)
    SMS_setTile(' ');

  SMS_printatXY(6, 13, "  1 Player");
  SMS_printatXY(6, 15, "  2 Player");
  while (!(SMS_getKeysPressed() & (PORT_A_KEY_1 | PORT_B_KEY_1 | PORT_A_KEY_2 | PORT_B_KEY_2)))
  {
    wait_play_seed();
    if (SMS_getKeysPressed() & (PORT_A_KEY_UP | PORT_B_KEY_UP | PORT_A_KEY_DOWN | PORT_B_KEY_DOWN | PORT_A_KEY_LEFT | PORT_B_KEY_LEFT | PORT_A_KEY_RIGHT | PORT_B_KEY_RIGHT))
      game.cpu_opponent = !game.cpu_opponent;

    for (int i = 0; i < 2; i++)
      SMS_printatXY(6, 13 + 2*i, ((i == 0) == game.cpu_opponent) ? ">" : " ");
  }


  SMS_printatXY(0, 18, " Total games to play?           ");
  while (SMS_getKeysPressed());

  while (!(SMS_getKeysPressed() & (PORT_A_KEY_1 | PORT_B_KEY_1 | PORT_A_KEY_2 | PORT_B_KEY_2)))
  {
    wait_play_seed();
    if ((game.points_limit < 99) && (SMS_getKeysPressed() & (PORT_A_KEY_UP | PORT_B_KEY_UP | PORT_A_KEY_RIGHT | PORT_B_KEY_RIGHT)))
      game.points_limit++;
    else if ((game.points_limit > 1) && (SMS_getKeysPressed() & (PORT_A_KEY_DOWN | PORT_B_KEY_DOWN | PORT_A_KEY_LEFT | PORT_B_KEY_LEFT)))
      game.points_limit--;

    print_dec2(28, 18, game.points_limit, ' ');
  }

  if (game.mode == GAME_MODE_ARCADE)
  {
    SMS_printatXY(0, 19, " Both players can win point?    ");
    while (SMS_getKeysPressed());

    game.allow_death_point = true;
    while (!(SMS_getKeysPressed() & (PORT_A_KEY_1 | PORT_B_KEY_1 | PORT_A_KEY_2 | PORT_B_KEY_2)))
    {
      wait_play_seed();
      if (SMS_getKeysPressed() & (PORT_A_KEY_UP | PORT_B_KEY_UP | PORT_A_KEY_RIGHT | PORT_B_KEY_RIGHT | PORT_A_KEY_DOWN | PORT_B_KEY_DOWN | PORT_A_KEY_LEFT | PORT_B_KEY_LEFT))
        game.allow_death_point = !game.allow_death_point;

      SMS_printatXY(29, 19, game.allow_death_point ? "Y" : "N");
    }

    SMS_printatXY(0, 20, " Allow dying throw?             ");
    while (SMS_getKeysPressed());

    game.allow_death_throw = true;
    while (!(SMS_getKeysPressed() & (PORT_A_KEY_1 | PORT_B_KEY_1 | PORT_A_KEY_2 | PORT_B_KEY_2)))
    {
      wait_play_seed();
      if (SMS_getKeysPressed() & (PORT_A_KEY_UP | PORT_B_KEY_UP | PORT_A_KEY_RIGHT | PORT_B_KEY_RIGHT | PORT_A_KEY_DOWN | PORT_B_KEY_DOWN | PORT_A_KEY_LEFT | PORT_B_KEY_LEFT))
        game.allow_death_throw = !game.allow_death_throw;

      SMS_printatXY(29, 20, game.allow_death_throw ? "Y" : "N");
    }
  }

#ifdef USEPSGLIB
  PSGSilenceChannels();
  PSGStop();
#endif

  SMS_mapROMBank(background_palette_bin_bank);
  fade_out(background_palette_bin, NULL, 0);
  clear_screen(BLANK_TILE);
  SMS_waitForVBlank();
}

void show_game_over(void)
{
#ifdef USEPSGLIB
  PSGSilenceChannels();
  PSGStop();
#endif
  SMS_initSprites();
  SMS_copySpritestoSAT();
  
  SMS_mapROMBank(font_bin_bank);
  SMS_loadTiles(GLYPH(' '), 32, font_bin_size);
  clear_screen(" ");
  SMS_printatXY(11, 7, "GAME OVER!");
  SMS_printatXY(13, 9, "Score:");
  SMS_printatXY(7, 11, "Player 1");
  SMS_printatXY(7, 12, "Player 2");

  for (int i = 0; i < N_PLAYERS; i++)
    print_dec2(23, 11 + i, game.players[i].score, ' ');

  SMS_mapROMBank(background_palette_bin_bank);
  fade_in(background_palette_bin, NULL, 0);

  uint8_t counter = 0;
  while (!(SMS_getKeysPressed() & (PORT_A_KEY_1 | PORT_A_KEY_2 | PORT_B_KEY_1 | PORT_B_KEY_2)))
  {
    SMS_waitForVBlank();
    SMS_printatXY(8, 22, !(counter & 64) ? "Press any button" : "                ");
    counter++;
  }
  SMS_mapROMBank(background_palette_bin_bank);
  fade_out(background_palette_bin, NULL, 0);
}

void main(void)
{
  show_splash();
  while (true)
  {
    show_main_menu();
    play_game();
    show_game_over();
  }
}
