#include "assets.generated.h"
#include "meta.h"

#define CHR(c) (c - 32)
#define GLYPH(c) (font_bin + 32*CHR(c))

#define CYAN 11
#define DARKGRAY 9
#define GREEN 0

#define background_palette background_palette_bin
#define background_palette_size background_palette_bin_size
#define arcade_sprites_palette arcade_sprites_palette_bin
#define arcade_sprites_palette_size arcade_sprites_palette_bin_size
#define arcade_sprites_tiles arcade_sprites_bin
#define arcade_sprites_tiles_size arcade_sprites_bin_size
#define classic_sprites_palette classic_sprites_palette_bin
#define classic_sprites_palette_size classic_sprites_palette_bin_size
#define classic_sprites_tiles classic_sprites_bin
#define classic_sprites_tiles_size classic_sprites_bin_size
#define windometer_tiles wind_bin

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT (192 - 8)
#define SCREEN_TOP (SCREEN_HEIGHT - 16)

#define BG_TILE_FROM_LO 114
#define BG_TILE_FROM_HI 320
#define LABEL_TILE_FROM 426
#define NUMBER_TILE_FROM 416
#define WIND_TILE_FROM (LABEL_TILE_FROM + 15)
#define BLANK_TILE 0

#define SPRITE_TILES_FROM_ABS 0
#define SPRITE_TILES_FROM 0
#define GORILLA_TILE_FROM (SPRITE_TILES_FROM + 5)
#define BANANA_TILE_FROM (GORILLA_TILE_FROM + 8)
#define EXPLOSION_TILE_FROM (BANANA_TILE_FROM + 8)
#define BANANA_ICON (BANANA_TILE_FROM + 7)

// banana animation
extern const MetaSpriteTile banana_tiles[8];
extern MetaSpriteFrame banana_frames[8];

// gorilla animation
extern const MetaSpriteTile left_gorilla_tiles[4];
extern const MetaSpriteTile right_gorilla_tiles[4];
extern const MetaSpriteFrame left_gorilla_frame;
extern const MetaSpriteFrame right_gorilla_frame;

// explosion animation
extern const MetaSpriteTile explosion_tiles[25][4];
extern MetaSpriteFrame explosion_frame[25];
