#include "assets.h"
#include "meta.h"

// banana animation
const MetaSpriteTile banana_tiles[8] = {
    {.tile_ref = BANANA_TILE_FROM + 0, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 1, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 2, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 3, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 4, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 5, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 6, .dx = 0, .dy = 0},
    {.tile_ref = BANANA_TILE_FROM + 7, .dx = 0, .dy = 0}};

MetaSpriteFrame banana_frames[8] = {
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[0], .next_frame = &banana_frames[1]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[1], .next_frame = &banana_frames[2]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[2], .next_frame = &banana_frames[3]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[3], .next_frame = &banana_frames[4]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[4], .next_frame = &banana_frames[5]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[5], .next_frame = &banana_frames[6]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[6], .next_frame = &banana_frames[7]},
  {.duration = 2, .tile_count = 1, .tiles = &banana_tiles[7], .next_frame = &banana_frames[0]}
};

// gorilla animation
const MetaSpriteTile left_gorilla_tiles[4] = {
    {.tile_ref = GORILLA_TILE_FROM + 0, .dx = 0, .dy = 0},
    {.tile_ref = GORILLA_TILE_FROM + 1, .dx = 8, .dy = 0},
    {.tile_ref = GORILLA_TILE_FROM + 2, .dx = 0, .dy = 8},
    {.tile_ref = GORILLA_TILE_FROM + 3, .dx = 8, .dy = 8}};

const MetaSpriteTile right_gorilla_tiles[4] = {
    {.tile_ref = GORILLA_TILE_FROM + 4, .dx = 0, .dy = 0},
    {.tile_ref = GORILLA_TILE_FROM + 5, .dx = 8, .dy = 0},
    {.tile_ref = GORILLA_TILE_FROM + 6, .dx = 0, .dy = 8},
    {.tile_ref = GORILLA_TILE_FROM + 7, .dx = 8, .dy = 8}};

const MetaSpriteFrame left_gorilla_frame = {.duration = 0, .tile_count = 4, .tiles = &left_gorilla_tiles[0]};
const MetaSpriteFrame right_gorilla_frame = {.duration = 0, .tile_count = 4, .tiles = &right_gorilla_tiles[0]};

// explosion animation
const MetaSpriteTile explosion_tiles[25][4] = {
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 0, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 1, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 2, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 3, .dx = 8, .dy = 8}
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 4, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 5, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 6, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 7, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 8, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 9, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 10, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 11, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 12, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 13, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 14, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 15, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 16, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 17, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 18, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 19, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 20, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 21, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 22, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 23, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 24, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 25, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 26, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 27, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 28, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 29, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 30, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 31, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 32, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 33, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 34, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 35, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 36, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 37, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 38, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 39, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 40, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 41, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 42, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 43, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 44, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 45, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 46, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 47, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 48, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 49, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 50, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 51, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 52, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 53, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 54, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 55, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 56, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 57, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 58, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 59, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 60, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 61, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 62, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 63, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 64, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 65, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 66, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 67, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 68, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 69, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 70, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 71, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 72, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 73, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 70, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 71, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 74, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 75, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 70, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 71, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 76, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 77, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 78, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 79, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 80, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 81, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 82, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 83, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 84, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 81, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 85, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 86, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 87, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 81, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 88, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 89, .dx = 8, .dy = 8},
  },
  {
    {.tile_ref = EXPLOSION_TILE_FROM + 90, .dx = 0, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 81, .dx = 8, .dy = 0},
    {.tile_ref = EXPLOSION_TILE_FROM + 91, .dx = 0, .dy = 8},
    {.tile_ref = EXPLOSION_TILE_FROM + 92, .dx = 8, .dy = 8}
  }
};

MetaSpriteFrame explosion_frame[25] = {
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[0][0], .next_frame = &explosion_frame[1]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[1][0], .next_frame = &explosion_frame[2]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[2][0], .next_frame = &explosion_frame[3]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[3][0], .next_frame = &explosion_frame[4]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[4][0], .next_frame = &explosion_frame[5]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[5][0], .next_frame = &explosion_frame[6]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[6][0], .next_frame = &explosion_frame[7]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[7][0], .next_frame = &explosion_frame[8]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[8][0], .next_frame = &explosion_frame[9]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[9][0], .next_frame = &explosion_frame[10]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[10][0], .next_frame = &explosion_frame[11]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[11][0], .next_frame = &explosion_frame[12]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[12][0], .next_frame = &explosion_frame[13]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[13][0], .next_frame = &explosion_frame[14]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[14][0], .next_frame = &explosion_frame[15]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[15][0], .next_frame = &explosion_frame[16]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[16][0], .next_frame = &explosion_frame[17]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[17][0], .next_frame = &explosion_frame[18]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[18][0], .next_frame = &explosion_frame[19]},
  {.duration = 8, .tile_count = 4, .tiles = &explosion_tiles[19][0], .next_frame = &explosion_frame[20]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[20][0], .next_frame = &explosion_frame[21]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[21][0], .next_frame = &explosion_frame[22]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[22][0], .next_frame = &explosion_frame[23]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[23][0], .next_frame = &explosion_frame[24]},
  {.duration = 2, .tile_count = 4, .tiles = &explosion_tiles[24][0]}
};
