#include <stdint.h>

uint16_t get_random(void)
{
  static uint16_t random = 0x49E5;
  uint8_t lsb = random & 1;
  random >>= 1;
  if (lsb == 1)
    random ^= 0xb400u;
  return random;
}
