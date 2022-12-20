#include <stdbool.h>
#include <stdint.h>
#include "projectiles.h"

#define VELOCITY_SCALE 32
#define EARTH_GRAVITY 15
#define WIND_FACTOR 4
#define LIMIT 0x80000000

int8_t wind_speed = 0;

/* look up table for trig */
const int lut[] = {
  512, // sin(0) or cos(90)
  511, // sin(1) or cos(89)
  511, // sin(2) or cos(88)
  511, // sin(3) or cos(87)
  510, // sin(4) or cos(86)
  510, // sin(5) or cos(85)
  509, // sin(6) or cos(84)
  508, // sin(7) or cos(83)
  507, // sin(8) or cos(82)
  505, // sin(9) or cos(81)
  504, // sin(10) or cos(80)
  502, // sin(11) or cos(79)
  500, // sin(12) or cos(78)
  498, // sin(13) or cos(77)
  496, // sin(14) or cos(76)
  494, // sin(15) or cos(75)
  492, // sin(16) or cos(74)
  489, // sin(17) or cos(73)
  486, // sin(18) or cos(72)
  484, // sin(19) or cos(71)
  481, // sin(20) or cos(70)
  477, // sin(21) or cos(69)
  474, // sin(22) or cos(68)
  471, // sin(23) or cos(67)
  467, // sin(24) or cos(66)
  464, // sin(25) or cos(65)
  460, // sin(26) or cos(64)
  456, // sin(27) or cos(63)
  452, // sin(28) or cos(62)
  447, // sin(29) or cos(61)
  443, // sin(30) or cos(60)
  438, // sin(31) or cos(59)
  434, // sin(32) or cos(58)
  429, // sin(33) or cos(57)
  424, // sin(34) or cos(56)
  419, // sin(35) or cos(55)
  414, // sin(36) or cos(54)
  408, // sin(37) or cos(53)
  403, // sin(38) or cos(52)
  397, // sin(39) or cos(51)
  392, // sin(40) or cos(50)
  386, // sin(41) or cos(49)
  380, // sin(42) or cos(48)
  374, // sin(43) or cos(47)
  368, // sin(44) or cos(46)
  362, // sin(45) or cos(45)
  355, // sin(46) or cos(44)
  349, // sin(47) or cos(43)
  342, // sin(48) or cos(42)
  335, // sin(49) or cos(41)
  329, // sin(50) or cos(40)
  322, // sin(51) or cos(39)
  315, // sin(52) or cos(38)
  308, // sin(53) or cos(37)
  300, // sin(54) or cos(36)
  293, // sin(55) or cos(35)
  286, // sin(56) or cos(34)
  278, // sin(57) or cos(33)
  271, // sin(58) or cos(32)
  263, // sin(59) or cos(31)
  256, // sin(60) or cos(30)
  248, // sin(61) or cos(29)
  240, // sin(62) or cos(28)
  232, // sin(63) or cos(27)
  224, // sin(64) or cos(26)
  216, // sin(65) or cos(25)
  208, // sin(66) or cos(24)
  200, // sin(67) or cos(23)
  191, // sin(68) or cos(22)
  183, // sin(69) or cos(21)
  175, // sin(70) or cos(20)
  166, // sin(71) or cos(19)
  158, // sin(72) or cos(18)
  149, // sin(73) or cos(17)
  141, // sin(74) or cos(16)
  132, // sin(75) or cos(15)
  123, // sin(76) or cos(14)
  115, // sin(77) or cos(13)
  106, // sin(78) or cos(12)
  97, // sin(79) or cos(11)
  88, // sin(80) or cos(10)
  80, // sin(81) or cos(9)
  71, // sin(82) or cos(8)
  62, // sin(83) or cos(7)
  53, // sin(84) or cos(6)
  44, // sin(85) or cos(5)
  35, // sin(86) or cos(4)
  26, // sin(87) or cos(3)
  17, // sin(88) or cos(2)
  8, // sin(89) or cos(1)
  0 // sin(90) or cos(0)
};

void projectile_set_wind_speed(int8_t speed)
{
  wind_speed = speed;
}

bool projectile_move(Projectile *p) {
  if (p->oob) return p->oob;

  p->x += (p->vx / WIND_FACTOR);
  p->y += p->vy;

  if (p->x > LIMIT || p->y > LIMIT)
  {
    p->oob = true;
  }
  // else ?
  {
    p->vy -= EARTH_GRAVITY;
    p->vx += wind_speed;
  }
  
  return p->oob;
}

void projectile_launch_from(Projectile *p, uint32_t x, uint32_t y, int angle, int velocity) {
  p->x = x;
  p->y = y;
  p->vx = (((int32_t)velocity * lut[angle % 90] * (angle > 90 ? -1 : 1)) / VELOCITY_SCALE) * WIND_FACTOR;
  p->vy = ((int32_t)velocity * lut[(450 - angle) % 90]) / VELOCITY_SCALE;
  p->oob = 0; 
}
