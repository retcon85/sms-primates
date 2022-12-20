#ifndef __META_H__
#define __META_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define META_FLAG_HIDDEN 1
#define META_FLAG_CUSTOM_HITBOX 2
#define META_FLAG_HITBOX_DEFINED 8

typedef struct MetaSpriteTile MetaSpriteTile;

typedef struct MetaSpriteTile
{
  uint8_t tile_ref;
  int8_t dx;
  int8_t dy;
} MetaSpriteTile;

typedef struct MetaSpriteFrame MetaSpriteFrame;

typedef struct MetaSpriteFrame
{
  uint8_t duration;
  uint8_t tile_count;
  const MetaSpriteTile *tiles;
  const MetaSpriteFrame *next_frame;
} MetaSpriteFrame;

typedef struct Hitbox
{
  uint8_t x1;
  uint8_t y1;
  uint8_t x2;
  uint8_t y2;
} Hitbox;

typedef struct MetaSprite
{
  uint8_t x;
  uint8_t y;
  const MetaSpriteFrame *current_frame;
  uint8_t counter;
  uint8_t flags;
  Hitbox hitbox;
} MetaSprite;

extern void meta_animate_and_render_all(MetaSprite *metas[], int length);
extern void meta_animate(MetaSprite *meta);
extern void meta_render(const MetaSprite *meta);
extern void meta_set_frame(MetaSprite *meta, const MetaSpriteFrame *frame);
extern void meta_move_to(MetaSprite *meta, uint8_t x, uint8_t y);
extern void meta_move_by(MetaSprite *meta, int8_t x, int8_t y);
extern void meta_show(MetaSprite *meta);
extern void meta_hide(MetaSprite *meta);
extern bool meta_is_visible(MetaSprite *meta);
extern bool meta_collides(MetaSprite *a, MetaSprite *b);

#endif
