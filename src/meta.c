#include "meta.h"
#include "SMSlib.h"

void meta_animate_and_render_all(MetaSprite *metas[], int length)
{
  for (int i = 0; i < length; i++)
  {
    meta_animate(metas[i]);
    meta_render(metas[i]);
  }
}

void meta_animate(MetaSprite *meta)
{
  if ((meta->current_frame != NULL) && (meta->current_frame->duration > 0) && (meta->counter == 0))
  {
    meta_set_frame(meta, meta->current_frame->next_frame);
  }
  else
  {
    meta->counter--;
  }
}

void meta_render(const MetaSprite *meta)
{
  if (!(meta->flags & META_FLAG_HIDDEN) && (meta->current_frame != NULL))
  {
    for (int i = 0; i < meta->current_frame->tile_count; i++)
    {
      SMS_addSprite(
          meta->x + meta->current_frame->tiles[i].dx,
          meta->y + meta->current_frame->tiles[i].dy,
          meta->current_frame->tiles[i].tile_ref);
    }
  }
}

void recalc_hitbox(MetaSprite *meta)
{
  if (meta->current_frame != NULL)
  {
    meta->hitbox.x1 = meta->x;
    meta->hitbox.x2 = meta->x + 8;
    meta->hitbox.y1 = meta->y;
    meta->hitbox.y2 = meta->y + 8;
    for (int i = 0; i < meta->current_frame->tile_count; i++)
    {
      if ((meta->x + meta->current_frame->tiles[i].dx) < meta->hitbox.x1)
        meta->hitbox.x1 = meta->x + meta->current_frame->tiles[i].dx;
      if ((meta->x + meta->current_frame->tiles[i].dx + 8) > meta->hitbox.x2)
        meta->hitbox.x2 = meta->x + meta->current_frame->tiles[i].dx + 8;
      if ((meta->y + meta->current_frame->tiles[i].dy) < meta->hitbox.y1)
        meta->hitbox.y1 = meta->y + meta->current_frame->tiles[i].dy;
      if ((meta->y + meta->current_frame->tiles[i].dy + 8) > meta->hitbox.y2)
        meta->hitbox.y2 = meta->y + meta->current_frame->tiles[i].dy + 8;
    }
    meta->flags |= META_FLAG_HITBOX_DEFINED;
  }
}

bool meta_collides(MetaSprite *a, MetaSprite *b)
{
  if (!(a->flags & (META_FLAG_HITBOX_DEFINED | META_FLAG_CUSTOM_HITBOX)))
  {
    recalc_hitbox(a);
  }
  if (!(b->flags & (META_FLAG_HITBOX_DEFINED | META_FLAG_CUSTOM_HITBOX)))
  {
    recalc_hitbox(b);
  }
  if (b->hitbox.x2 < a->hitbox.x1) return false;
  if (b->hitbox.x1 > a->hitbox.x2) return false;
  if (b->hitbox.y2 < a->hitbox.y1) return false;
  if (b->hitbox.y1 > a->hitbox.y2) return false;
  return true;
}

void meta_set_frame(MetaSprite *meta, const MetaSpriteFrame *frame)
{
  meta->current_frame = frame;
  if (frame != NULL)
  {
    meta->counter = frame->duration - 1;
  }
  meta->flags &= ~(META_FLAG_HITBOX_DEFINED);
}

inline void meta_move_to(MetaSprite *meta, uint8_t x, uint8_t y)
{
  meta->x = x;
  meta->y = y;
}

inline void meta_move_by(MetaSprite *meta, int8_t x, int8_t y)
{
  meta->x += x;
  meta->y += y;
}

inline void meta_show(MetaSprite *meta)
{
  meta->flags &= ~(META_FLAG_HIDDEN);
}

inline void meta_hide(MetaSprite *meta)
{
  meta->flags |= META_FLAG_HIDDEN;
}

inline bool meta_is_visible(MetaSprite *meta)
{
  return !(meta->flags & META_FLAG_HIDDEN);
}
