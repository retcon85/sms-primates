#include <stdint.h>
#include "SMSlib.h"
#ifdef USEPSGLIB // Set USEPSGLIB := true in Makefile to build with PSGlib for music and sounds
#include "PSGlib.h"
#endif
#include "assets.h"
#include "random.h"
#include "meta.h"
#include "game.h"
#include "util.h"

#pragma save
#pragma disable_warning 85
void blip(uint16_t rnd)
{
  __asm
  ld a, l
  and #0x0f
  or #0x80
  out (#0x7f), a
  ld a, h
  and #0x3f
  out (#0x7f), a
  ld a, #0x99
  out (#0x7f), a
  __endasm;
}

#pragma restore

void discombobulate(void)
{
  static uint8_t n = 0;
  uint16_t rnd = get_random();
  // SMS_setBGPaletteColor(9 + (rnd & 3), ((rnd & 0xfc) >> 2));
  uint8_t colors[] = { background_palette[9], background_palette[9], background_palette[10], background_palette[11] };
  if ((n % 16)  == 0)
  {
    blip(rnd);
    // SMS_setBGPaletteColor(9 + (rnd & 3), colors[(rnd & 0xc) >> 2]);
  }
  n++;
}

#ifdef USEPSGLIB
#define DISCOMBOBULATE do { \
  discombobulate(); \
} while(0)
#else
#define DISCOMBOBULATE {};
#endif

void generate_skyline(void)
{
#ifdef USEPSGLIB
  PSGSilenceChannels();
  PSGStop();
#endif

  // randomize player positions
  meta_move_to(&game.players[0].ms, 8 + (63 & get_random()), 0);
  meta_set_frame(&game.players[0].ms, &right_gorilla_frame);
  meta_move_to(&game.players[1].ms, 176 + (63 & get_random()), 0);
  meta_set_frame(&game.players[1].ms, &left_gorilla_frame);

  SMS_setNextTileatXY(0, 2);
  for (int i = 0; i < 20*32; i++)
    SMS_setTile(BLANK_TILE);
  SMS_VRAMmemsetW(32 * BG_TILE_FROM_LO, 0, 32 * (NUMBER_TILE_FROM - BG_TILE_FROM_LO));

  SMS_mapROMBank(background_palette_bin_bank);
  if (game.mode == GAME_MODE_ARCADE)
    fade_in(background_palette_bin, arcade_sprites_palette_bin, arcade_sprites_palette_bin_bank);
  else
    fade_in(background_palette_bin, classic_sprites_palette_bin, classic_sprites_palette_bin_bank);

  int next_lo_idx = BG_TILE_FROM_LO;
  int next_hi_idx = BG_TILE_FROM_HI;
  int x = 8;
  while (x < 256)
  {
    int height = 24 + (63 & get_random());
    int width = 16 + (15 & get_random());
    if (x + width >= 256) width = 263 - x;
    int bcol = 0;
    while (bcol == 0) bcol = 3 & get_random() & get_random();

    uint8_t tile[5][8][4];
    int top = 8 + SCREEN_TOP - height;
    int wy = -1;
    int n = 0;

    if (game.players[0].ms.x >= x && game.players[0].ms.x <= (x + width))
      meta_move_to(&game.players[0].ms, x, top - 16);
    if (game.players[1].ms.x >= x && game.players[1].ms.x <= (x + width))
      meta_move_to(&game.players[1].ms, ((x + width) < 256 ? x + width : 255) - 16, top - 16);

    // calculate and send a row of tiles at a time
    for (int row = top / 8; row < ((8 + SCREEN_TOP) / 8); row++)
    {
      DISCOMBOBULATE;
      // first calculate the next building's tiles for this tile row, one pixel row at a time
      for (int i = 0; i < 8; i++)
      {
        int bx = 0;
        int px = x % 8;
        int wx = 0;

        if ((wy == 0) || (wy == 1) || (wy == 4) || ((row == (top / 8)) && (i == 0)))
        {
          int j = 0;
          int col = wy >= 0 ? bcol : 0;
          while (bx < width)
          {
            DISCOMBOBULATE;
            tile[j][i][0] = tile[j][i][1] = tile[j][i][2] = tile[j][i][3] = 0; // default to clear sky
            while (px < 8)
            {
              if (bx >= width)
                col = 0;
              else if ((wy == 1) && (wx == 1) && ((width - bx) > 2) && (((8 * row) + i) < (5 + SCREEN_TOP)))
                col = 4 + ((3 & get_random()) == 3 ? 1 : 0);
              else if ((wy == 1) && (wx == 3))
                col = bcol;

              tile[j][i][0] = (tile[j][i][0] << 1) | ((col & 0x01) ? 1 : 0);
              tile[j][i][1] = (tile[j][i][1] << 1) | ((col & 0x02) ? 1 : 0);
              tile[j][i][2] = (tile[j][i][2] << 1) | ((col & 0x04) ? 1 : 0);
              tile[j][i][3] = (tile[j][i][3] << 1) | (col ? 1 : 0);
              px++;
              bx++;
              wx = wx == 3 ? 0 : wx + 1;
            }
            j++;
            px = 0;
          }

          if ((row == (top / 8)) && (i == 0))
            n = j;
        }
        else
        {
          for (int j = 0; j < n; j++)
          {
            tile[j][i][0] = tile[j][(i + 7) % 8][0];
            tile[j][i][1] = tile[j][(i + 7) % 8][1];
            tile[j][i][2] = tile[j][(i + 7) % 8][2];
            tile[j][i][3] = tile[j][(i + 7) % 8][3];
          }
        }
        if (((row * 8) + i + 1) >= top)
          wy = (wy == 5) ? 0 : wy + 1;
      }

      // now send the tiles to the VDP
      unsigned int next_idx = row < 19 ? next_lo_idx : next_hi_idx;
      unsigned int idx = next_idx;
      // if we're not starting on a tile boundary, the first tile might need to be merged
      if (x % 8 != 0)
        idx = 0x1ff & SMS_getTileatXY(x / 8, row);
      
      if ((idx > 0) && (idx != next_idx))
      {
        uint8_t merge[8][4];
        SMS_readVRAM(&merge, (32 * idx), 32);
        for (int i = 0; i < 8; i++)
        {
          tile[0][i][0] |= merge[i][0];
          tile[0][i][1] |= merge[i][1];
          tile[0][i][2] |= merge[i][2];
          tile[0][i][3] |= merge[i][3];
        }
        SMS_loadTiles(&tile[0], idx, 32);
        for (int i = 1; i < n; i++)
        {
          DISCOMBOBULATE;
          SMS_loadTiles(&tile[i], next_idx, 32);
          SMS_setTileatXY((x / 8) + i, row, next_idx | TILE_PRIORITY);
          next_idx++;
        }
      }
      else
      {
        for (int i = 0; i < n; i++)
        {
          DISCOMBOBULATE;
          SMS_loadTiles(&tile[i], next_idx, 32);
          SMS_setTileatXY((x / 8) + i, row, next_idx | TILE_PRIORITY);
          next_idx++;
        }
      }
      if (row < 19)
        next_lo_idx = next_idx;
      else
        next_hi_idx = next_idx;
    }
    x += width + 1;
  }
  PSGSilenceChannels();
  // restore palette to recover from discombobulation
  SMS_loadBGPalette(background_palette);
}
