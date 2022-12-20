#include <stdlib.h>
#include "util.h"

inline void clear_screen(const char *with)
{
  SMS_setNextTileatLoc(0);
  for (int i = 0; i < 896; i++)
    SMS_print(with);
}

const unsigned char color_reduction[]={RGB(0,0,0), RGB(0,0,1), RGB(0,1,1), RGB(1,1,1),
                                      RGB(1,1,2), RGB(1,2,2), RGB(2,2,2),
                                      RGB(2,2,3), RGB(2,3,3), RGB(3,3,3)};

void fade_in(const void *bg_palette, const void *sprite_palette, unsigned int sprite_palette_bank)
{
  SMS_displayOff();
  for (int i = sizeof(color_reduction) - 1; i >= 0; i--)
  {
    SMS_waitForVBlank();
    SMS_waitForVBlank();

    if (bg_palette != NULL)
      SMS_loadBGPaletteafterColorSubtraction(bg_palette, color_reduction[i]);
    if (sprite_palette != NULL)
    {
      SMS_saveROMBank();
      SMS_mapROMBank(sprite_palette_bank);
      SMS_loadSpritePaletteafterColorSubtraction(sprite_palette, color_reduction[i]);
      SMS_restoreROMBank();
    }
    if (i == sizeof(color_reduction) - 1)
      SMS_displayOn();
  }
}

void fade_out(const void *bg_palette, const void *sprite_palette, unsigned int sprite_palette_bank)
{
  for (int i = 0; i < sizeof(color_reduction); i++)
  {
    SMS_waitForVBlank();
    SMS_waitForVBlank();

    if (bg_palette != NULL)
      SMS_loadBGPaletteafterColorSubtraction(bg_palette, color_reduction[i]);
    if (sprite_palette != NULL)
    {
      SMS_saveROMBank();
      SMS_mapROMBank(sprite_palette_bank);
      SMS_loadSpritePaletteafterColorSubtraction(sprite_palette, color_reduction[i]);
      SMS_restoreROMBank();
    }
  }
  SMS_displayOff();
}
