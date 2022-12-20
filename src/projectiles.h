#ifndef __PROJECTILES_H__
#define __PROJECTILES_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct Projectile {
  uint32_t x;
  uint32_t y;
  int32_t vx;
  int32_t vy;
  uint8_t oob;
} Projectile;

bool projectile_move(Projectile *p);
void projectile_launch_from(Projectile *p, uint32_t x, uint32_t y, int angle, int velocity);
void projectile_set_wind_speed(int8_t speed);

#endif